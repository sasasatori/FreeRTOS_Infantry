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
    float outputMAX;
    float outputMIN;
} PID_Parament_t;

typedef __packed struct
{
    float ref;
    float fdb;
    float error;
    float sum;
    float output;
    PID_Parament_t parament;
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
        int16_t speed[5];
        int16_t current;
        int8_t  temp;
    }encoder;

} Motor_t;

#endif /*_MOTOR_H_*/