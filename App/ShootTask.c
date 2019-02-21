/**
*@file ShootTask.c
*@date 2019/2/14
*@author Satori
*@brief 符似符似��圻仇芙隠��符tmd
*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！哈喘！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

#include "ShootTask.h"
#include "cmsis_os.h"

#include "sys_config.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

extern osThreadId CanMsg_Send_TaskHandle;

shoot_t shooter;

uint32_t shoot_time_ms;
uint32_t shoot_time_last;

Motor_t Left_Fric_Wheel;
Motor_t Right_Fric_Wheel;

Motor_t Trigger;

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

                //彫価態嚥王態験嚠豚李峙
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
    ;
}

/**
* @brief :  唯峭符似
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Stop_Handler(void)
{
    //唯峭彫価態才王態
    shooter.fric_wheel_run = WHEEL_STOP;
    shooter.trigger.trig_mode = TRIG_STOP;
}
