/**
*@file  ChassisTask.h
*@date  2019/2/16
*@author Satori
*@brief ����һ�µ��̵����߰���Ķ���
*/

#ifndef _CHASSIS_H_
#define _CHASSIS_H_

/*�����������������������������������������������������������������á�������������������������������������������������������������*/

#include "stm32f4xx_hal.h"
#include "Motor.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

typedef enum
{
    CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL        = 0,
    CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL      = 1,
    CHASSIS_FREE_KEYMOUSE_CONTROL               = 2,
    CHASSIS_SWAY                                = 3,
    CHASSIS_STOP                                = 4,
} chassis_mode_e;

/*���������������������������������������������������������������ṹ�塪������������������������������������������������������������*/

//�ṹ�������洢���������йصĲ���
//1.����ģʽ
//2.�����ٶ�vx����
//3.�����ٶ�vy����
//4.�����ٶ�vw����

typedef __packed struct
{

    chassis_mode_e      chassis_mode;
    float               vx;   //foward/back
    float               vy;   //left/right
    float               vw;   //clockwise/anticlockwise

} chassis_t;

/*��������������������������������������������������������������������������������������������������������������������������������*/


/*��������������������������������������������������������������������������������������������������������������������������������*/
void Chassis_Task(void const * argument);

#endif /*_CHASSIS_H_*/
