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

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

uint32_t can_send_time_last;
uint32_t can_send_time_ms;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/**
* @brief :  can窟僕販暦
* @param :  NONE
* @retval:  NONE
* @note  :  侃噐怦毘彜蓑��吉棋栖徭久徒才堝岬販暦窟僕栖議佚催
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
                //Send_Chassis_Cur(10000,10000,10000,10000);;
            }

            if (event.value.signals & GIMBAL_SEND_SIGNAL)
            {
                ;
            }
        }
    }
}
