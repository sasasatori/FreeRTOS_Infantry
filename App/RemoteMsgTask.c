/**
*@file RemoteControl.c
*@date 2019/2/13
*@author Satori
*@brief 接收遥控器并解码
*/

/*———————————————————————————————引用头文件———————————————————————————————*/
#include "bsp_uart.h"
#include "bsp_can.h"

#include "RemoteMsgTask.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "main.h"

#include "ChassisTask.h"
#include "GimbalTask.h"
#include "ShootTask.h"

#include "sys_config.h"

/*———————————————————————————————定义变量———————————————————————————————*/

extern osThreadId RemoteMsg_Receive_ModeSw_TaskHandle;
extern osTimerId  Chassis_Timer_Id;
extern osTimerId  Gimbal_Timer_Id;

extern chassis_t  chassis;
extern gimbal_t   gimbal;
extern shoot_t    shooter;

/*———————————————————————————————任务函数———————————————————————————————*/

/**
* @brief :  遥控器实现模式切换  
* @param :  NONE
* @retval:  NONE
* @note  :  从接收那边给一个唤醒，然后执行模式切换，天哪好好的一个os我就用来执行模式切换，而且最后还是通过改一个全局变量来n
*/

void RemoteMsg_Receive_ModeSw_TaskStart(void * argument)
{
    osEvent Event;
    
    //启动两个软件定时器，从此开始定时执行云台任务和底盘任务
    osTimerStart(Chassis_Timer_Id, CHASSIS_PERIOD);
    osTimerStart(Gimbal_Timer_Id, GIMBAL_PERIOD);

    while(1)
    {
        Event = osSignalWait(RC_MODE_SIGNAL | KM_MODE_SIGNAL | ST_MODE_SIGNAL , osWaitForever);

        //然后就是分一下模式咯
        if(Event.status == osEventSignal)
        {
            if(Event.value.signals & RC_MODE_SIGNAL)
            {
                //进入遥控器控制模式
                Remote_Control_handler();                
            }
            if(Event.value.signals & KM_MODE_SIGNAL)
            {
                //进入键鼠控制模式
                Keymouse_Control_handler();
            }
            if(Event.value.signals & ST_MODE_SIGNAL)
            {
                //进入停止模式
                Stop_handler();
            }
        }
    }
}

/*———————————————————————————————其他执行函数———————————————————————————————*/

/**
* @brief :  遥控器解码
* @param :  
* @retval:  NONE
* @note  :  总之就是解码
*/

void RemoteMsg_decode_handler(remote_info_t *remote_data , uint8_t *buff)
{
    //遥控器的解码
    remote_data->remote.ch0 = ((uint16_t)buff[0] | (uint16_t)buff[1] << 8) & 0x07FF;
    remote_data->remote.ch0 -= 1024;

    remote_data->remote.ch1 = ((uint16_t)buff[1] >> 3 | (uint16_t)buff[2] << 5) & 0x07FF;
    remote_data->remote.ch1 -= 1024;

    remote_data->remote.ch2 = ((uint16_t)buff[2] >> 6 | (uint16_t)buff[3] << 2 | (uint16_t)buff[4] << 10) & 0x07FF;
    remote_data->remote.ch2 -= 1024;
    
    remote_data->remote.ch3 = ((uint16_t)buff[4] >> 1 | (uint16_t)buff[5] << 7) & 0x07FF;
    remote_data->remote.ch3 -= 1024;

    //防止一下零飘，听说遥控器可以校准，但是还是这么写一下吧
    if(remote_data->remote.ch0 <=5 && remote_data->remote.ch0 >= -5)
        remote_data->remote.ch0 = 0;
    if(remote_data->remote.ch1 <=5 && remote_data->remote.ch1 >= -5)
        remote_data->remote.ch1 = 0;
    if(remote_data->remote.ch2 <=5 && remote_data->remote.ch2 >= -5)
        remote_data->remote.ch2 = 0;
    if(remote_data->remote.ch3 <=5 && remote_data->remote.ch3 >= -5)
        remote_data->remote.ch3 = 0;

    remote_data->remote.s1 = (((uint8_t)buff[5] >> 4) & 0x000C) >> 2;
    remote_data->remote.s2 = ((uint8_t)buff[5] >> 4) & 0x0003;
    
    //鼠标的解码
    remote_data->mouse.x = (uint8_t)buff[6] | ((uint8_t)buff[7] << 8);
    remote_data->mouse.y = (uint8_t)buff[8] | ((uint8_t)buff[9] << 8);
    remote_data->mouse.z = (uint8_t)buff[10] | ((uint8_t)buff[11] << 8);

    remote_data->mouse.left = buff[12];
    remote_data->mouse.right = buff[13];

    //键盘的解码
    remote_data->keyboard.key = buff[14] | (buff[15] << 8);
}

/**
* @brief :  遥控器模式选择
* @param :  remote_data
* @retval:  NONE
* @note  :  遥控器的s2用来控制模式，打到上面是遥控，打到下面是停止，打到中间是键鼠
*/

void Remote_Mode_Select(remote_info_t *remote_data)
{
    static uint8_t mode_last = 0;
    uint8_t mode_now;
    mode_now = remote_data->remote.s2;
    
    if(mode_now != mode_last)
    {
        if(mode_now == RC_MODE)
        {
            osSignalSet(RemoteMsg_Receive_ModeSw_TaskHandle,RC_MODE_SIGNAL);
        }
        else if(mode_now == KM_MODE)
        {
            osSignalSet(RemoteMsg_Receive_ModeSw_TaskHandle,KM_MODE_SIGNAL);
        }
        else if(mode_now == ST_MODE)
        {
            osSignalSet(RemoteMsg_Receive_ModeSw_TaskHandle,ST_MODE_SIGNAL);
        }
        else
        {
            Error_Handler();
        }
    }

    mode_last = mode_now;
};

//把模式参数弄成遥控器控制模式
void Remote_Control_handler(void)
{
    //绿灯亮
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
    //给各个执行器设置模式
    chassis.chassis_mode        = CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL;
    gimbal.gimbal_mode          = GIMBAL_REMOTE_CONTROL;
    shooter.ctrl_mode           = SHOOT_REMOTE_CONTROL;
    shooter.shoot_gear          = BULLET_SPD_MID;
    shooter.fric_wheel_run      = WHEEL_STOP;
    shooter.trigger.trig_mode   = TRIG_STOP;
    //具体还是得根据机械他们画的长啥样来确定
    shooter.trigger.dir         = CLOCKWISE;
}

//把模式参数弄成键鼠控制模式，不过键鼠模式是真的烦，下面的模式太多了，所以只能先弄一个默认模式
void Keymouse_Control_handler(void)
{
    //红灯亮
    HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
    //给各个执行器设置默认模式
    chassis.chassis_mode        = CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL;
    gimbal.gimbal_mode          = GIMBAL_KEYMOUSE_CONTROL;
    shooter.ctrl_mode           = SHOOT_KEYMOUSE_CONTROL;
    shooter.shoot_gear          = BULLET_SPD_MID;
    shooter.fric_wheel_run      = WHEEL_STOP;
    shooter.trigger.trig_mode   = TRIG_STOP;
    //反正跟着机械走
    shooter.trigger.dir         = CLOCKWISE;
}

//把模式参数弄成停止模式
void Stop_handler(void)
{
    //红绿灯都亮
    HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
    //给各个执行器设置模式
    chassis.chassis_mode        = CHASSIS_STOP;
    gimbal.gimbal_mode          = GIMBAL_STOP;
    shooter.ctrl_mode           = SHOOT_STOP;
}
