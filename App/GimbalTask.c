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

Motor_t Gimbal_Motor_Yaw;
Motor_t Gimbal_Motor_Pitch;

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

    switch (gimbal.gimbal_mode)
    {
        case GIMBAL_REMOTE_CONTROL:
        {
            Gimbal_Remote_Control_Handler();
        }break;

        case GIMBAL_KEYMOUSE_CONTROL:
        {
            Gimbal_Keymouse_Control_Handler();
        }break;

        case GIMBAL_AUTO:
        {
            Gimbal_Auto_Handler();
        }break;

        case GIMBAL_STOP:
        {
            Gimbal_Stop_Handler();
        }break;

        default:
        {
            Error_Handler();
        }break;
    }
    
    osSignalSet(Shoot_TaskHandle,SHOOT_SEND_SIGNAL);
    osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

/**
* @brief :  ң��������
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Gimbal_Remote_Control_Handler(void)
{
    ;
}

/**
* @brief :  �������
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Gimbal_Keymouse_Control_Handler(void)
{
    ;
}

/**
* @brief :  ��̨�Զ�����
* @param :  NONE
* @retval:  NONE
* @note  :  ��Ҳ��֪����ô�Զ����ƣ�������������
*/

void Gimbal_Auto_Handler(void)
{
    ;
}

/**
* @brief :  ��֮ͣ�Ͷ���
* @param :  NONE
* @retval:  NONE
* @note  :  ûɶ��˵
*/

void Gimbal_Stop_Handler(void)
{
    gimbal.yaw_gyro_ref     = 0;
    gimbal.pitch_gyro_ref   = 0;
}
