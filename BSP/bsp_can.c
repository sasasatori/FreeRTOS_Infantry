/**
*@file bsp_can.c
*@date 2019/2/17
*@author Satori
*@brief ���Ǹ��ǳ��򵥵�can��bsp��ʵ��
*/

/*�����������������������������������������������������������������á�������������������������������������������������������������*/

#include "bsp_can.h"
#include "GimbalTask.h"
#include "ChassisTask.h"
#include "ShootTask.h"

#include "sys_config.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

/*���������������������������������������������������������������������塪������������������������������������������������������������*/

/*��������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

/**
* @brief :  û��ʲô���õĻص�����
* @param :  hcan
* @retval:  NONE
* @note  :  ������������
*/

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    //�����switch-case����ݲ�ͬ�ĵ��id���н���
    switch (hcan->pRxMsg->StdId)
    {
        case CAN_3508_M1_ID:
        case CAN_3508_M2_ID:
        case CAN_3508_M3_ID:
        case CAN_3508_M4_ID:
        {
            ;
        }break;

        case CAN_6623_PI_ID:
        {
            ;
        }break;

        case CAN_6623_YA_ID:
        {
            ;
        }break;

        case CAN_TRIGGER_ID:
        {
            ;
        }break;

        case SHOOTER_LEF_ID:
        case SHOOTER_RIG_ID:
        {
            ;
        }break;

        default:
        {
            ;
        }break;
    }

    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FMP0);
    __HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FMP0);
}

//����������ҽʽд��������Ա�����ʽд����
void __HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan)
{
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FMP0);
}

/**
* @brief :  ��ʼ��CAN1,CAN2�Ĺ�����
* @param :  NONE
* @retval:  NONE
* @note  :  ûɶ��˵�ģ���д���費��ʼ����
*/

void Can_Device_Init(void)
{
    CAN_FilterConfTypeDef   can_filter;
    static CanTxMsgTypeDef  Tx1Message;
    static CanRxMsgTypeDef  Rx1Message;
    static CanTxMsgTypeDef  Tx2Message;
    static CanRxMsgTypeDef  Rx2Message;

    can_filter.FilterNumber         = 0;
    can_filter.FilterMode           = CAN_FILTERMODE_IDMASK;
    can_filter.FilterMode           = CAN_FILTERSCALE_32BIT;
    can_filter.FilterIdHigh         = 0x0000;
    can_filter.FilterIdLow          = 0x0000;
    can_filter.FilterMaskIdHigh     = 0x0000;
    can_filter.FilterMaskIdLow      = 0x0000;
    can_filter.FilterFIFOAssignment = CAN_FilterFIFO0;
    can_filter.BankNumber           = 14;
    can_filter.FilterActivation     = ENABLE;
    HAL_CAN_ConfigFilter(&hcan1, &can_filter);

    can_filter.FilterNumber         = 14;
    HAL_CAN_ConfigFilter(&hcan2, &can_filter);

    hcan1.pTxMsg = &Tx1Message;
    hcan1.pRxMsg = &Rx1Message;
    hcan2.pTxMsg = &Tx2Message;
    hcan2.pRxMsg = &Rx2Message;
}

/**
* @brief :  ����CAN�����ж�
* @param :  NONE
* @retval:  NONE
* @note  :  ûɶ��˵�ģ�����main�������һ��������ʼ��
*/

void Can_Receive_Start(void)
{
    HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
    HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
}

/**
* @brief :  ���̵��can���� 
* @param :  CM1_Current-CM4_Current 
* @retval:  NONE
* @note  :  �����̵�����͵���
*/
void Send_Chassis_Cur(int16_t CM1_Current,int16_t CM2_Current,int16_t CM3_Current,int16_t CM4_Current)
{
    CHASSIS_CAN.pTxMsg->StdId   = CAN_CHASSIS_ALL_ID;
    CHASSIS_CAN.pTxMsg->IDE     = CAN_ID_STD;
    CHASSIS_CAN.pTxMsg->RTR     = CAN_RTR_DATA;
    CHASSIS_CAN.pTxMsg->DLC     = 0x08;
    CHASSIS_CAN.pTxMsg->Data[0] = CM1_Current >> 8;
    CHASSIS_CAN.pTxMsg->Data[1] = CM1_Current;
    CHASSIS_CAN.pTxMsg->Data[2] = CM2_Current >> 8;
    CHASSIS_CAN.pTxMsg->Data[3] = CM2_Current;
    CHASSIS_CAN.pTxMsg->Data[4] = CM3_Current >> 8;
    CHASSIS_CAN.pTxMsg->Data[5] = CM3_Current;
    CHASSIS_CAN.pTxMsg->Data[6] = CM4_Current >> 8;
    CHASSIS_CAN.pTxMsg->Data[7] = CM4_Current;
    HAL_CAN_Transmit(&CHASSIS_CAN, 10);
};

/**
* @brief :  ��̨&Ħ���ֵ��Can����
* @param :  Yaw_Current,Pitch_Current
* @retval:  NONE
* @note  :  ����̨������͵���
*/
void Send_Gimbal_Cur(int16_t YAW_Current,int16_t PIT_Current,int16_t Left_Current,int16_t Right_Current)
{
    GIMBAL_SHOOTER_CAN.pTxMsg->StdId   = GIMBAL_SHOOTER_ID;
    GIMBAL_SHOOTER_CAN.pTxMsg->IDE     = CAN_ID_STD;
    GIMBAL_SHOOTER_CAN.pTxMsg->RTR     = CAN_RTR_DATA;
    GIMBAL_SHOOTER_CAN.pTxMsg->DLC     = 0x08;
    GIMBAL_SHOOTER_CAN.pTxMsg->Data[0] = YAW_Current >> 8;
    GIMBAL_SHOOTER_CAN.pTxMsg->Data[1] = YAW_Current;
    GIMBAL_SHOOTER_CAN.pTxMsg->Data[2] = PIT_Current >> 8;
    GIMBAL_SHOOTER_CAN.pTxMsg->Data[3] = PIT_Current;
    GIMBAL_SHOOTER_CAN.pTxMsg->Data[4] = Left_Current >> 8;
    GIMBAL_SHOOTER_CAN.pTxMsg->Data[5] = Left_Current;
    GIMBAL_SHOOTER_CAN.pTxMsg->Data[6] = Right_Current >> 8;
    GIMBAL_SHOOTER_CAN.pTxMsg->Data[7] = Right_Current;
    HAL_CAN_Transmit(&GIMBAL_SHOOTER_CAN, 1);
};

/**
* @brief :  �������Can����
* @param :  Trigger_Current
* @retval:  NONE
* @note  :  ������������͵���
*/
void Send_Trigger_Cur(int16_t Trigger_Current)
{
    TRIGGER_CAN.pTxMsg->StdId   = CAN_TRIGGER_ID;
    TRIGGER_CAN.pTxMsg->IDE     = CAN_ID_STD;
    TRIGGER_CAN.pTxMsg->RTR     = CAN_RTR_DATA;
    TRIGGER_CAN.pTxMsg->DLC     = 0x08;
    TRIGGER_CAN.pTxMsg->Data[0] = Trigger_Current >> 8;
    TRIGGER_CAN.pTxMsg->Data[1] = Trigger_Current;
    TRIGGER_CAN.pTxMsg->Data[2] = 0;
    TRIGGER_CAN.pTxMsg->Data[3] = 0;
    TRIGGER_CAN.pTxMsg->Data[4] = 0;
    TRIGGER_CAN.pTxMsg->Data[5] = 0;
    TRIGGER_CAN.pTxMsg->Data[6] = 0;
    TRIGGER_CAN.pTxMsg->Data[7] = 0;
    HAL_CAN_Transmit(&TRIGGER_CAN, 1);
}
