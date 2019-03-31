/**
*@file      bsp_uart.h
*@date      2019/2/14
*@author    Satori
*@brief     bsp_can��ͷ�ļ����ϻ���
*/

#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_

/*�����������������������������������������������������������������á�������������������������������������������������������������*/

#include "stm32f4xx_hal.h"

#include "can.h"
#include "Motor.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

typedef enum
{
    //�ⲿ����������ȡ��������ı���
    //������Щ����������CAN1����
    CAN_3508_M1_ID      = 0x201,
    CAN_3508_M2_ID      = 0x202,
    CAN_3508_M3_ID      = 0x203,
    CAN_3508_M4_ID      = 0x204,
    CAN_6623_YA_ID      = 0x205,
    CAN_6623_PI_ID      = 0x206,
    //������Ħ���ֵ����������CAN2����
    CAN_TRIGGER_ID      = 0x201,
    SHOOTER_LEF_ID      = 0x202,
    SHOOTER_RIG_ID      = 0x203,

    //�ⲿ���Ƿ��ͱ���
    //������Щ����CAN1����
    CAN_CHASSIS_ALL_ID  = 0x200,
    CAN_GIMBAL_ID       = 0x1FF,
    //Ħ���ֺͲ��ֹ���CAN2����
    CAN_SHOOTER_ID      = 0x200,

} can_id_e;

/*���������������������������������������������������������������ṹ�塪������������������������������������������������������������*/

/*��������������������������������������������������������������������������������������������������������������������������������*/

extern CanTxMsgTypeDef  Tx1Message;
extern CanRxMsgTypeDef  Rx1Message;
extern CanTxMsgTypeDef  Tx2Message;
extern CanRxMsgTypeDef  Rx2Message;

/*��������������������������������������������������������������������������������������������������������������������������������*/

void Can_Device_Init(CAN_HandleTypeDef* hcan);
void Can_Receive_Start(void);
void Send_Chassis_Cur(int16_t CM1_Current,int16_t CM2_Current,int16_t CM3_Current,int16_t CM4_Current);
void Send_Gimbal_Cur(int16_t YAW_Current,int16_t PIT_Current);
void Send_Trigger_Cur(int16_t Trigger_Current,int16_t Left_Current,int16_t Right_Current);

#endif /*BSP_CAN_H_*/
