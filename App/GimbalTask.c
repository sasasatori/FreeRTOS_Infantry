/**
*@file  gimbalTask.c
*@date  2019/2/14
*@author Satori
*@brief 总之就是执行云台的任务
*/

#include "GimbalTask.h"
#include "bsp_can.h"
#include "RemoteMsgTask.h"
#include "sys_config.h"
#include "cmsis_os.h"

/*———————————————————————————————定义变量———————————————————————————————*/

//掏出云台结构体
gimbal_t gimbal;

//前面一个大括号里面是速度环的pid参数，后面一个是位置环的pid参数
Motor_t Gimbal_Motor_Yaw    = {{{-3.0f,0.0f,0.0f},{-90.0f,0.0f,0.0f}}};
Motor_t Gimbal_Motor_Pitch  = {{{3.0f,0.0f,0.0f},{60.0f,0.0008f,0.0f}}};

//CAN发送任务和射击任务
extern osThreadId CanMsg_Send_TaskHandle;
extern osThreadId Shoot_TaskHandle;

extern remote_info_t remote_data;

//定义两个时间有关的变量用来监测任务是否正常定时执行
uint32_t gimbal_time_last;
uint32_t gimbal_time_ms;

/*———————————————————————————————任务函数———————————————————————————————*/

/**
* @brief :  定时执行的云台任务，且控制周期比较短
* @param :  NONE
* @retval:  NONE
* @note  :  因为是定时执行的任务所以不用写while(1)，最后会给shoot_task和CanMsg_Send_Task发送通知
*/

void Gimbal_Task(void const * argument)
{
    gimbal_time_ms = HAL_GetTick() - gimbal_time_last;
    gimbal_time_last = HAL_GetTick();

    if((gimbal.gimbal_mode != GIMBAL_REMOTE_CONTROL) &&
       (gimbal.gimbal_mode != GIMBAL_KEYMOUSE_CONTROL) &&
       (gimbal.gimbal_mode != GIMBAL_AUTO))
    {
        Gimbal_Stop_Handler();
    }
    else
    {
        switch (gimbal.gimbal_mode)
        {
            case GIMBAL_REMOTE_CONTROL:
            {
                Gimbal_Remote_Control_Handler();
            }break;

            case GIMBAL_KEYMOUSE_CONTROL:
            {
                Gimbal_Keymouse_Control_Handler();
            }break;

            case GIMBAL_AUTO:
            {
                Gimbal_Auto_Handler();
            }break;

            default:
            {
                Error_Handler();
            }break;
        }
        
        gimbal_pid_calc(&Gimbal_Motor_Yaw);
        gimbal_pid_calc(&Gimbal_Motor_Pitch);
    }
    
    osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
}

/*———————————————————————————————其他执行函数———————————————————————————————*/

/**
* @brief :  遥控器控制
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Gimbal_Remote_Control_Handler(void)
{
    ;
}

/**
* @brief :  键鼠控制
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Gimbal_Keymouse_Control_Handler(void)
{
    ;
}

/**
* @brief :  云台自动控制
* @param :  NONE
* @retval:  NONE
* @note  :  我也不知道怎么自动控制，反正先留着呗
*/

void Gimbal_Auto_Handler(void)
{
    ;
}

/**
* @brief :  总之停就对了
* @param :  NONE
* @retval:  NONE
* @note  :  没啥好说
*/

void Gimbal_Stop_Handler(void)
{
    Gimbal_Motor_Pitch.pid.output   = 0;
    Gimbal_Motor_Yaw.pid.output     = 0;
}

/**
* @brief :  双环pid计算
* @param :  Motor
* @retval:  none
* @note  :  我的pid是无敌的
*/

void gimbal_pid_calc(Motor_t *Motor)
{
    float pos_error,spd_error;

    pos_error = Motor->pid.pos_ref - Motor->pid.pos_fdb;
    Motor->pid.sum_pos += pos_error;
    Motor->pid.derror_pos = Motor->pid.error_pos[0] - Motor->pid.error_pos[1];

    Motor->pid.error_pos[1] = Motor->pid.error_pos[0];
    Motor->pid.error_pos[0] = pos_error;

    Motor->pid.spd_ref = -(Motor->pid.pos_parament.kp * pos_error + 
                           Motor->pid.pos_parament.ki * Motor->pid.sum_pos +
                           Motor->pid.pos_parament.kd * Motor->pid.derror_pos);


    spd_error = Motor->pid.spd_ref - Motor->pid.spd_fdb;
    Motor->pid.sum_spd += spd_error;
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = spd_error;

    Motor->pid.output = Motor->pid.spd_parament.kp * spd_error + 
                        Motor->pid.spd_parament.ki * Motor->pid.sum_spd + 
                        Motor->pid.spd_parament.kd * Motor->pid.derror_spd;
        
    //计算完pid，加个限幅
    if(Motor->pid.output >= GIMBAL_SPD_MAX)
    Motor->pid.output = GIMBAL_SPD_MAX;
    if(Motor->pid.output <= -GIMBAL_SPD_MAX)
    Motor->pid.output = -GIMBAL_SPD_MAX;

}

/**
* @brief : 用于整定参数 
* @param :  *Motor
* @retval:  none
* @note  :  就是整定参数
*/

float pos_kp,pos_ki,pos_kd;
float spd_kp,spd_ki,spd_kd;

void gimbal_pid_parament_fix(Motor_t *Motor)
{
    float pos_error,spd_error;

    pos_error = Motor->pid.pos_ref - Motor->pid.pos_fdb;
    Motor->pid.sum_pos += pos_error;
    Motor->pid.derror_pos = Motor->pid.error_pos[0] - Motor->pid.error_pos[1];

    Motor->pid.error_pos[1] = Motor->pid.error_pos[0];
    Motor->pid.error_pos[0] = pos_error;

    //调参用
    Motor->pid.spd_ref = -(pos_kp * pos_error + 
                           pos_ki * Motor->pid.sum_pos +
                           pos_kd * Motor->pid.derror_pos);

    spd_error = Motor->pid.spd_ref - Motor->pid.spd_fdb;
    Motor->pid.sum_spd += spd_error;
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = spd_error;

    
    Motor->pid.output = spd_kp * spd_error + 
                        spd_ki * Motor->pid.sum_spd + 
                        spd_kd * Motor->pid.derror_spd;
    
    //计算完pid，加个限幅
    if(Motor->pid.output >= GIMBAL_SPD_MAX)
    Motor->pid.output = GIMBAL_SPD_MAX;
    if(Motor->pid.output <= -GIMBAL_SPD_MAX)
    Motor->pid.output = -GIMBAL_SPD_MAX;
}
