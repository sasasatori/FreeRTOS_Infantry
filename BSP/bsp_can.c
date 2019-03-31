/**
*@file bsp_can.c
*@date 2019/2/17
*@author Satori
*@brief 这是个非常简单的can的bsp层实现
*/

/*———————————————————————————————引用———————————————————————————————*/

#include "bsp_can.h"
#include "GimbalTask.h"
#include "ChassisTask.h"
#include "ShootTask.h"

#include "sys_config.h"
#include "Motor.h"

/*———————————————————————————————宏定义———————————————————————————————*/

/*———————————————————————————————变量定义———————————————————————————————*/

//总之就是一堆电机

extern Motor_t Chassis_Motor_1;
extern Motor_t Chassis_Motor_2;
extern Motor_t Chassis_Motor_3;
extern Motor_t Chassis_Motor_4;

extern Motor_t Gimbal_Motor_Pitch;
extern Motor_t Gimbal_Motor_Yaw;

extern Motor_t Left_Fric_Wheel;
extern Motor_t Right_Fric_Wheel;

extern Motor_t Trigger;

//检测时间
uint32_t canmsg_rx_time_last;
uint32_t canmsg_rx_time_ms;

/*———————————————————————————————执行函数———————————————————————————————*/

/**
* @brief :  没有什么鸟用的回调函数
* @param :  hcan
* @retval:  NONE
* @note  :  反正就是这样
*/

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan)
{

    canmsg_rx_time_ms = HAL_GetTick() - canmsg_rx_time_last;
    canmsg_rx_time_last = HAL_GetTick();

    if (hcan->Instance == CAN1)
    {
        switch (hcan->pRxMsg->StdId)
        {
            case CAN_3508_M1_ID:
            {
                Chassis_Motor_1.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Chassis_Motor_1.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                Chassis_Motor_1.pid.spd_fdb   = (float)Chassis_Motor_1.encoder.speed;
            }break;

            case CAN_3508_M2_ID:
            {
                Chassis_Motor_2.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Chassis_Motor_2.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                Chassis_Motor_2.pid.spd_fdb   = (float)Chassis_Motor_2.encoder.speed;
            }break;

            case CAN_3508_M3_ID:
            {
                Chassis_Motor_3.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Chassis_Motor_3.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                Chassis_Motor_3.pid.spd_fdb   = (float)Chassis_Motor_3.encoder.speed;
            }break;

            case CAN_3508_M4_ID:
            {
                Chassis_Motor_4.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Chassis_Motor_4.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                Chassis_Motor_4.pid.spd_fdb   = (float)Chassis_Motor_4.encoder.speed;
            }break;

            case CAN_6623_PI_ID:
            {
                Gimbal_Motor_Pitch.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Gimbal_Motor_Pitch.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                //最后yaw轴和pitch轴的角度数据都可怜兮兮的要从编码器获取了
                Gimbal_Motor_Pitch.pid.pos_fdb = ((float)Gimbal_Motor_Pitch.encoder.angle - 5850.0f) / GIMBAL_REAL_RATIO;
            }break;

            case CAN_6623_YA_ID:
            {
                Gimbal_Motor_Yaw.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Gimbal_Motor_Yaw.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                Gimbal_Motor_Yaw.pid.pos_fdb = ((float)Gimbal_Motor_Yaw.encoder.angle - 6183.0f) / GIMBAL_REAL_RATIO;

            }break;

            default:
            {
                ;
            }break;
        }
    //下面的switch-case会根据不同的电机id进行解码        
    }

    if (hcan->Instance == CAN2)
    {
        switch (hcan->pRxMsg->StdId)
        {
            case CAN_TRIGGER_ID:
            {
                Trigger.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Trigger.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                Trigger.pid.pos_fdb = (float)Trigger.encoder.angle;
                Trigger.pid.spd_fdb = (float)Trigger.encoder.speed;
            }break;

            case SHOOTER_LEF_ID:
            {
                Left_Fric_Wheel.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Left_Fric_Wheel.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                Left_Fric_Wheel.pid.pos_fdb = (float)Left_Fric_Wheel.encoder.angle;
                Left_Fric_Wheel.pid.spd_fdb = (float)Left_Fric_Wheel.encoder.speed;
            }break;

            case SHOOTER_RIG_ID:
            {
                Right_Fric_Wheel.encoder.angle = hcan->pRxMsg->Data[0] << 8 | hcan->pRxMsg->Data[1];
                Right_Fric_Wheel.encoder.speed = hcan->pRxMsg->Data[2] << 8 | hcan->pRxMsg->Data[3];
                Right_Fric_Wheel.pid.pos_fdb = (float)Right_Fric_Wheel.encoder.angle;
                Right_Fric_Wheel.pid.spd_fdb = (float)Right_Fric_Wheel.encoder.speed;
            }break;

            default:
            {
                ;
            }break;
        }
    }

    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FMP0);
    __HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FMP0);
}

//死马当做活马医式写法（简称自暴自弃式写法）
void __HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan)
{
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FMP0);
}

/**
* @brief :  初始化CAN1,CAN2的过滤器
* @param :  NONE
* @retval:  NONE
* @note  :  没啥好说的，你写外设不初始化吗？
*/

void Can_Device_Init(CAN_HandleTypeDef* hcan)
{
  CAN_FilterConfTypeDef canfilter;
  
  static CanTxMsgTypeDef  Tx1Message;
  static CanRxMsgTypeDef  Rx1Message;
  static CanTxMsgTypeDef  Tx2Message;
  static CanRxMsgTypeDef  Rx2Message;
  
  canfilter.FilterMode              = CAN_FILTERMODE_IDMASK;
  canfilter.FilterScale             = CAN_FILTERSCALE_32BIT;
  
  canfilter.FilterIdHigh            = 0x0000;
  canfilter.FilterIdLow             = 0x0000;
  canfilter.FilterMaskIdHigh        = 0x0000;
  canfilter.FilterMaskIdLow         = 0x0000;
  
  canfilter.FilterFIFOAssignment    = CAN_FilterFIFO0;
  canfilter.FilterActivation        = ENABLE;
  canfilter.BankNumber              = 14;
  
  if(hcan == &hcan1)
  {
    canfilter.FilterNumber = 0;
    hcan->pTxMsg = &Tx1Message;
    hcan->pRxMsg = &Rx1Message;
  }
  if(hcan == &hcan2)
  {
    canfilter.FilterNumber = 14;
    hcan->pTxMsg = &Tx2Message;
    hcan->pRxMsg = &Rx2Message;
  }
  
  HAL_CAN_ConfigFilter(hcan, &canfilter);
  
}

/**
* @brief :  开启CAN接收中断
* @param :  NONE
* @retval:  NONE
* @note  :  没啥好说的，就在main里面调用一下用来初始化
*/

void Can_Receive_Start(void)
{
    HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
    HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
}

/**
* @brief :  底盘电机can发送 
* @param :  CM1_Current-CM4_Current 
* @retval:  NONE
* @note  :  给底盘电机发送电流
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
* @brief :  云台电机Can发送
* @param :  Yaw_Current,Pitch_Current
* @retval:  NONE
* @note  :  给云台电机发送电流
*/
void Send_Gimbal_Cur(int16_t YAW_Current,int16_t PIT_Current)
{
    GIMBAL_CAN.pTxMsg->StdId   = CAN_GIMBAL_ID;
    GIMBAL_CAN.pTxMsg->IDE     = CAN_ID_STD;
    GIMBAL_CAN.pTxMsg->RTR     = CAN_RTR_DATA;
    GIMBAL_CAN.pTxMsg->DLC     = 0x08;
    GIMBAL_CAN.pTxMsg->Data[0] = YAW_Current >> 8;
    GIMBAL_CAN.pTxMsg->Data[1] = YAW_Current;
    GIMBAL_CAN.pTxMsg->Data[2] = PIT_Current >> 8;
    GIMBAL_CAN.pTxMsg->Data[3] = PIT_Current;
    GIMBAL_CAN.pTxMsg->Data[4] = 0;
    GIMBAL_CAN.pTxMsg->Data[5] = 0;
    GIMBAL_CAN.pTxMsg->Data[6] = 0;
    GIMBAL_CAN.pTxMsg->Data[7] = 0;
    HAL_CAN_Transmit(&GIMBAL_CAN, 1);
};

/**
* @brief :  拨弹电机Can发送
* @param :  Trigger_Current
* @retval:  NONE
* @note  :  给拨弹电机发送电流
*/
void Send_Trigger_Cur(int16_t Trigger_Current,int16_t Left_Current,int16_t Right_Current)
{
    SHOOTER_CAN.pTxMsg->StdId   = CAN_SHOOTER_ID;
    SHOOTER_CAN.pTxMsg->IDE     = CAN_ID_STD;
    SHOOTER_CAN.pTxMsg->RTR     = CAN_RTR_DATA;
    SHOOTER_CAN.pTxMsg->DLC     = 0x08;
    SHOOTER_CAN.pTxMsg->Data[0] = Trigger_Current >> 8;
    SHOOTER_CAN.pTxMsg->Data[1] = Trigger_Current;
    SHOOTER_CAN.pTxMsg->Data[2] = Left_Current >> 8;
    SHOOTER_CAN.pTxMsg->Data[3] = Left_Current;
    SHOOTER_CAN.pTxMsg->Data[4] = Right_Current >> 8;
    SHOOTER_CAN.pTxMsg->Data[5] = Right_Current;
    SHOOTER_CAN.pTxMsg->Data[6] = 0;
    SHOOTER_CAN.pTxMsg->Data[7] = 0;
    HAL_CAN_Transmit(&SHOOTER_CAN, 1);
}
