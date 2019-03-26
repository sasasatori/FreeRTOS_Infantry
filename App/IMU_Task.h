#ifndef _IMU_H_
#define _IMU_H_

#include "bsp_imu.h"
#include "cmsis_os.h"

typedef struct 
{
    int roll_cnt;
    int pitch_cnt;
    int yaw_cnt;

    float last_roll;
    float last_pitch;
    float last_yaw;

    float roll;
    float pitch;
    float yaw;
} imu_attitude_t;

typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t mx;
    int16_t my;
    int16_t mz;

    int16_t temp;

    int16_t gx;
    int16_t gy;
    int16_t gz;

    int16_t ax_offset;
    int16_t ay_offset;
    int16_t az_offset;

    int16_t gx_offset;
    int16_t gy_offset;
    int16_t gz_offset;
} mpu_data_t;

typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t mx;
    int16_t my;
    int16_t mz;

    float wx;
    float wy;
    float wz;

    float vx;
    float vy;
    float vz;

    float rol;
    float pit;
    float yaw;
} imu_data_t;

extern mpu_data_t mpu_data;
extern imu_data_t imu;
extern imu_attitude_t atti;

void init_quaternion(void);
void IMU_TaskStart(void const * argument);
float invSqrt(float x);
void init_quaternion(void);
void imu_AHRS_update(void);
void imu_attitude_update(void);

#endif /*_IMU_H_*/
