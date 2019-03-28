/**
*@file IMU_Task.c
*@date 2019/2/14
*@author Satori
*@brief 用来解算imu的姿态角
*/

#include "bsp_imu.h"
#include "IMU_Task.h"
#include "GimbalTask.h"
#include "sys_config.h"
#include "cmsis_os.h"
#include "math.h"

extern Motor_t Gimbal_Motor_Pitch;
extern Motor_t Gimbal_Motor_Yaw;

uint32_t imu_time_last;
uint32_t imu_time_ms;

mpu_data_t      mpu_data;
imu_data_t      imu;
imu_attitude_t  atti;

/* imu task static parameter */
static volatile float q0 = 1.0f;
static volatile float q1 = 0.0f;
static volatile float q2 = 0.0f;
static volatile float q3 = 0.0f;
static volatile uint32_t last_update, now_update;
static volatile float exInt, eyInt, ezInt;
static volatile float gx, gy, gz, ax, ay, az, mx, my, mz;   //

extern void mpu_get_data(void);

void IMU_TaskStart(void const * argument)
{
    uint32_t imu_wake_time = osKernelSysTick();

    while(1)
    {
        imu_time_ms = HAL_GetTick() - imu_time_last;
        imu_time_last = HAL_GetTick();

        mpu_get_data();
        imu_AHRS_update();
        imu_attitude_update();
        gimbal_fdb_update();

        osDelayUntil(&imu_wake_time, IMU_PERIOD);
    }
}

/**
* @brief :  快速求平方根倒数
* @param :  x
* @retval:  1/sqrt(x)
* @note  :  none
*/
float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}

#define BOARD_DOWN 1   //

void init_quaternion(void)
{
  int16_t hx, hy;
  float temp;

  hx = imu.mx;
  hy = imu.my;

  if (hy != 0)
    temp = hx/hy;
  else
    return ;

  #ifdef BOARD_DOWN
  if(hx<0 && hy <0)   //OK
  {
    if(fabs(temp) >= 1)
    {
      q0 = -0.005;
      q1 = -0.199;
      q2 = 0.979;
      q3 = -0.0089;
    }
    else
    {
      q0 = -0.008;
      q1 = -0.555;
      q2 = 0.83;
      q3 = -0.002;
    }
    
  }
  else if (hx<0 && hy > 0) //OK
  {
    if(fabs(temp) >= 1)   
    {
      q0 = 0.005;
      q1 = -0.199;
      q2 = -0.978;
      q3 = 0.012;
    }
    else
    {
      q0 = 0.005;
      q1 = -0.553;
      q2 = -0.83;
      q3 = -0.0023;
    }
    
  }
  else if (hx > 0 && hy > 0)   //OK
  {
    if(fabs(temp) >= 1)
    {
      q0 = 0.0012;
      q1 = -0.978;
      q2 = -0.199;
      q3 = -0.005;
    }
    else
    {
      q0 = 0.0023;
      q1 = -0.83;
      q2 = -0.553;
      q3 = 0.0023;
    }
    
  }
  else if (hx > 0 && hy < 0)     //OK
  {
    if(fabs(temp) >= 1)
    {
      q0 = 0.0025;
      q1 = 0.978;
      q2 = -0.199;
      q3 = 0.008;
    }
    else
    {
      q0 = 0.0025;
      q1 = 0.83;
      q2 = -0.56;
      q3 = 0.0045;
    }
  }
  #else
    if(hx<0 && hy <0)
  {
    if(fabs(temp) >= 1)
    {
      q0 = 0.195;
      q1 = -0.015;
      q2 = 0.0043;
      q3 = 0.979;
    }
    else
    {
      q0 = 0.555;
      q1 = -0.015;
      q2 = 0.006;
      q3 = 0.829;
    }
    
  }
  else if (hx<0 && hy > 0)
  {
    if(fabs(temp) >= 1)
    {
      q0 = -0.193;
      q1 = -0.009;
      q2 = -0.006;
      q3 = 0.979;
    }
    else
    {
      q0 = -0.552;
      q1 = -0.0048;
      q2 = -0.0115;
      q3 = 0.8313;
    }
    
  }
  else if (hx>0 && hy > 0)
  {
    if(fabs(temp) >= 1)
    {
      q0 = -0.9785;
      q1 = 0.008;
      q2 = -0.02;
      q3 = 0.195;
    }
    else
    {
      q0 = -0.9828;
      q1 = 0.002;
      q2 = -0.0167;
      q3 = 0.5557;
    }
    
  }
  else if (hx > 0 && hy < 0)
  {
    if(fabs(temp) >= 1)
    {
      q0 = -0.979;
      q1 = 0.0116;
      q2 = -0.0167;
      q3 = -0.195;
    }
    else
    {
      q0 = -0.83;
      q1 = 0.014;
      q2 = -0.012;
      q3 = -0.556;
    }
  }
  #endif
}

float halfT;
float Kp  = 2.0, Ki = 0.01;

//#define Kp 2.0f    // proportional gain governs rate of convergence to accelerometer/magnetometer
//#define Ki 0.01f   // integral gain governs rate of convergence of gyroscope biases
static void imu_AHRS_update(void) 
{
  float norm;
  float hx, hy, hz, bx, bz;
  float vx, vy, vz, wx, wy, wz;
  float ex, ey, ez;//, halfT;
  float tempq0,tempq1,tempq2,tempq3;

  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;

  gx = imu.wx;
  gy = imu.wy;
  gz = imu.wz;
  ax = imu.ax;
  ay = imu.ay;
  az = imu.az;
  mx = imu.mx;
  my = imu.my;
  mz = imu.mz;

  now_update = HAL_GetTick(); //ms
  halfT =  ((float)(now_update - last_update) / 2000.0f);
  last_update = now_update;


  //Fast inverse square-root
  norm = invSqrt(ax*ax + ay*ay + az*az);
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;

  norm = invSqrt(mx*mx + my*my + mz*mz);
  mx = mx * norm;
  my = my * norm;
  mz = mz * norm;

  // compute reference direction of flux
  hx = 2.0f*mx*(0.5f - q2q2 - q3q3) + 2.0f*my*(q1q2 - q0q3) + 2.0f*mz*(q1q3 + q0q2);
  hy = 2.0f*mx*(q1q2 + q0q3) + 2.0f*my*(0.5f - q1q1 - q3q3) + 2.0f*mz*(q2q3 - q0q1);
  hz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2);
  bx = sqrt((hx*hx) + (hy*hy));
  bz = hz; 

  // estimated direction of gravity and flux (v and w)
  vx = 2.0f*(q1q3 - q0q2);
  vy = 2.0f*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
  wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
  wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
  wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);  

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) + (my*wz - mz*wy);
  ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

  if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
  {
      exInt = exInt + ex * Ki * halfT;
      eyInt = eyInt + ey * Ki * halfT;
      ezInt = ezInt + ez * Ki * halfT;
      // PI
      gx = gx + Kp*ex + exInt;
      gy = gy + Kp*ey + eyInt;
      gz = gz + Kp*ez + ezInt;
  }
  // 
  tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

  //normalise quaternion
  norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
  q0 = tempq0 * norm;
  q1 = tempq1 * norm;
  q2 = tempq2 * norm;
  q3 = tempq3 * norm;

}

//int yaw_a_js;
static void imu_attitude_update(void)
{
  imu.rol = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1)* 57.3; // roll       -pi----pi
  imu.pit = asin(-2*q1*q3 + 2*q0*q2)* 57.3;                         // pitch    -pi/2----pi/2 
  imu.yaw = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1)* 57.3; // yaw        -pi----pi
  
  
  if (imu.yaw - atti.last_yaw > 330)
    atti.yaw_cnt--;
  else if (imu.yaw - atti.last_yaw < -330)
    atti.yaw_cnt++;
  
  atti.last_yaw = imu.yaw;
  
  atti.yaw   = imu.yaw + atti.yaw_cnt*360;
  atti.pitch = -imu.rol;
  atti.roll  = imu.pit;
}

void gimbal_fdb_update(void)
{
  //IMU的数据我就只要pitch轴的角度了，yaw轴的解算没融合磁力计数据垃圾的一批
  Gimbal_Motor_Pitch.pid.pos_fdb = atti.pitch;
  //速度反馈两个我都要
  Gimbal_Motor_Pitch.pid.spd_fdb = mpu_data.gz / 16.384f;
  Gimbal_Motor_Yaw.pid.spd_ref   = (mpu_data.gx / 16.384f) + 4.0f;
}
