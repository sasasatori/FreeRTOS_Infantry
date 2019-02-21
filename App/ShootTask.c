/**
*@file ShootTask.c
*@date 2019/2/14
*@author Satori
*@brief Éä»÷Éä»÷£¬Ô­µØÉç±££¬Éätmd
*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÒýÓÃ¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

#include "ShootTask.h"
#include "cmsis_os.h"

#include "sys_config.h"

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªºê¶¨Òå¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª±äÁ¿¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

extern osThreadId CanMsg_Send_TaskHandle;

shoot_t shooter;

uint32_t shoot_time_ms;
uint32_t shoot_time_last;

Motor_t Left_Fric_Wheel;
Motor_t Right_Fric_Wheel;

Motor_t Trigger;

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÈÎÎñº¯Êý¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

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

                //Ä¦²ÁÂÖÓë²¦ÂÖ¸³ÓèÆÚÍûÖµ
                //Ä¦²ÁÂÖÓë²¦ÂÖpid¼ÆËã

                //×îºó¸øcan·¢ËÍÈÎÎñ·¢ËÍÍ¨Öª
                osSignalSet(CanMsg_Send_TaskHandle,GIMBAL_SEND_SIGNAL);
            }
        }

    }
}

/*¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ªÆäËûÖ´ÐÐº¯Êý¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/

/**
* @brief :  ×Ô¶¯Éä»÷
* @param :  NONE
* @retval:  NONE
* @note  :  ·´ÕýÄªÃûÆäÃî¾Í×Ô¶¯Éä»÷ÆðÀ´ÁËºÙºÙºÙ
*/

void Shoot_Auto_Handler(void)
{
    ;
}

/**
* @brief :  ¼üÊó¿ØÖÆ
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Keymouse_Handler(void)
{
    ;
}

/**
* @brief :  Ò£¿Ø
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Remote_Handler(void)
{
    ;
}

/**
* @brief :  Í£Ö¹Éä»÷
* @param :  NONE
* @retval:  NONE
* @note  :  NONE
*/

void Shoot_Stop_Handler(void)
{
    //Í£Ö¹Ä¦²ÁÂÖºÍ²¦ÂÖ
    shooter.fric_wheel_run = WHEEL_STOP;
    shooter.trigger.trig_mode = TRIG_STOP;
}
