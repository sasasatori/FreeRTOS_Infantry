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

shoot_t shooter;

uint32_t shoot_time_ms;
uint32_t shoot_time_last;

/*��������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

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
            }
        }

    }
}
