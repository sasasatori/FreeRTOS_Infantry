/**
*@file RemoteControl.c
*@date 2019/2/13
*@author Satori
*@brief ����ң����������
*/

/*������������������������������������������������������������������ͷ�ļ���������������������������������������������������������������*/
#include "bsp_uart.h"

#include "RemoteMsgTask.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "main.h"

#include "ChassisTask.h"
#include "GimbalTask.h"

#include "sys_config.h"

/*�����������������������������������������������������������������������������������������������������������������������������������*/

extern osThreadId RemoteMsg_Receive_ModeSw_TaskHandle;

/*����������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  ң����ʵ��ģʽ�л�  
* @param :  NONE
* @retval:  NONE
* @note  :  �ӽ����Ǳ߸�һ�����ѣ�Ȼ��ִ��ģʽ�л������ĺúõ�һ��os�Ҿ�����ִ��ģʽ�л������������ͨ����һ��ȫ�ֱ�����n
*/

void RemoteMsg_Receive_ModeSw_TaskStart(void * argument)
{
    osEvent Event;
    while(1)
    {
        Event = osSignalWait(RC_MODE_SIGNAL | KM_MODE_SIGNAL | ST_MODE_SIGNAL , osWaitForever);

        //Ȼ����Ƿ�һ��ģʽ��
        if(Event.status == osEventSignal)
        {
            if(Event.value.signals & RC_MODE_SIGNAL)
            {
                //����ң��������ģʽ
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,RESET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,SET);
            }
            if(Event.value.signals & KM_MODE_SIGNAL)
            {
                //����������ģʽ
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,RESET);
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,SET);
            }
            if(Event.value.signals & ST_MODE_SIGNAL)
            {
                //����ֹͣģʽ
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,RESET);
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,RESET);
            }
        }
    }
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

/**
* @brief :  ң��������
* @param :  
* @retval:  NONE
* @note  :  ��֮���ǽ���
*/

void RemoteMsg_decode_handler(remote_info_t *remote_data , uint8_t *buff)
{
    //ң�����Ľ���
    remote_data->remote.ch0 = ((uint16_t)buff[0] | (uint16_t)buff[1] << 8) & 0x07FF;
    remote_data->remote.ch0 -= 1024;

    remote_data->remote.ch1 = ((uint16_t)buff[1] >> 3 | (uint16_t)buff[2] << 5) & 0x07FF;
    remote_data->remote.ch1 -= 1024;

    remote_data->remote.ch2 = ((uint16_t)buff[2] >> 6 | (uint16_t)buff[3] << 2 | (uint16_t)buff[4] << 10) & 0x07FF;
    remote_data->remote.ch2 -= 1024;
    
    remote_data->remote.ch3 = ((uint16_t)buff[4] >> 1 | (uint16_t)buff[5] << 7) & 0x07FF;
    remote_data->remote.ch3 -= 1024;

    //��ֹһ����Ʈ����˵ң��������У׼�����ǻ�����ôдһ�°�
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
    
    //���Ľ���
    remote_data->mouse.x = (uint8_t)buff[6] | ((uint8_t)buff[7] << 8);
    remote_data->mouse.y = (uint8_t)buff[8] | ((uint8_t)buff[9] << 8);
    remote_data->mouse.z = (uint8_t)buff[10] | ((uint8_t)buff[11] << 8);

    remote_data->mouse.left = buff[12];
    remote_data->mouse.right = buff[13];

    //���̵Ľ���
    remote_data->keyboard.key = buff[14] | (buff[15] << 8);
}

/**
* @brief :  ң����ģʽѡ��
* @param :  remote_data
* @retval:  NONE
* @note  :  ң������s2��������ģʽ����������ң�أ���������ֹͣ�����м��Ǽ���
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
