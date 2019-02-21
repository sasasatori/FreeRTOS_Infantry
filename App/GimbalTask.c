/**
*@file  gimbalTask.c
*@date  2019/2/14
*@author Satori
*@brief 总之就是执行云台的任务
*/

#include "GimbalTask.h"
#include "bsp_can.h"

#include "sys_config.h"
#include "cmsis_os.h"

/*———————————————————————————————定义变量———————————————————————————————*/

//掏出云台结构体
gimbal_t gimbal;

//CAN发送任务和射击任务
extern osThreadId CanMsg_Send_TaskHandle;
extern osThreadId Shoot_TaskHandle;

//定义两个时间有关的变量用来监测任务是否正常定时执行
uint32_t gimbal_time_last;
uint32_t gimbal_time_ms;

Motor_t Gimbal_Motor_Yaw;
Motor_t Gimbal_Motor_Pitch;

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

        case GIMBAL_STOP:
        {
            Gimbal_Stop_Handler();
        }break;

        default:
        {
            Error_Handler();
        }break;
    }
    
    osSignalSet(Shoot_TaskHandle,SHOOT_SEND_SIGNAL);
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
    gimbal.yaw_gyro_ref     = 0;
    gimbal.pitch_gyro_ref   = 0;
}
