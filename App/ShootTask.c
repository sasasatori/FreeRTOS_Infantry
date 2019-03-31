/**
*@file ShootTask.c
*@date 2019/2/14
*@author Satori
*@brief 符似符似��圻仇芙隠��符tmd
*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！哈喘！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

#include "ShootTask.h"
#include "cmsis_os.h"
#include "RemoteMsgTask.h"

#include "sys_config.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

#define REMOTE_KEY_UP       1
#define REMOTE_KEY_MID      3
#define REMOTE_KEY_DOWN     2

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

extern osThreadId CanMsg_Send_TaskHandle;

shoot_t shooter;

uint32_t shoot_time_ms;
uint32_t shoot_time_last;

Motor_t Left_Fric_Wheel;
Motor_t Right_Fric_Wheel;

Motor_t Trigger;

extern remote_info_t remote_data;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！販暦痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

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

                    case SHOOT_STOP:
                    {
                        Shoot_Stop_Handler();
                    }break;

                    default:
                    {
                        Error_Handler();
                    }break;
                }

                //彫価態嚥王態pid柴麻

                //恷朔公can窟僕販暦窟僕宥岑
                osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
            }
        }

    }
}

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！凪麿峇佩痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/**
* @brief :  徭強符似
* @param :  NONE
* @retval:  NONE
* @note  :  郡屎暢兆凪虫祥徭強符似軟栖阻細細細
*/

void Shoot_Auto_Handler(void)
{
    ;
}

/**
* @brief :  囚報陣崙
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Keymouse_Handler(void)
{
    ;
}

/**
* @brief :  劭陣
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Remote_Handler(void)
{
    static uint8_t mode_last = 0;
    static uint8_t mode_now = 0;

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
* @brief :  唯峭符似
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
* @brief :  柴麻彫価態pid
* @param :  Motor
* @retval:  none
* @note  :  none
*/

void fric_wheel_pid_calc(Motor_t *Motor)
{
    ;
}

/**
* @brief :  喘栖距歌
* @param :  Motor
* @retval:  none
* @note  :  none
*/

void fric_wheel_pid_parament_fix(Motor_t *Motor)
{
    ;
}
