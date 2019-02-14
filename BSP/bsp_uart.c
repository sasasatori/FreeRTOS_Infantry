/**
*@file  bsp_uart.c 
*@date  2019/2/13
*@author Satori
*@brief ´®¿ÚµÄbsp²ã£¬°üº¬ÁËÒ£¿ØÆ÷£¬²ÃÅÐÏµÍ³£¬ÊÓ¾õµÄ½âÂë
*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÒýÓÃÍ·ÎÄ¼þ¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

#include "RemoteMsgTask.h"

#include "bsp_uart.h"
#include "usart.h"
#include "cmsis_os.h"
#include "sys_config.h"

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¶¨Òå±äÁ¿¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

uint8_t             dbus_buff[DBUS_BUF_LENTH];
remote_info_t       remote_data;

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªº¯Êý¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/**
* @brief :  ´®¿Ú½ÓÊÕ
* @param :  ´®¿Ú¾ä±ú
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
* @brief :  »Øµ÷º¯Êý
* @param :  ´®¿Ú¾ä±ú
* @retval:  NONE
* @note  :  ¾ÍÊÇ»Øµ÷
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
        }
    }
}

/**
* @brief :  ¿ªÊ¼´®¿Ú²»¶¨³¤ÖÐ¶Ï
* @param :  NONE
* @retval:  NONE
* @note  :  ¼û³õÊ¼»¯
*/

void RemoteMsg_Receive_Init(void)
{
    __HAL_UART_CLEAR_IDLEFLAG(&DBUS_HUART);
    __HAL_UART_ENABLE_IT(&DBUS_HUART,UART_IT_IDLE);
}

