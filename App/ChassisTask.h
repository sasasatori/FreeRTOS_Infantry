/**
*@file  ChassisTask.h
*@date  2019/2/16
*@author Satori
*@brief 協吶匯和久徒議岱鈍伊壯議叫廉
*/

#ifndef _CHASSIS_H_
#define _CHASSIS_H_

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！哈喘！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

#include "stm32f4xx_hal.h"
#include "RemoteMsgTask.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！潤更悶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/



/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

typedef enum
{
    CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL        = 0,
    CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL      = 1,
    CHASSIS_FREE_KEYMOUSE_CONTROL               = 2,
    CHASSIS_SWAY                                = 3,
    CHASSIS_STOP                                = 4,
} chassis_mode;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
void Chassis_Task(void const * argument);

#endif /*_CHASSIS_H_*/
