/**
*@file CanMsgTask.c
*@date 2019/2/14
*@author Satori
*@brief 悳岻祥頁窟僕CAN佚催��嗤諒籾��
*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！哈喘！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

#include "CanMsgTask.h"
#include "bsp_can.h"

#include "sys_config.h"
#include "cmsis_os.h"
#include "Motor.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

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

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/**
* @brief :  can窟僕販暦
* @param :  NONE
* @retval:  NONE
* @note  :  侃噐怦毘彜蓑��吉棋栖徭久徒��堝岬��窟符販暦窟僕栖議佚催
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
                //can窟僕久徒窮送方象
                Send_Chassis_Cur((int16_t)(Chassis_Motor_1.pid.output),\
                                (int16_t)(Chassis_Motor_2.pid.output),\
                                (int16_t)(Chassis_Motor_3.pid.output),\
                                (int16_t)(Chassis_Motor_4.pid.output));
            }

            if (event.value.signals & GIMBAL_SEND_SIGNAL)
            {
                //can窟僕堝岬��彫価態��王徒窮字方象
                Send_Gimbal_Cur((int16_t)(Gimbal_Motor_Yaw.pid.output),\
                                (int16_t)(Gimbal_Motor_Pitch.pid.output));
                
                Send_Trigger_Cur((int16_t)Trigger.pid.output,\
                                (int16_t)(Left_Fric_Wheel.pid.output),\
                                (int16_t)(Right_Fric_Wheel.pid.output));
            }
        }
    }
}
