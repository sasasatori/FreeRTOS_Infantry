/**
*@file ChassisTask.c
*@date 2019/2/14
*@author Satori
*@brief 总之就是执行底盘任务
*/

#include "RemoteMsgTask.h"
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
extern remote_info_t remote_data;

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
            Chassis_Remote_Control_Handler();
        }break;

        case CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL:
        case CHASSIS_FREE_KEYMOUSE_CONTROL:
        {
            Chassis_Keymouse_Control_Handler();
        }break;

        case CHASSIS_SWAY:
        {
            Chassis_Sway_Handler();
        }break;

        case CHASSIS_STOP:
        {
            Chassis_Stop_Handler();
        }break;
    
        default:
        {
            Error_Handler();
        }break;
    }

    osSignalSet(CanMsg_Send_TaskHandle, CHASSIS_SEND_SIGNAL);
}

/*———————————————————————————————其他执行函数———————————————————————————————*/

//遥控模式
void Chassis_Remote_Control_Handler(void)
{
    //第一段用来做遥控器到底盘的解算
    ;
}

//键鼠模式
void Chassis_Keymouse_Control_Handler(void)
{
    ;
}

//晃tm的
void Chassis_Sway_Handler(void)
{
    ;
}

//THE WORLD！時よ！止まれ！
void Chassis_Stop_Handler(void)
{
    chassis.vx = 0;
    chassis.vy = 0;
    chassis.vw = 0;
}
