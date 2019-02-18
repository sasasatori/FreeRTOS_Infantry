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



    osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
}

/*———————————————————————————————其他执行函数———————————————————————————————*/
