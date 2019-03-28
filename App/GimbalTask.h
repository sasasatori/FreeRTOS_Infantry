/**
*@file  gimbalTask.h
*@date  2019/2/16
*@author Satori
*@brief ����һ����̨�����߰���Ķ���
*/

#ifndef _GIMBAL_H_
#define _GIMBAL_H_

/*�����������������������������������������������������������������á�������������������������������������������������������������*/

#include "stm32f4xx_hal.h"
#include "Motor.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

typedef enum
{
    GIMBAL_REMOTE_CONTROL           = 0,
    GIMBAL_KEYMOUSE_CONTROL         = 1,
    GIMBAL_AUTO                     = 2,
    GIMBAL_STOP                     = 3,
} gimbal_mode_e;

/*���������������������������������������������������������������ṹ�塪������������������������������������������������������������*/

//һ�����������̨�����Ľṹ��
//1.��̨ģʽ
//2.yaw��Ƕ�����
//3.pitch��Ƕ�����
//4.yaw���ٶ�����
//5.pitch���ٶ�����
typedef __packed struct
{
    gimbal_mode_e   gimbal_mode;
    float           yaw_angle_ref;
    float           pitch_angle_ref;
    float           yaw_gyro_ref;
    float           pitch_gyro_ref;
} gimbal_t;

/*��������������������������������������������������������������������������������������������������������������������������������*/

/*��������������������������������������������������������������������������������������������������������������������������������*/

void Gimbal_Task(void const * argument);
void Gimbal_Remote_Control_Handler(void);
void Gimbal_Keymouse_Control_Handler(void);
void Gimbal_Auto_Handler(void);
void Gimbal_Stop_Handler(void);

void gimbal_pid_calc(Motor_t *Motor);
#endif  /*_GIMBAL_H_*/
