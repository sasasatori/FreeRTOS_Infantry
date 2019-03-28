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
#include "Motor.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

typedef enum
{
    GIMBAL_REMOTE_CONTROL           = 0,
    GIMBAL_KEYMOUSE_CONTROL         = 1,
    GIMBAL_AUTO                     = 2,
    GIMBAL_STOP                     = 3,
} gimbal_mode_e;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！潤更悶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

//匯倖喘栖贋慧堝岬歌方議潤更悶
//1.堝岬庁塀
//2.yaw已叔業豚李
//3.pitch已叔業豚李
//4.yaw已堀業豚李
//5.pitch已堀業豚李
typedef __packed struct
{
    gimbal_mode_e   gimbal_mode;
    float           yaw_angle_ref;
    float           pitch_angle_ref;
    float           yaw_gyro_ref;
    float           pitch_gyro_ref;
} gimbal_t;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

void Gimbal_Task(void const * argument);
void Gimbal_Remote_Control_Handler(void);
void Gimbal_Keymouse_Control_Handler(void);
void Gimbal_Auto_Handler(void);
void Gimbal_Stop_Handler(void);

void gimbal_pid_calc(Motor_t *Motor);
#endif  /*_GIMBAL_H_*/
