/**
*@file  gimbalTask.h
*@date  2019/2/16
*@author Satori
*@brief 定义一下射击的乱七八糟的东西
*/

#ifndef _SHOOT_H_
#define _SHOOT_H_

/*———————————————————————————————引用———————————————————————————————*/

#include "stm32f4xx_hal.h"
#include "Motor.h"

/*———————————————————————————————宏定义———————————————————————————————*/

typedef enum
{
    SHOOT_AUTO                      = 0,
    SHOOT_REMOTE_CONTROL            = 1,
    SHOOT_KEYMOUSE_CONTROL          = 2,
    SHOOT_STOP                      = 3,
} shoot_mode_e;

typedef enum
{
    BULLET_SPD_LOW                  = 0,
    BULLET_SPD_MID                  = 1,
    BULLET_SPD_HIG                  = 2,
} bullet_spd_e;

typedef enum
{
    TRIG_ONCE                       = 0,
    TRIG_CONT                       = 1,
    TRIG_STOP                       = 2,
    TRIG_BLK                        = 3,
} trig_mode_e;

typedef enum
{
    WHEEL_STOP                      = 0,
    WHEEL_RUN                       = 1,
} wheel_status_e;

typedef enum
{
    CLOCKWISE                       = 0,
    ANTI_CLOCKWISE                  = 1,
} trigger_dir_e;

/*———————————————————————————————结构体———————————————————————————————*/

//所有与发射机构有关的参数会在这个里面给定义：
//1.射击模式
//2.射击挡位
//3.持续射击时间
//4.摩擦轮开启/不开
//5.摩擦轮转速
//6.射出的子弹数
//7.剩余的子弹数

typedef __packed struct
{
    shoot_mode_e       clrl_mode;
    bullet_spd_e       shoot_gear;
    uint32_t           continue_shoot_time;
    wheel_status_e     fric_wheel_run;
    uint16_t           fric_wheel_spd;
    uint16_t           shoot_bullets;
    uint16_t           remain_bullets;
} shoot_t;

//所有与拨弹机构有关的参数会在这个里面给定义：
//由于拨弹电机要做双环控制，所以会有位置+速度环的期望在里面
//1.拨弹模式
//2.速度环期望
//3.位置环期望
//4.方向
//5.供弹速度

typedef __packed struct
{
    trig_mode_e         trig_mode;
    int32_t             spd_ref;
    int32_t             pos_ref;
    trigger_dir_e       dir;
    int32_t             feed_bullet_spd;
} trigger_t;

/*———————————————————————————————变量———————————————————————————————*/


/*———————————————————————————————函数———————————————————————————————*/

// void Shoot_TaskStart(void const * argument);

#endif /*_SHOOT_H_*/
