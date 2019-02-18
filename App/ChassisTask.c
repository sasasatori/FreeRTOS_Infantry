/**
*@file ChassisTask.c
*@date 2019/2/14
*@author Satori
*@brief ��֮����ִ�е�������
*/

#include "ChassisTask.h"
#include "bsp_can.h"

#include "sys_config.h"
#include "cmsis_os.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

#define SWAY_ANGLE      40

/*�����������������������������������������������������������������������������������������������������������������������������������*/

//�侲���ͳ��ҵĵ��̽ṹ�壬��������һ���ṹ�����
chassis_t chassis;

//û��ʲô�����CAN��������
extern osThreadId CanMsg_Send_TaskHandle;

//��������ʱ���йصı���������������Ƿ�������ʱִ��
uint32_t chassis_time_last;
uint32_t chassis_time_ms;

/*����������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  ��ʱִ�еĵ�������
* @param :  NONE
* @retval:  NONE
* @note  :  ��ʱִ�е��������Բ���дwhile(1)�������CanMsg_Send_Task��֪ͨ����
*/

void Chassis_Task(void const * argument)
{
    //��û�����ǿ�����û�������Ķ�ʱִ�ж���
    chassis_time_ms = HAL_GetTick() - chassis_time_last;
    chassis_time_last = HAL_GetTick();

    switch (chassis.chassis_mode)
    {
        case CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL:
        {
            ;
        }break;

        case CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL:
        {
            ;
        }break;

        case CHASSIS_FREE_KEYMOUSE_CONTROL:
        {
            ;
        }break;

        case CHASSIS_SWAY:
        {
            ;
        }break;

        case CHASSIS_STOP:
        {
            ;
        }break;
    
        default:
        {
            Error_Handler();
        }break;
    }

    osSignalSet(CanMsg_Send_TaskHandle, CHASSIS_SEND_SIGNAL);
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/
