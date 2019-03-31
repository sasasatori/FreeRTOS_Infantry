/**
*@file CanMsgTask.c
*@date 2019/2/14
*@author Satori
*@brief ��֮���Ƿ���CAN�źţ������⣿
*/

/*�����������������������������������������������������������������á�������������������������������������������������������������*/

#include "CanMsgTask.h"
#include "bsp_can.h"

#include "sys_config.h"
#include "cmsis_os.h"
#include "Motor.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

/*��������������������������������������������������������������������������������������������������������������������������������*/

uint32_t can_send_time_last;
uint32_t can_send_time_ms;

extern Motor_t Chassis_Motor_1;
extern Motor_t Chassis_Motor_2;
extern Motor_t Chassis_Motor_3;
extern Motor_t Chassis_Motor_4;

extern Motor_t Gimbal_Motor_Pitch;
extern Motor_t Gimbal_Motor_Yaw;

extern Motor_t Left_Fric_Wheel;
extern Motor_t Right_Fric_Wheel;

extern Motor_t Trigger;

/*��������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  can��������
* @param :  NONE
* @retval:  NONE
* @note  :  ��������״̬���ȴ����Ե��̣���̨�����������������ź�
*/

void CanMsg_Send_TaskStart(void const * argument)
{
    osEvent event;

    while(1)
    {
        event = osSignalWait(CHASSIS_SEND_SIGNAL | GIMBAL_SEND_SIGNAL ,osWaitForever);

        can_send_time_ms = HAL_GetTick() - can_send_time_last;
        can_send_time_last = HAL_GetTick();

        if(event.status == osEventSignal)
        {
            if (event.value.signals & CHASSIS_SEND_SIGNAL)
            {
                //can���͵��̵�������
                Send_Chassis_Cur((int16_t)(Chassis_Motor_1.pid.output),\
                                (int16_t)(Chassis_Motor_2.pid.output),\
                                (int16_t)(Chassis_Motor_3.pid.output),\
                                (int16_t)(Chassis_Motor_4.pid.output));
            }

            if (event.value.signals & GIMBAL_SEND_SIGNAL)
            {
                //can������̨��Ħ���֣����̵������
                Send_Gimbal_Cur((int16_t)(Gimbal_Motor_Yaw.pid.output),\
                                (int16_t)(Gimbal_Motor_Pitch.pid.output));
                
                Send_Trigger_Cur((int16_t)Trigger.pid.output,\
                                (int16_t)(Left_Fric_Wheel.pid.output),\
                                (int16_t)(Right_Fric_Wheel.pid.output));
            }
        }
    }
}
