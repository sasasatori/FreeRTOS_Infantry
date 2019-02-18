/**
*@file      sys_config.h
*@date      2019/2/14
*@author    Satori
*@brief     公寄社戻工光嶽崎協吶
*/

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_    

#include "stm32f4xx_hal.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！劭陣匂譜崔！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

#define RC_MODE                 1
#define ST_MODE                 2
#define KM_MODE                 3

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！久徒譜崔！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！堝岬譜崔！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！窟符字更譜崔！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！久徒歌方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！堝岬歌方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！狼由住札譜崔！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/


/*CAN�犢�*/
#define CHASSIS_CAN             hcan1
#define GIMBAL_SHOOTER_CAN      hcan1

#define TRIGGER_CAN             hcan2

/*UART�犢�*/
#define DBUS_HUART              huart1
#define JUDGE_HUART             huart2
#define COMPUTER_HUART          huart6

/*OS�犢�*/
#define GIMBAL_PERIOD           1
#define CHASSIS_PERIOD          10

//佚催楚協吶
//0bit:劭陣匂陣崙庁塀  1bit:囚報陣崙庁塀 
#define RC_MODE_SIGNAL          ( 1 << 0 )
#define KM_MODE_SIGNAL          ( 1 << 1 )
#define ST_MODE_SIGNAL          ( 1 << 2 )
#define CHASSIS_SEND_SIGNAL     ( 1 << 3 )
#define GIMBAL_SEND_SIGNAL      ( 1 << 4 )

//協吶畠蕉議窮字窮送
typedef struct
{
    //膨倖久徒窮字��曾倖堝岬窮字��曾倖彫価態窮字��匯倖王態窮字
    int16_t Chassis_Cur[4];
    int16_t Gimbal_Cur[2];
    int16_t Shooter_Cur[2];
    int16_t Trigger_Cur;
} motor_current_t;

/*凪麿*/
#define PI                      3.142f

#endif /*_SYS_CONFIG_H_*/
