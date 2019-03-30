/**
*@file  Motor.h
*@date  2019/2/16
*@author Satori
*@brief ����йصĶ������Ž���
*/

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f4xx_hal.h"

typedef __packed struct
{
    float kp;
    float ki;
    float kd;
} PID_Parament_t;

typedef __packed struct
{
    PID_Parament_t spd_parament;
    PID_Parament_t pos_parament;
    float spd_ref;
    float pos_ref;
    float spd_fdb;
    float pos_fdb;
    float error_spd[2];
    float error_pos[2];
    float derror_spd;
    float derror_pos;
    float sum_spd;
    float sum_pos;
    float output;
} PID_t;

//�ṹ�������洢����йصĲ���
//1.PID����
//2.��������õ�����

typedef __packed struct
{
    PID_t   pid;

    __packed struct
    {
        int16_t angle;
        int16_t speed;
        int16_t current;
        int8_t  temp;
    }encoder;

} Motor_t;

#endif /*_MOTOR_H_*/
