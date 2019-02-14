/**
*@file      bsp_uart.h
*@date      2019/2/14
*@author    Satori
*@brief     bsp_uart��ͷ�ļ����ϻ���
*/

#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include "usart.h"

/*���������������������������������������������������������������ṹ�塪������������������������������������������������������������*/

/*��������������������������������������������������������������������������������������������������������������������������������*/

/*��������������������������������������������������������������������������������������������������������������������������������*/
void Uart_Recieve_Handler(UART_HandleTypeDef *huart);
void uart_rx_callback(UART_HandleTypeDef *huart);

uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream);

#endif /*_BSP_UART_H_*/
