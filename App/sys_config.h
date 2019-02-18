/**
*@file      sys_config.h
*@date      2019/2/14
*@author    Satori
*@brief     ¸ø´ó¼ÒÌá¹©¸÷ÖÖºê¶¨Òå
*/

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_    

#include "stm32f4xx_hal.h"

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÒ£¿ØÆ÷ÉèÖÃ¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

#define RC_MODE                 1
#define ST_MODE                 2
#define KM_MODE                 3

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªµ×ÅÌÉèÖÃ¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

//µ×ÅÌµÄ×î´óËÙ¶È,ÆäÊµÎÒ¸ù±¾²»ÏëÒª¸øËÙ¶ÈÉèÏÞ£¬ÒòÎªÎÒÒªdejavu
//Ò£¿ØÆ÷ÉèÖÃµÄ×î´óËÙ¶È
#define CHASSIS_RC_SPD_MAX_X    3300.0f
#define CHASSIS_RC_SPD_MAX_Y    3300.0f
#define CHASSIS_RC_SPD_MAX_R    300.0f

//Ò£¿ØÆ÷ÖµÓ³Éäµ½µ×ÅÌµÄÆÚÍûÖµ
#define CHASSIS_RC_MOVE_RATIO   1.0f

//¼üÅÌÉèÖÃµÄ×î´óËÙ¶È
#define CHASSIS_KB_SPD_MAX_X    3300.0f
#define CHASSIS_KB_SPD_MAX_Y    3300.0f
#define CHASSIS_KB_SPD_MAX_R    300.0f

//¼üÅÌÓ³Éäµ½µ×ÅÌµÄÆÚÍûÖµ
#define CHASSIS_KB_MOVE_RATIO_X 1.0f
#define CHASSIS_KB_MOVE_RATIO_Y 1.0f
#define CHASSIS_KB_MOVE_RATIO_R 1.0f

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÔÆÌ¨ÉèÖÃ¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª·¢Éä»ú¹¹ÉèÖÃ¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªµ×ÅÌ²ÎÊý¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÔÆÌ¨²ÎÊý¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÏµÍ³½»»¥ÉèÖÃ¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/


/*CANÏà¹Ø*/
#define CHASSIS_CAN             hcan1
#define GIMBAL_SHOOTER_CAN      hcan1

#define TRIGGER_CAN             hcan2

/*UARTÏà¹Ø*/
#define DBUS_HUART              huart1
#define JUDGE_HUART             huart2
#define COMPUTER_HUART          huart6

/*OSÏà¹Ø*/
#define GIMBAL_PERIOD           1
#define CHASSIS_PERIOD          10

//ÐÅºÅÁ¿¶¨Òå
#define RC_MODE_SIGNAL          ( 1 << 0 )
#define KM_MODE_SIGNAL          ( 1 << 1 )
#define ST_MODE_SIGNAL          ( 1 << 2 )
#define CHASSIS_SEND_SIGNAL     ( 1 << 3 )
#define GIMBAL_SEND_SIGNAL      ( 1 << 4 )
#define SHOOT_SEND_SIGNAL       ( 1 << 5 )

//¶¨ÒåÈ«¾ÖµÄµç»úµçÁ÷
typedef struct
{
    //ËÄ¸öµ×ÅÌµç»ú£¬Á½¸öÔÆÌ¨µç»ú£¬Á½¸öÄ¦²ÁÂÖµç»ú£¬Ò»¸ö²¦ÂÖµç»ú
    int16_t Chassis_Cur[4];
    int16_t Gimbal_Cur[2];
    int16_t Shooter_Cur[2];
    int16_t Trigger_Cur;
} motor_current_t;

/*ÆäËû*/
#define PI                      3.142f

#endif /*_SYS_CONFIG_H_*/
