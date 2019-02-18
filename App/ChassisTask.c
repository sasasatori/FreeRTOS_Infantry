/**
*@file ChassisTask.c
*@date 2019/2/14
*@author Satori
*@brief 总之就是执行底盘任务
*/

#include "ChassisTask.h"
#include "bsp_can.h"

#include "sys_config.h"
#include "cmsis_os.h"

/*———————————————————————————————宏定义———————————————————————————————*/

#define SWAY_ANGLE      40

/*———————————————————————————————定义变量———————————————————————————————*/

//冷静的掏出我的底盘结构体，并定义了一个结构体变量
chassis_t chassis;

//没有什么意义的CAN发送任务
extern osThreadId CanMsg_Send_TaskHandle;

//定义两个时间有关的变量用来监控任务是否正常定时执行
uint32_t chassis_time_last;
uint32_t chassis_time_ms;

/*———————————————————————————————任务函数———————————————————————————————*/

/**
* @brief :  定时执行的底盘任务
* @param :  NONE
* @retval:  NONE
* @note  :  定时执行的任务，所以不用写while(1)，最后会给CanMsg_Send_Task发通知来着
*/

void Chassis_Task(void const * argument)
{
    //对没错，就是看看有没有正常的定时执行而已
    chassis_time_ms = HAL_GetTick() - chassis_time_last;
    chassis_time_last = HAL_GetTick();

    switch (chassis.chassis_mode)
    {
        case CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL:
        {
            ;
        }break;

        case CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL:
        {
            ;
        }break;

        case CHASSIS_FREE_KEYMOUSE_CONTROL:
        {
            ;
        }break;

        case CHASSIS_SWAY:
        {
            ;
        }break;

        case CHASSIS_STOP:
        {
            ;
        }break;
    
        default:
        {
            Error_Handler();
        }break;
    }

    osSignalSet(CanMsg_Send_TaskHandle, CHASSIS_SEND_SIGNAL);
}

/*———————————————————————————————其他执行函数———————————————————————————————*/
