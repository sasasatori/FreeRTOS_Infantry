/**
*@file ShootTask.c
*@date 2019/2/14
*@author Satori
*@brief ��������ԭ���籣����tmd
*/

/*�����������������������������������������������������������������á�������������������������������������������������������������*/

#include "ShootTask.h"
#include "cmsis_os.h"

#include "sys_config.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

/*��������������������������������������������������������������������������������������������������������������������������������*/

extern osThreadId CanMsg_Send_TaskHandle;

shoot_t shooter;

uint32_t shoot_time_ms;
uint32_t shoot_time_last;

Motor_t Left_Fric_Wheel;
Motor_t Right_Fric_Wheel;

Motor_t Trigger;

/*����������������������������������������������������������������������������������������������������������������������������������*/

void Shoot_TaskStart(void const * argument)
{
    osEvent event;

    while(1)
    {
        event = osSignalWait(SHOOT_SEND_SIGNAL,osWaitForever);

        if (event.status == osEventSignal)
        {
            if (event.value.signals & SHOOT_SEND_SIGNAL)
            {
                shoot_time_ms = HAL_GetTick() - shoot_time_last;
                shoot_time_last = HAL_GetTick();

                switch (shooter.ctrl_mode)
                {
                    case SHOOT_AUTO:
                    {
                        Shoot_Auto_Handler();
                    }break;

                    case SHOOT_KEYMOUSE_CONTROL:
                    {
                        Shoot_Keymouse_Handler();
                    }break;

                    case SHOOT_REMOTE_CONTROL:
                    {
                        Shoot_Remote_Handler();
                    }break;

                    case SHOOT_STOP:
                    {
                        Shoot_Stop_Handler();
                    }break;

                    default:
                    {
                        Error_Handler();
                    }break;
                }

                //Ħ�����벦�ָ�������ֵ
                //Ħ�����벦��pid����

                //����can����������֪ͨ
                osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
            }
        }

    }
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

/**
* @brief :  �Զ����
* @param :  NONE
* @retval:  NONE
* @note  :  ����Ī��������Զ���������˺ٺٺ�
*/

void Shoot_Auto_Handler(void)
{
    ;
}

/**
* @brief :  �������
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Keymouse_Handler(void)
{
    ;
}

/**
* @brief :  ң��
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Remote_Handler(void)
{
    ;
}

/**
* @brief :  ֹͣ���
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Stop_Handler(void)
{
    //ֹͣĦ���ֺͲ���
    shooter.fric_wheel_run = WHEEL_STOP;
    shooter.trigger.trig_mode = TRIG_STOP;
}
