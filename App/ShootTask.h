/**
*@file  gimbalTask.h
*@date  2019/2/16
*@author Satori
*@brief ����һ����������߰���Ķ���
*/

#ifndef _SHOOT_H_
#define _SHOOT_H_

/*�����������������������������������������������������������������á�������������������������������������������������������������*/

#include "stm32f4xx_hal.h"
#include "Motor.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

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

/*���������������������������������������������������������������ṹ�塪������������������������������������������������������������*/

//�����뷢������йصĲ������������������壺
//1.���ģʽ
//2.�����λ
//3.�������ʱ��
//4.Ħ���ֿ���/����
//5.Ħ����ת��
//6.������ӵ���
//7.ʣ����ӵ���

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

//�����벦�������йصĲ������������������壺
//���ڲ������Ҫ��˫�����ƣ����Ի���λ��+�ٶȻ�������������
//1.����ģʽ
//2.�ٶȻ�����
//3.λ�û�����
//4.����
//5.�����ٶ�

typedef __packed struct
{
    trig_mode_e         trig_mode;
    int32_t             spd_ref;
    int32_t             pos_ref;
    trigger_dir_e       dir;
    int32_t             feed_bullet_spd;
} trigger_t;

/*��������������������������������������������������������������������������������������������������������������������������������*/


/*��������������������������������������������������������������������������������������������������������������������������������*/

// void Shoot_TaskStart(void const * argument);

#endif /*_SHOOT_H_*/
