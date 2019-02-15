/**
*@file  bsp_uart.c 
*@date  2019/2/13
*@author Satori
*@brief ���ڵ�bsp�㣬������ң����������ϵͳ���Ӿ��Ľ���
*/

/*������������������������������������������������������������������ͷ�ļ���������������������������������������������������������������*/

#include "RemoteMsgTask.h"

#include "bsp_uart.h"
#include "usart.h"
#include "cmsis_os.h"
#include "sys_config.h"

/*�����������������������������������������������������������������������������������������������������������������������������������*/

uint8_t             dbus_buff[DBUS_BUF_LENTH];
remote_info_t       remote_data;

/*��������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  ���ڽ���
* @param :  ���ھ��
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
* @brief :  �ص�����
* @param :  ���ھ��
* @retval:  NONE
* @note  :  ���ǻص�
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
            //������֪ͨ�ķ�ʽ������ģʽ�л�
            Remote_Mode_Select(&remote_data);
        }
    }
}

/**
* @brief :  ��ʼ���ڲ������ж�
* @param :  NONE
* @retval:  NONE
* @note  :  ����ʼ��
*/

void RemoteMsg_Receive_Init(void)
{
    __HAL_UART_CLEAR_IDLEFLAG(&DBUS_HUART);
    __HAL_UART_ENABLE_IT(&DBUS_HUART,UART_IT_IDLE);
}

