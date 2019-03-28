/**
*@file      sys_config.h
*@date      2019/2/14
*@author    Satori
*@brief     ������ṩ���ֺ궨��
*/

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_    

#include "stm32f4xx_hal.h"

/*��������������������������������������������������������������ң�������á�������������������������������������������������������������*/

#define RC_MODE                 1
#define ST_MODE                 2
#define KM_MODE                 3

/*���������������������������������������������������������������������á�������������������������������������������������������������*/

//���̵�����ٶ�,��ʵ�Ҹ�������Ҫ���ٶ����ޣ���Ϊ��Ҫdejavu
//����ٶ�
#define CHASSIS_SPD_MAX         5000.0f

//ң����ֵӳ�䵽���̵�����ֵ
#define CHASSIS_RC_MOVE_RATIO   15.0f

//����ӳ�䵽���̵�����ֵ
#define CHASSIS_KB_MOVE_RATIO   1.0f

/*����������������������������������������������������������������̨���á�������������������������������������������������������������*/

//��̨�������ٶ�ֵ
#define GIMBAL_SPD_MAX          5000.0f

//�����������ʵ�ʽǶȵ�ӳ��ֵ
#define GIMBAL_REAL_RATIO       22.8f

/*������������������������������������������������������������������������á�������������������������������������������������������������*/

//����Ħ����������λ��ת��
#define SHOOTER_SPD_HIGH        3000.0f
#define SHOOTER_SPD_MID         2000.0f
#define SHOOTER_SPD_LOW         1000.0f

/*�����������������������������������������������������������������̲�����������������������������������������������������������������*/

/*����������������������������������������������������������������̨������������������������������������������������������������������*/

/*��������������������������������������������������������������ϵͳ�������á�������������������������������������������������������������*/


/*CAN���*/
#define CHASSIS_CAN             hcan1
#define GIMBAL_SHOOTER_CAN      hcan1

#define TRIGGER_CAN             hcan2

/*UART���*/
#define DBUS_HUART              huart1
#define JUDGE_HUART             huart2
#define COMPUTER_HUART          huart6

/*OS���*/
#define GIMBAL_PERIOD           1
#define CHASSIS_PERIOD          10
#define IMU_PERIOD              1

//�ź�������
#define RC_MODE_SIGNAL          ( 1 << 0 )
#define KM_MODE_SIGNAL          ( 1 << 1 )
#define ST_MODE_SIGNAL          ( 1 << 2 )
#define CHASSIS_SEND_SIGNAL     ( 1 << 3 )
#define GIMBAL_SEND_SIGNAL      ( 1 << 4 )
#define SHOOT_SEND_SIGNAL       ( 1 << 5 )

//����ȫ�ֵĵ������
typedef struct
{
    //�ĸ����̵����������̨���������Ħ���ֵ����һ�����ֵ��
    int16_t Chassis_Cur[4];
    int16_t Gimbal_Cur[2];
    int16_t Shooter_Cur[2];
    int16_t Trigger_Cur;
} motor_current_t;

/*����*/
#define PI                      3.142f

#endif /*_SYS_CONFIG_H_*/
