/**
*@file  Motor.h
*@date  2019/2/16
*@author Satori
*@brief 电机有关的东西都放进来
*/

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f4xx_hal.h"

typedef __packed struct
{
    float kp;
    float ki;
    float kd;
    float max;
    float min;
} PID_Parament_t;

typedef __packed struct
{
    PID_Parament_t parament;
    float spd_ref;
    float pos_ref;
    float spd_fdb;
    float pos_fdb;
    float error[2];
    float derror;
    float sum;
    float output;
} PID_t;

//结构体用来存储电机有关的参数
//1.PID参数
//2.解码器获得的数据

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
