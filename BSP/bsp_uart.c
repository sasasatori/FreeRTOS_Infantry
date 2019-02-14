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


/*��������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  ���ڽ���
* @param :  ���ھ��
* @retval:  NONE
* @note  :  ������ڿ���+�жϣ����ûص�����
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
* @brief :  �ص�����
* @param :  ���ھ��
* @retval:  NONE
* @note  :  �жϻص�
*/

void uart_rx_callback(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    //�����￪ʼ�ָ���ͬ����Ϣ��Դ

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
* @brief :  ����dma����
* @param :  dma���
* @retval:  ����
* @note  :  NONE
*/

uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
    return ((uint16_t)(dma_stream->NDTR));
}
