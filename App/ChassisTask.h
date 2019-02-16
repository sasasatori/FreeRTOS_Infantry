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
#include "Motor.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

typedef enum
{
    CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL        = 0,
    CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL      = 1,
    CHASSIS_FREE_KEYMOUSE_CONTROL               = 2,
    CHASSIS_SWAY                                = 3,
    CHASSIS_STOP                                = 4,
} chassis_mode_e;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！潤更悶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

//潤更悶喘栖贋刈化汚久徒嗤購議歌方
//1.久徒庁塀
//2.久徒堀業vx豚李
//3.久徒堀業vy豚李
//4.久徒堀業vw豚李

typedef __packed struct
{

    chassis_mode_e      chassis_mode;
    float               vx;   //foward/back
    float               vy;   //left/right
    float               vw;   //clockwise/anticlockwise

} chassis_t;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/


/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
void Chassis_Task(void const * argument);

#endif /*_CHASSIS_H_*/
