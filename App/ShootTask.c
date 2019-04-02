/**
*@file ShootTask.c
*@date 2019/2/14
*@author Satori
*@brief ��������ԭ���籣����tmd
*/

/*�����������������������������������������������������������������á�������������������������������������������������������������*/

#include "ShootTask.h"
#include "cmsis_os.h"
#include "RemoteMsgTask.h"

#include "sys_config.h"

/*���������������������������������������������������������������궨�塪������������������������������������������������������������*/

#define REMOTE_KEY_UP       1
#define REMOTE_KEY_MID      3
#define REMOTE_KEY_DOWN     2

/*��������������������������������������������������������������������������������������������������������������������������������*/

extern osThreadId CanMsg_Send_TaskHandle;

shoot_t shooter;

uint32_t shoot_time_ms;
uint32_t shoot_time_last;

Motor_t Left_Fric_Wheel;
Motor_t Right_Fric_Wheel;

Motor_t Trigger;

extern remote_info_t remote_data;

/*����������������������������������������������������������������������������������������������������������������������������������*/

void Shoot_TaskStart(void const * argument)
{
    osEvent event;

    while(1)
    {
        event = osSignalWait(SHOOT_SEND_SIGNAL,osWaitForever);

        if (event.status == osEventSignal)
        {
            if (event.value.signals & SHOOT_SEND_SIGNAL)
            {
                shoot_time_ms = HAL_GetTick() - shoot_time_last;
                shoot_time_last = HAL_GetTick();

                if(shooter.ctrl_mode == SHOOT_STOP)
                {
                    Shoot_Stop_Handler();
                }
                else
                {
                    switch (shooter.ctrl_mode)
                    {
                        case SHOOT_AUTO:
                        {
                            Shoot_Auto_Handler();
                        }break;

                        case SHOOT_KEYMOUSE_CONTROL:
                        {
                            Shoot_Keymouse_Handler();
                        }break;

                        case SHOOT_REMOTE_CONTROL:
                        {
                            Shoot_Remote_Handler();
                        }break;

                        default:
                        {
                            Error_Handler();
                        }break;
                    }
                }
                

                //Ħ�����벦��pid����

                //����can����������֪ͨ
                osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
            }
        }

    }
}

/*������������������������������������������������������������������ִ�к�����������������������������������������������������������������*/

/**
* @brief :  �Զ����
* @param :  NONE
* @retval:  NONE
* @note  :  ����Ī��������Զ���������˺ٺٺ�
*/

void Shoot_Auto_Handler(void)
{
    ;
}

/**
* @brief :  �������
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Keymouse_Handler(void)
{
    static uint8_t left_now,left_last;
    static uint8_t right_now,right_last;
    static uint8_t left_counter;
    static uint8_t right_counter;

    left_now = remote_data.mouse.left;
    right_now = remote_data.mouse.right; 

    if(left_now != left_last)
    {
        if(remote_data.mouse.left == PRESS)
        {
            left_counter++;

            if(left_counter % 2 == 0)
            {
                Trigger.pid.output = 0;
            }
            else
            {
                Trigger.pid.output = TRIGGER_SPD_LOW;
            }
        }
    }    

    if(right_now != right_last)
    {
        if(remote_data.mouse.right == PRESS)
        {
            right_counter++;

            if(right_counter % 2 == 0)
            {
                Left_Fric_Wheel.pid.output  = 0;
                Right_Fric_Wheel.pid.output = 0;
            }
            else
            {
                Left_Fric_Wheel.pid.output  = SHOOTER_SPD_LOW;
                Right_Fric_Wheel.pid.output = -SHOOTER_SPD_LOW;
            }
        }
    }

    left_last = left_now;
    right_last = right_now;
}

/**
* @brief :  ң��
* @param :  NONE
* @retval:  NONE
* @note  :  ң����ģʽ�£�Ħ�����ٶȱ��궨���޶���low�����ֲ��õ���������
*/

void Shoot_Remote_Handler(void)
{
    static uint8_t mode_last    = 0;
    static uint8_t mode_now     = 0;

    static uint8_t key_up_counter;
    static uint8_t key_down_counter;

    mode_now = remote_data.remote.s1;

    if(mode_now != mode_last)
    {
        switch(mode_now)
        {
            case REMOTE_KEY_UP:
            {
                key_up_counter++;
                
                if(key_up_counter % 2 == 0)
                {
                    Left_Fric_Wheel.pid.output  = 0;
                    Right_Fric_Wheel.pid.output = 0;
                }
                else
                {
                    Left_Fric_Wheel.pid.output  = SHOOTER_SPD_LOW;
                    Right_Fric_Wheel.pid.output = -SHOOTER_SPD_LOW;
                }
                
            }break;

            case REMOTE_KEY_DOWN:
            {
                key_down_counter++;
                
                if(key_down_counter % 2 == 0)
                {
                    Trigger.pid.output = 0;
                }
                else
                {
                    Trigger.pid.output = TRIGGER_SPD_LOW;
                }
            }break;
        }
    }
    
    mode_last = mode_now;
}

/**
* @brief :  ֹͣ���
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Stop_Handler(void)
{
    Left_Fric_Wheel.pid.output  = 0;
    Right_Fric_Wheel.pid.output = 0;
    Trigger.pid.output          = 0;
}

/**
* @brief :  ����Ħ����pid
* @param :  Motor
* @retval:  none
* @note  :  none
*/

void fric_wheel_pid_calc(Motor_t *Motor)
{
    float error;

    error = Motor->pid.spd_ref - Motor->pid.spd_fdb;
    Motor->pid.sum_spd += error;
    
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = error;

    Motor->pid.output = Motor->pid.spd_parament.kp * Motor->pid.error_spd[0] +
                        Motor->pid.spd_parament.ki * Motor->pid.sum_spd +
                        Motor->pid.spd_parament.kd * Motor->pid.derror_spd;    
}

/**
* @brief :  ������Ħ���ֵ���
* @param :  Motor
* @retval:  none
* @note  :  none
*/
float fric_kp,fric_ki,fric_kd;

void fric_wheel_pid_parament_fix(Motor_t *Motor)
{
    float error;

    error = Motor->pid.spd_ref - Motor->pid.spd_fdb;

    Motor->pid.sum_spd += error;
    
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = error;

    Motor->pid.output = fric_kp * Motor->pid.error_spd[0] +
                        fric_ki * Motor->pid.sum_spd +
                        fric_kd * Motor->pid.derror_spd;    
}

/**
* @brief :  ���㲦��pid
* @param :  Motor
* @retval:  none
* @note  :  noen
*/

void trigger_pid_calc(Motor_t *Motor)
{
    float pos_error,spd_error;

    pos_error = Motor->pid.pos_ref - Motor->pid.pos_fdb;
    Motor->pid.sum_pos += pos_error;
    Motor->pid.derror_pos = Motor->pid.error_pos[0] - Motor->pid.error_pos[1];

    Motor->pid.error_pos[1] = Motor->pid.error_pos[0];
    Motor->pid.error_pos[0] = pos_error;

    Motor->pid.spd_ref = Motor->pid.pos_parament.kp * pos_error + 
                         Motor->pid.pos_parament.ki * Motor->pid.sum_pos +
                         Motor->pid.pos_parament.kd * Motor->pid.derror_pos;


    spd_error = Motor->pid.spd_ref - Motor->pid.spd_fdb;
    Motor->pid.sum_spd += spd_error;
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = spd_error;

    Motor->pid.output = Motor->pid.spd_parament.kp * spd_error + 
                        Motor->pid.spd_parament.ki * Motor->pid.sum_spd + 
                        Motor->pid.spd_parament.kd * Motor->pid.derror_spd;
}

/**
* @brief :  ���ֵ���
* @param :  Motor
* @retval:  none
* @note  :  none
*/

float trigger_pos_kp,trigger_pos_ki,trigger_pos_kd;
float trigger_spd_kp,trigger_spd_ki,trigger_spd_kd;

void trigger_pid_parament_fix(Motor_t *Motor)
{
    float pos_error,spd_error;

    pos_error = Motor->pid.pos_ref - Motor->pid.pos_fdb;
    Motor->pid.sum_pos += pos_error;
    Motor->pid.derror_pos = Motor->pid.error_pos[0] - Motor->pid.error_pos[1];

    Motor->pid.error_pos[1] = Motor->pid.error_pos[0];
    Motor->pid.error_pos[0] = pos_error;

    //������
    Motor->pid.spd_ref = trigger_pos_kp * pos_error + 
                         trigger_pos_ki * Motor->pid.sum_pos +
                         trigger_pos_kd * Motor->pid.derror_pos;

    spd_error = Motor->pid.spd_ref - Motor->pid.spd_fdb;
    Motor->pid.sum_spd += spd_error;
    Motor->pid.derror_spd = Motor->pid.error_spd[0] - Motor->pid.error_spd[1];

    Motor->pid.error_spd[1] = Motor->pid.error_spd[0];
    Motor->pid.error_spd[0] = spd_error;

    
    Motor->pid.output = trigger_spd_kp * spd_error + 
                        trigger_spd_ki * Motor->pid.sum_spd + 
                        trigger_spd_kd * Motor->pid.derror_spd;
}
