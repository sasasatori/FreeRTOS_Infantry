/**
*@file ChassisTask.c
*@date 2019/2/14
*@author Satori
*@brief 总之就是执行底盘任务
*/

#include "RemoteMsgTask.h"
#include "ChassisTask.h"
#include "bsp_can.h"
#include "Motor.h"

#include "sys_config.h"
#include "cmsis_os.h"

/*———————————————————————————————宏定义———————————————————————————————*/

#define SWAY_ANGLE      40

/*———————————————————————————————定义变量———————————————————————————————*/

//冷静的掏出我的底盘结构体，并定义了一个结构体变量
chassis_t chassis;

//冷静的掏出Motor结构体，并一口气定义了四个,甚至还顺手搞了一波pid参数配置
//最惨的是这波pid参数的效果还挺难看的，我也很绝望
Motor_t Chassis_Motor_1 = {{{15.0f,0.0f,0.0f}}};
Motor_t Chassis_Motor_2 = {{{15.0f,0.0f,0.0f}}};
Motor_t Chassis_Motor_3 = {{{15.0f,0.0f,0.0f}}};
Motor_t Chassis_Motor_4 = {{{15.0f,0.0f,0.0f}}};

//没有什么意义的CAN发送任务
extern osThreadId CanMsg_Send_TaskHandle;
extern remote_info_t remote_data;

//定义两个时间有关的变量用来监控任务是否正常定时执行
uint32_t chassis_time_last;
uint32_t chassis_time_ms;

//调参用
//float ref,fdb;

/*———————————————————————————————任务函数———————————————————————————————*/

/**
* @brief :  定时执行的底盘任务
* @param :  NONE
* @retval:  NONE
* @note  :  定时执行的任务，所以不用写while(1)，最后会给CanMsg_Send_Task发通知来着
*/

void Chassis_Task(void const * argument)
{
    //对没错，就是看看有没有正常的定时执行而已
    chassis_time_ms = HAL_GetTick() - chassis_time_last;
    chassis_time_last = HAL_GetTick();

    switch (chassis.chassis_mode)
    {
        case CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL:
        {
            Chassis_Remote_Control_Handler();
        }break;

        case CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL:
        case CHASSIS_FREE_KEYMOUSE_CONTROL:
        {
            Chassis_Keymouse_Control_Handler();
        }break;

        case CHASSIS_SWAY:
        {
            Chassis_Sway_Handler();
        }break;

        case CHASSIS_STOP:
        {
            Chassis_Stop_Handler();
        }break;
    
        default:
        {
            Error_Handler();
        }break;
    }

    ChassisRef_to_MotorRef_Handler();

    Chassis_Motor_PIDCalc(&Chassis_Motor_1);
    Chassis_Motor_PIDCalc(&Chassis_Motor_2);
    Chassis_Motor_PIDCalc(&Chassis_Motor_3);
    Chassis_Motor_PIDCalc(&Chassis_Motor_4);

    //用来调参
    // ref = Chassis_Motor_3.pid.spd_ref;
    // fdb = Chassis_Motor_3.pid.spd_fdb;

    osSignalSet(CanMsg_Send_TaskHandle, CHASSIS_SEND_SIGNAL);
}

/*———————————————————————————————其他执行函数———————————————————————————————*/

//遥控模式
void Chassis_Remote_Control_Handler(void)
{
    taskENTER_CRITICAL();
    //第一段用来做遥控器到底盘的解算
    chassis.vy = remote_data.remote.ch0 * CHASSIS_RC_MOVE_RATIO;
    chassis.vx = remote_data.remote.ch1 * CHASSIS_RC_MOVE_RATIO;
    //这边严格意义上来说要用和云台角度之间的PID来进行处理，不过我急着写底盘就先不管这么多了
    chassis.vw = remote_data.remote.ch2 * CHASSIS_RC_MOVE_RATIO;
    taskEXIT_CRITICAL();
    if(chassis.vx >= CHASSIS_STR_MAX)   chassis.vx =  CHASSIS_STR_MAX;
    if(chassis.vx <= -CHASSIS_STR_MAX)  chassis.vx = -CHASSIS_STR_MAX;
    if(chassis.vy >= CHASSIS_STR_MAX)   chassis.vy =  CHASSIS_STR_MAX;
    if(chassis.vy <= -CHASSIS_STR_MAX)  chassis.vy = -CHASSIS_STR_MAX;
    if(chassis.vw >= CHASSIS_ROT_MAX)   chassis.vw =  CHASSIS_ROT_MAX;
    if(chassis.vw <= -CHASSIS_ROT_MAX)  chassis.vw = -CHASSIS_ROT_MAX;
}

/**
* @brief :  键鼠控制
* @param :  NONE
* @retval:  NONE
* @note  :  键鼠模式下也分为跟随和不跟随云台两种模式
*/

void Chassis_Keymouse_Control_Handler(void)
{
    taskENTER_CRITICAL();
    switch (chassis.chassis_mode)
    {
        case CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL:
        {
            ;
        }break;

        case CHASSIS_FREE_KEYMOUSE_CONTROL:
        {
            ;
        }
    }
    taskEXIT_CRITICAL();
}

//晃tm的
void Chassis_Sway_Handler(void)
{
    ;
}

//THE WORLD！時よ！止まれ！
void Chassis_Stop_Handler(void)
{
    taskENTER_CRITICAL();
    //反正停就对了
    chassis.vx = 0;
    chassis.vy = 0;
    chassis.vw = 0;
    taskEXIT_CRITICAL();
}

void ChassisRef_to_MotorRef_Handler(void)
{
    taskENTER_CRITICAL();
    Chassis_Motor_1.pid.spd_ref =   chassis.vx + chassis.vy + chassis.vw;
    Chassis_Motor_2.pid.spd_ref = - chassis.vx + chassis.vy + chassis.vw;
    Chassis_Motor_3.pid.spd_ref = - chassis.vx - chassis.vy + chassis.vw;
    Chassis_Motor_4.pid.spd_ref =   chassis.vx - chassis.vy + chassis.vw;
    taskEXIT_CRITICAL();
}

//将来有一天调参的时候还是会用得上的
//float kp,ki,kd;

void Chassis_Motor_PIDCalc(Motor_t *Motor)
{
    float Error;

    Error = Motor->pid.spd_ref - Motor->pid.spd_fdb;
    Motor->pid.sum_spd += Error;
    
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = Error;

    Motor->pid.output = Motor->pid.spd_parament.kp * Motor->pid.error_spd[0] +
                        Motor->pid.spd_parament.ki * Motor->pid.sum_spd +
                        Motor->pid.spd_parament.kd * Motor->pid.derror_spd;

    //计算pid
    
    //仅调参用
    // Motor->pid.output = kp * Motor->pid.error_spd[0] +
    //                     ki * Motor->pid.sum_spd +
    //                     kd * Motor->pid.derror_spd;

}
