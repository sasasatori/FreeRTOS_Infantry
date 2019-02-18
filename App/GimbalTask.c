/**
*@file  gimbalTask.c
*@date  2019/2/14
*@author Satori
*@brief ��֮����ִ����̨������
*/

#include "GimbalTask.h"
#include "bsp_can.h"

#include "sys_config.h"
#include "cmsis_os.h"

/*�����������������������������������������������������������������������������������������������������������������������������������*/

//�ͳ���̨�ṹ��
gimbal_t gimbal;

//CAN����������������
extern osThreadId CanMsg_Send_TaskHandle;
extern osThreadId Shoot_TaskHandle;

//��������ʱ���йصı���������������Ƿ�������ʱִ��
uint32_t gimbal_time_last;
uint32_t gimbal_time_ms;

/*����������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  ��ʱִ�е���̨�����ҿ������ڱȽ϶�
* @param :  NONE
* @retval:  NONE
* @note  :  ��Ϊ�Ƕ�ʱִ�е��������Բ���дwhile(1)�������shoot_task��CanMsg_Send_Task����֪ͨ
*/

void Gimbal_Task(void const * argument)
{
    gimbal_time_ms = HAL_GetTick() - gimbal_time_last;
    gimbal_time_last = HAL_GetTick();



    osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/
