/**
*@file  bsp_uart.c 
*@date  2019/2/13
*@author Satori
*@brief 串口的bsp层，包含了遥控器，裁判系统，视觉的解码
*/

/*———————————————————————————————引用头文件———————————————————————————————*/

#include "RemoteMsgTask.h"

#include "bsp_uart.h"
#include "usart.h"
#include "cmsis_os.h"
#include "sys_config.h"

/*———————————————————————————————定义变量———————————————————————————————*/

uint8_t             dbus_buff[DBUS_BUF_LENTH];
remote_info_t       remote_data;

/*———————————————————————————————函数———————————————————————————————*/

/**
* @brief :  串口接收
* @param :  串口句柄
* @retval:  NONE
* @note  :  NONE
*/

void Uart_Recieve_Handler(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) && 
      __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
  {
    uart_rx_callback(huart);
  }
}

/**
* @brief :  回调函数
* @param :  串口句柄
* @retval:  NONE
* @note  :  就是回调
*/

static void uart_rx_callback(UART_HandleTypeDef *huart)
{
	  uint32_t rx_data_len = 0;
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    
    if (huart == &DBUS_HUART)
    {
        (void)USART1->SR;
        (void)USART1->DR;
        __HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx,DMA_FLAG_TCIF2_6);
        HAL_UART_DMAStop(&huart1);
        rx_data_len = DBUS_MAX_LENTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
        HAL_UART_Receive_DMA(&huart1, dbus_buff, DBUS_MAX_LENTH);  
        if(rx_data_len == DBUS_BUF_LENTH)
        {
            RemoteMsg_decode_handler(&remote_data,dbus_buff);
            //以任务通知的方式来进行模式切换
            Remote_Mode_Select(&remote_data);
        }
    }
}

/**
* @brief :  开始串口不定长中断
* @param :  NONE
* @retval:  NONE
* @note  :  见初始化
*/

void RemoteMsg_Receive_Init(void)
{
    __HAL_UART_CLEAR_IDLEFLAG(&DBUS_HUART);
    __HAL_UART_ENABLE_IT(&DBUS_HUART,UART_IT_IDLE);
}

