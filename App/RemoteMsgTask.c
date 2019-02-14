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

/*�����������������������������������������������������������������������������������������������������������������������������������*/



/*����������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  ң����ʵ��ģʽ�л�  
* @param :  NONE
* @retval:  NONE
* @note  :  �ӽ����Ǳ߸�һ�����ѣ�Ȼ��ִ��ģʽ�л������ĺúõ�һ��os�Ҿ�����ִ��ģʽ�л�
*/

void RemoteMsg_Receive_ModeSw_TaskStart(void * argument)
{
    while(1)
    {
        ;
    }
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

/**
* @brief :  ң��������
* @param :  
* @retval:  NONE
* @note  :  ��֮���ǽ���
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

    //����֮������ֹһ����Ʈ
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
