/**
*@file  gimbalTask.h
*@date  2019/2/16
*@author Satori
*@brief 協吶匯和堝岬議岱鈍伊壯議叫廉
*/

#ifndef _GIMBAL_H_
#define _GIMBAL_H_

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！哈喘！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

#include "stm32f4xx_hal.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！潤更悶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

typedef enum
{
    GIMBAL_REMOTE_CONTROL           = 0,
    GIMBAL_KEYMOUSE_CONTROL         = 1,
    GIMBAL_AUTO                     = 2,
    GIMBAL_STOP                     = 3,
} gimbal_mode;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

void Gimbal_Task(void const * argument);

#endif  /*_GIMBAL_H_*/
