/**
*@file      bsp_uart.h
*@date      2019/2/14
*@author    Satori
*@brief     bsp_uart議遊猟周�┠六娃�
*/

#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include "usart.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！潤更悶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
void Uart_Recieve_Handler(UART_HandleTypeDef *huart);
void uart_rx_callback(UART_HandleTypeDef *huart);

uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream);

#endif /*_BSP_UART_H_*/
