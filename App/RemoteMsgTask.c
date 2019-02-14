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

/*———————————————————————————————定义变量———————————————————————————————*/



/*———————————————————————————————任务函数———————————————————————————————*/

/**
* @brief :  遥控器实现模式切换  
* @param :  NONE
* @retval:  NONE
* @note  :  从接收那边给一个唤醒，然后执行模式切换，天哪好好的一个os我就用来执行模式切换
*/

void RemoteMsg_Receive_ModeSw_TaskStart(void * argument)
{
    while(1)
    {
        ;
    }
}

/*———————————————————————————————其他执行函数———————————————————————————————*/

/**
* @brief :  遥控器解码
* @param :  
* @retval:  NONE
* @note  :  总之就是解码
*/

void RemoteMsg_decode_handler(remote_info_t *rc , uint8_t *buff)
{
    rc->remote.ch0 = ((uint16_t)buff[0] | (uint16_t)buff[1] << 8) & 0x07FF;
    rc->remote.ch0 -= 1024;

    rc->remote.ch1 = ((uint16_t)buff[1] >> 3 | (uint16_t)buff[2] << 5) & 0x07FF;
    rc->remote.ch1 -= 1024;

    rc->remote.ch2 = ((uint16_t)buff[2] >> 6 | (uint16_t)buff[3] << 2 | (uint16_t)buff[4] << 10) & 0x07FF;
    rc->remote.ch2 -= 1024;
    
    rc->remote.ch3 = ((uint16_t)buff[4] >> 1 | (uint16_t)buff[5] << 7) & 0x07FF;
    rc->remote.ch3 -= 1024;

    //高明之处：防止一下零飘
    if(rc->remote.ch0 <=5 && rc->remote.ch0 >= -5)
        rc->remote.ch0 = 0;
    if(rc->remote.ch1 <=5 && rc->remote.ch1 >= -5)
        rc->remote.ch1 = 0;
    if(rc->remote.ch2 <=5 && rc->remote.ch2 >= -5)
        rc->remote.ch2 = 0;
    if(rc->remote.ch3 <=5 && rc->remote.ch3 >= -5)
        rc->remote.ch3 = 0;

    rc->remote.s1 = (((uint8_t)buff[5] >> 4) & 0x000C) >> 2;
    rc->remote.s2 = ((uint8_t)buff[5] >> 4) & 0x0003;

    
}
