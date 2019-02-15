/**
*@file RemoteControl.c
*@date 2019/2/13
*@author Satori
*@brief 接收遥控器并解码
*/

/*———————————————————————————————引用头文件———————————————————————————————*/
#include "bsp_uart.h"

#include "RemoteMsgTask.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "main.h"

#include "ChassisTask.h"
#include "GimbalTask.h"

#include "sys_config.h"

/*———————————————————————————————定义变量———————————————————————————————*/

extern osThreadId RemoteMsg_Receive_ModeSw_TaskHandle;

/*———————————————————————————————任务函数———————————————————————————————*/

/**
* @brief :  遥控器实现模式切换  
* @param :  NONE
* @retval:  NONE
* @note  :  从接收那边给一个唤醒，然后执行模式切换，天哪好好的一个os我就用来执行模式切换
*/

void RemoteMsg_Receive_ModeSw_TaskStart(void * argument)
{
    osEvent Event;
    while(1)
    {
        Event = osSignalWait(RC_MODE_SIGNAL | KM_MODE_SIGNAL | ST_MODE_SIGNAL , osWaitForever);

        //然后就是分一下模式咯
        if(Event.status == osEventSignal)
        {
            if(Event.value.signals & RC_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,RESET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,SET);
            }
            if(Event.value.signals & KM_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,RESET);
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,SET);
            }
            if(Event.value.signals & ST_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,RESET);
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,RESET);
            }
        }
    }
}

/*———————————————————————————————其他执行函数———————————————————————————————*/

/**
* @brief :  遥控器解码
* @param :  
* @retval:  NONE
* @note  :  总之就是解码
*/

void RemoteMsg_decode_handler(remote_info_t *remote_data , uint8_t *buff)
{
    remote_data->remote.ch0 = ((uint16_t)buff[0] | (uint16_t)buff[1] << 8) & 0x07FF;
    remote_data->remote.ch0 -= 1024;

    remote_data->remote.ch1 = ((uint16_t)buff[1] >> 3 | (uint16_t)buff[2] << 5) & 0x07FF;
    remote_data->remote.ch1 -= 1024;

    remote_data->remote.ch2 = ((uint16_t)buff[2] >> 6 | (uint16_t)buff[3] << 2 | (uint16_t)buff[4] << 10) & 0x07FF;
    remote_data->remote.ch2 -= 1024;
    
    remote_data->remote.ch3 = ((uint16_t)buff[4] >> 1 | (uint16_t)buff[5] << 7) & 0x07FF;
    remote_data->remote.ch3 -= 1024;

    //防止一下零飘
    if(remote_data->remote.ch0 <=5 && remote_data->remote.ch0 >= -5)
        remote_data->remote.ch0 = 0;
    if(remote_data->remote.ch1 <=5 && remote_data->remote.ch1 >= -5)
        remote_data->remote.ch1 = 0;
    if(remote_data->remote.ch2 <=5 && remote_data->remote.ch2 >= -5)
        remote_data->remote.ch2 = 0;
    if(remote_data->remote.ch3 <=5 && remote_data->remote.ch3 >= -5)
        remote_data->remote.ch3 = 0;

    remote_data->remote.s1 = (((uint8_t)buff[5] >> 4) & 0x000C) >> 2;
    remote_data->remote.s2 = ((uint8_t)buff[5] >> 4) & 0x0003;

}

/**
* @brief :  遥控器模式选择
* @param :  remote_data
* @retval:  NONE
* @note  :  遥控器的s2用来控制模式，打到上面是遥控，打到下面是停止，打到中间是键鼠
*/

void Remote_Mode_Select(remote_info_t *remote_data)
{
    static uint8_t mode_last = 0;
    uint8_t mode_now;
    mode_now = remote_data->remote.s2;
    
    if(mode_now != mode_last)
    {
        if(mode_now == RC_MODE)
        {
            osSignalSet(RemoteMsg_Receive_ModeSw_TaskHandle,RC_MODE_SIGNAL);
        }
        else if(mode_now == KM_MODE)
        {
            osSignalSet(RemoteMsg_Receive_ModeSw_TaskHandle,KM_MODE_SIGNAL);
        }
        else if(mode_now == ST_MODE)
        {
            osSignalSet(RemoteMsg_Receive_ModeSw_TaskHandle,ST_MODE_SIGNAL);
        }
        else
        {
            Error_Handler();
        }
    }

    mode_last = mode_now;
};
