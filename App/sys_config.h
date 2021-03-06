/**
*@file      sys_config.h
*@date      2019/2/14
*@author    Satori
*@brief     给大家提供各种宏定义
*/

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_    

#include "stm32f4xx_hal.h"

/*———————————————————————————————遥控器设置———————————————————————————————*/

#define RC_MODE                 1
#define ST_MODE                 2
#define KM_MODE                 3

/*———————————————————————————————底盘设置———————————————————————————————*/

//底盘的最大速度,其实我根本不想要给速度设限，因为我要dejavu
//最大平动速度
#define CHASSIS_STR_MAX         3000.0f

//最大旋转速度
#define CHASSIS_ROT_MAX         2000.0f

//遥控器值映射到底盘的期望值
#define CHASSIS_RC_MOVE_RATIO   15.0f

//键盘映射到底盘的期望值
#define CHASSIS_STR_ACC_STEP    40.0f

#define CHASSIS_KM_ROT_SPD      2000.0f

/*———————————————————————————————云台设置———————————————————————————————*/

//云台电机最大速度值
#define GIMBAL_SPD_MAX          5000.0f

//定义编码器到实际角度的映射值
#define GIMBAL_REAL_RATIO       22.8f

//定义遥控器期望到实际角度的映射值
#define GIMABL_RC_RATIO         0.0001f

//定义pitch轴的角度幅值
#define GIMBAL_PITCH_MAX        40.0f

//定义yaw轴的角度幅值
#define GIMBAL_YAW_MAX          40.0f

//定义鼠标到云台的映射值
#define GIMBAL_MOUSE_RATIO      500.0f

/*———————————————————————————————发射机构设置———————————————————————————————*/

//设置摩擦轮三个挡位的转速
#define SHOOTER_SPD_HIGH        3000.0f

#define SHOOTER_SPD_MID         2000.0f

#define SHOOTER_SPD_LOW         1000.0f

#define TRIGGER_SPD_HIGH        3000.0f

#define TRIGGER_SPD_LOW         300.0f

/*———————————————————————————————IMU参数———————————————————————————————*/

#define IMU_TO_REAL_RATIO       16.384f

#define IMU_PITCH_BIAS          0.0f
#define IMU_YAW_BIAS            4.0f

#define FILTER_LENGTH           5

/*———————————————————————————————系统交互设置———————————————————————————————*/

/*CAN相关*/
#define CHASSIS_CAN             hcan1
#define GIMBAL_CAN              hcan1

#define SHOOTER_CAN             hcan2

/*UART相关*/
#define DBUS_HUART              huart1
#define JUDGE_HUART             huart2
#define COMPUTER_HUART          huart6

/*OS相关*/
#define GIMBAL_PERIOD           1
#define CHASSIS_PERIOD          10
#define IMU_PERIOD              1

//信号量定义
#define RC_MODE_SIGNAL          ( 1 << 0 )
#define KM_MODE_SIGNAL          ( 1 << 1 )
#define ST_MODE_SIGNAL          ( 1 << 2 )
#define CHASSIS_SEND_SIGNAL     ( 1 << 3 )
#define GIMBAL_SEND_SIGNAL      ( 1 << 4 )
#define SHOOT_SEND_SIGNAL       ( 1 << 5 )

//定义全局的电机电流
typedef struct
{
    //四个底盘电机，两个云台电机，两个摩擦轮电机，一个拨轮电机
    int16_t Chassis_Cur[4];
    int16_t Gimbal_Cur[2];
    int16_t Shooter_Cur[2];
    int16_t Trigger_Cur;
} motor_current_t;

//定义按键以及鼠标的释放与按下
typedef enum
{
    RELEASE = 0,
    PRESS   = 1,
} key_state_e;

/*其他*/
#define PI                      3.142f

#endif /*_SYS_CONFIG_H_*/
