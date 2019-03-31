/**
*@file CanMsgTask.c
*@date 2019/2/14
*@author Satori
*@brief ×ÜÖ®¾ÍÊÇ·¢ËÍCANÐÅºÅ£¬ÓÐÎÊÌâ£¿
*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÒýÓÃ¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

#include "CanMsgTask.h"
#include "bsp_can.h"

#include "sys_config.h"
#include "cmsis_os.h"
#include "Motor.h"

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªºê¶¨Òå¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª±äÁ¿¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

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

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªº¯Êý¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/**
* @brief :  can·¢ËÍÈÎÎñ
* @param :  NONE
* @retval:  NONE
* @note  :  ´¦ÓÚ×èÈû×´Ì¬£¬µÈ´ýÀ´×Ôµ×ÅÌ£¬ÔÆÌ¨£¬·¢ÉäÈÎÎñ·¢ËÍÀ´µÄÐÅºÅ
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
                //can·¢ËÍµ×ÅÌµçÁ÷Êý¾Ý
                Send_Chassis_Cur((int16_t)(Chassis_Motor_1.pid.output),\
                                (int16_t)(Chassis_Motor_2.pid.output),\
                                (int16_t)(Chassis_Motor_3.pid.output),\
                                (int16_t)(Chassis_Motor_4.pid.output));
            }

            if (event.value.signals & GIMBAL_SEND_SIGNAL)
            {
                //can·¢ËÍÔÆÌ¨£¬Ä¦²ÁÂÖ£¬²¦ÅÌµç»úÊý¾Ý
                Send_Gimbal_Cur((int16_t)(Gimbal_Motor_Yaw.pid.output),\
                                (int16_t)(Gimbal_Motor_Pitch.pid.output));
                
                Send_Trigger_Cur((int16_t)Trigger.pid.output,\
                                (int16_t)(Left_Fric_Wheel.pid.output),\
                                (int16_t)(Right_Fric_Wheel.pid.output));
            }
        }
    }
}
