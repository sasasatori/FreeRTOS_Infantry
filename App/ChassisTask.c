/**
*@file ChassisTask.c
*@date 2019/2/14
*@author Satori
*@brief ��֮����ִ�е�������
*/

#include "RemoteMsgTask.h"
#include "ChassisTask.h"
#include "bsp_can.h"
#include "Motor.h"

#include "sys_config.h"
#include "cmsis_os.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

#define SWAY_ANGLE      40

/*�����������������������������������������������������������������������������������������������������������������������������������*/

//�侲���ͳ��ҵĵ��̽ṹ�壬��������һ���ṹ�����
chassis_t chassis;

//�侲���ͳ�Motor�ṹ�壬��һ�����������ĸ�,������˳�ָ���һ��pid��������
//��ҵ����Ⲩpid������Ч����ͦ�ѿ��ģ���Ҳ�ܾ���
Motor_t Chassis_Motor_1 = {{{15.0f,0.0f,0.0f}}};
Motor_t Chassis_Motor_2 = {{{15.0f,0.0f,0.0f}}};
Motor_t Chassis_Motor_3 = {{{15.0f,0.0f,0.0f}}};
Motor_t Chassis_Motor_4 = {{{15.0f,0.0f,0.0f}}};

//û��ʲô�����CAN��������
extern osThreadId CanMsg_Send_TaskHandle;
extern remote_info_t remote_data;

//��������ʱ���йصı���������������Ƿ�������ʱִ��
uint32_t chassis_time_last;
uint32_t chassis_time_ms;

//������
//float ref,fdb;

/*����������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  ��ʱִ�еĵ�������
* @param :  NONE
* @retval:  NONE
* @note  :  ��ʱִ�е��������Բ���дwhile(1)�������CanMsg_Send_Task��֪ͨ����
*/

void Chassis_Task(void const * argument)
{
    //��û�����ǿ�����û�������Ķ�ʱִ�ж���
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

    //��������
    // ref = Chassis_Motor_3.pid.spd_ref;
    // fdb = Chassis_Motor_3.pid.spd_fdb;

    osSignalSet(CanMsg_Send_TaskHandle, CHASSIS_SEND_SIGNAL);
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

//ң��ģʽ
void Chassis_Remote_Control_Handler(void)
{
    taskENTER_CRITICAL();
    //��һ��������ң���������̵Ľ���
    chassis.vy = remote_data.remote.ch0 * CHASSIS_RC_MOVE_RATIO;
    chassis.vx = remote_data.remote.ch1 * CHASSIS_RC_MOVE_RATIO;
    //����ϸ���������˵Ҫ�ú���̨�Ƕ�֮���PID�����д��������Ҽ���д���̾��Ȳ�����ô����
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
* @brief :  �������
* @param :  NONE
* @retval:  NONE
* @note  :  ����ģʽ��Ҳ��Ϊ����Ͳ�������̨����ģʽ
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

//��tm��
void Chassis_Sway_Handler(void)
{
    ;
}

//THE WORLD���r�裡ֹ�ޤ죡
void Chassis_Stop_Handler(void)
{
    taskENTER_CRITICAL();
    //����ͣ�Ͷ���
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

//������һ����ε�ʱ���ǻ��õ��ϵ�
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

    //����pid
    
    //��������
    // Motor->pid.output = kp * Motor->pid.error_spd[0] +
    //                     ki * Motor->pid.sum_spd +
    //                     kd * Motor->pid.derror_spd;

}
