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
//ң�������õ�����ٶ�
#define CHASSIS_RC_SPD_MAX_X    3300.0f
#define CHASSIS_RC_SPD_MAX_Y    3300.0f
#define CHASSIS_RC_SPD_MAX_R    300.0f

//ң����ֵӳ�䵽���̵�����ֵ
#define CHASSIS_RC_MOVE_RATIO   1.0f

//�������õ�����ٶ�
#define CHASSIS_KB_SPD_MAX_X    3300.0f
#define CHASSIS_KB_SPD_MAX_Y    3300.0f
#define CHASSIS_KB_SPD_MAX_R    300.0f

//����ӳ�䵽���̵�����ֵ
#define CHASSIS_KB_MOVE_RATIO_X 1.0f
#define CHASSIS_KB_MOVE_RATIO_Y 1.0f
#define CHASSIS_KB_MOVE_RATIO_R 1.0f

/*����������������������������������������������������������������̨���á�������������������������������������������������������������*/

/*������������������������������������������������������������������������á�������������������������������������������������������������*/

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
