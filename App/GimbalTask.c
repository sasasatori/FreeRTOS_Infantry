/**
*@file  gimbalTask.c
*@date  2019/2/14
*@author Satori
*@brief ��֮����ִ����̨������
*/

#include "GimbalTask.h"
#include "bsp_can.h"
#include "RemoteMsgTask.h"
#include "sys_config.h"
#include "cmsis_os.h"

/*�����������������������������������������������������������������������������������������������������������������������������������*/

//�ͳ���̨�ṹ��
gimbal_t gimbal;

//ǰ��һ���������������ٶȻ���pid����������һ����λ�û���pid����
Motor_t Gimbal_Motor_Yaw    = {{{-3.0f,0.0f,0.0f},{-90.0f,0.0f,0.0f}}};
Motor_t Gimbal_Motor_Pitch  = {{{3.0f,0.0f,0.0f},{60.0f,0.0008f,0.0f}}};

//CAN����������������
extern osThreadId CanMsg_Send_TaskHandle;
extern osThreadId Shoot_TaskHandle;

extern remote_info_t remote_data;

//��������ʱ���йصı���������������Ƿ�������ʱִ��
uint32_t gimbal_time_last;
uint32_t gimbal_time_ms;

/*����������������������������������������������������������������������������������������������������������������������������������*/

/**
* @brief :  ��ʱִ�е���̨�����ҿ������ڱȽ϶�
* @param :  NONE
* @retval:  NONE
* @note  :  ��Ϊ�Ƕ�ʱִ�е��������Բ���дwhile(1)�������shoot_task��CanMsg_Send_Task����֪ͨ
*/

void Gimbal_Task(void const * argument)
{
    gimbal_time_ms = HAL_GetTick() - gimbal_time_last;
    gimbal_time_last = HAL_GetTick();

    if((gimbal.gimbal_mode != GIMBAL_REMOTE_CONTROL) &&
       (gimbal.gimbal_mode != GIMBAL_KEYMOUSE_CONTROL) &&
       (gimbal.gimbal_mode != GIMBAL_AUTO))
    {
        Gimbal_Stop_Handler();
    }
    else
    {
        switch (gimbal.gimbal_mode)
        {
            case GIMBAL_REMOTE_CONTROL:
            {
                Gimbal_Remote_Control_Handler();
            }break;

            case GIMBAL_KEYMOUSE_CONTROL:
            {
                Gimbal_Keymouse_Control_Handler();
            }break;

            case GIMBAL_AUTO:
            {
                Gimbal_Auto_Handler();
            }break;

            default:
            {
                Error_Handler();
            }break;
        }
        
        gimbal_pid_calc(&Gimbal_Motor_Yaw);
        gimbal_pid_calc(&Gimbal_Motor_Pitch);
    }
    
    osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

/**
* @brief :  ң��������
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Gimbal_Remote_Control_Handler(void)
{
    ;
}

/**
* @brief :  �������
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Gimbal_Keymouse_Control_Handler(void)
{
    ;
}

/**
* @brief :  ��̨�Զ�����
* @param :  NONE
* @retval:  NONE
* @note  :  ��Ҳ��֪����ô�Զ����ƣ�������������
*/

void Gimbal_Auto_Handler(void)
{
    ;
}

/**
* @brief :  ��֮ͣ�Ͷ���
* @param :  NONE
* @retval:  NONE
* @note  :  ûɶ��˵
*/

void Gimbal_Stop_Handler(void)
{
    Gimbal_Motor_Pitch.pid.output   = 0;
    Gimbal_Motor_Yaw.pid.output     = 0;
}

/**
* @brief :  ˫��pid����
* @param :  Motor
* @retval:  none
* @note  :  �ҵ�pid���޵е�
*/

void gimbal_pid_calc(Motor_t *Motor)
{
    float pos_error,spd_error;

    pos_error = Motor->pid.pos_ref - Motor->pid.pos_fdb;
    Motor->pid.sum_pos += pos_error;
    Motor->pid.derror_pos = Motor->pid.error_pos[0] - Motor->pid.error_pos[1];

    Motor->pid.error_pos[1] = Motor->pid.error_pos[0];
    Motor->pid.error_pos[0] = pos_error;

    Motor->pid.spd_ref = -(Motor->pid.pos_parament.kp * pos_error + 
                           Motor->pid.pos_parament.ki * Motor->pid.sum_pos +
                           Motor->pid.pos_parament.kd * Motor->pid.derror_pos);


    spd_error = Motor->pid.spd_ref - Motor->pid.spd_fdb;
    Motor->pid.sum_spd += spd_error;
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = spd_error;

    Motor->pid.output = Motor->pid.spd_parament.kp * spd_error + 
                        Motor->pid.spd_parament.ki * Motor->pid.sum_spd + 
                        Motor->pid.spd_parament.kd * Motor->pid.derror_spd;
        
    //������pid���Ӹ��޷�
    if(Motor->pid.output >= GIMBAL_SPD_MAX)
    Motor->pid.output = GIMBAL_SPD_MAX;
    if(Motor->pid.output <= -GIMBAL_SPD_MAX)
    Motor->pid.output = -GIMBAL_SPD_MAX;

}

/**
* @brief : ������������ 
* @param :  *Motor
* @retval:  none
* @note  :  ������������
*/

float pos_kp,pos_ki,pos_kd;
float spd_kp,spd_ki,spd_kd;

void gimbal_pid_parament_fix(Motor_t *Motor)
{
    float pos_error,spd_error;

    pos_error = Motor->pid.pos_ref - Motor->pid.pos_fdb;
    Motor->pid.sum_pos += pos_error;
    Motor->pid.derror_pos = Motor->pid.error_pos[0] - Motor->pid.error_pos[1];

    Motor->pid.error_pos[1] = Motor->pid.error_pos[0];
    Motor->pid.error_pos[0] = pos_error;

    //������
    Motor->pid.spd_ref = -(pos_kp * pos_error + 
                           pos_ki * Motor->pid.sum_pos +
                           pos_kd * Motor->pid.derror_pos);

    spd_error = Motor->pid.spd_ref - Motor->pid.spd_fdb;
    Motor->pid.sum_spd += spd_error;
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = spd_error;

    
    Motor->pid.output = spd_kp * spd_error + 
                        spd_ki * Motor->pid.sum_spd + 
                        spd_kd * Motor->pid.derror_spd;
    
    //������pid���Ӹ��޷�
    if(Motor->pid.output >= GIMBAL_SPD_MAX)
    Motor->pid.output = GIMBAL_SPD_MAX;
    if(Motor->pid.output <= -GIMBAL_SPD_MAX)
    Motor->pid.output = -GIMBAL_SPD_MAX;
}
