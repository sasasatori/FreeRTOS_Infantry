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


/*———————————————————————————————函数———————————————————————————————*/

/**
* @brief :  串口接收
* @param :  串口句柄
* @retval:  NONE
* @note  :  如果串口空闲+中断，调用回调函数
*/

void Uart_Recieve_Handler(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) &&
        __HAL_UART_GET_IT_SOURCE(huart,UART_IT_IDLE))
        {
            uart_rx_callback(huart);
        }
}

/**
* @brief :  回调函数
* @param :  串口句柄
* @retval:  NONE
* @note  :  中断回调
*/

void uart_rx_callback(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    //从这里开始分给不同的信息来源

    if (huart == &DBUS_HUART)
    {
        __HAL_DMA_DISABLE(huart->hdmarx);
        
        if((DBUS_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == DBUS_BUF_LEN)
        {
            
        }
    }

    // if (huart == JUDGE_HUART)
    // {

    // }

    // if (huart == COMPUTER_HUART)
    // {

    // }

}

/**
* @brief :  返回dma长度
* @param :  dma句柄
* @retval:  长度
* @note  :  NONE
*/

uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
    return ((uint16_t)(dma_stream->NDTR));
}
