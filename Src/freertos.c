/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "RemoteMsgTask.h"
#include "ChassisTask.h"
#include "GimbalTask.h"
#include "CanMsgTask.h"
#include "ShootTask.h"
#include "IMU_Task.h"

#include "sys_config.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId  RemoteMsg_Receive_ModeSw_TaskHandle;
osThreadId  CanMsg_Send_TaskHandle;

osThreadId  Shoot_TaskHandle;
osThreadId  IMU_TaskHandle;

osTimerId   Chassis_Timer_Id;
osTimerId   Gimbal_Timer_Id;
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void RemoteMsg_Receive_ModeSw_TaskStart(void const * argument);
void CanMsg_Send_TaskStart(void const * argument);

void Shoot_TaskStart(void const * argument);
void IMU_TaskStart(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  
  //大家注意，我们要进临界段了~
  taskENTER_CRITICAL();
  
  //先建立两个软件定时器~
  osTimerDef(Chassis_Timer, Chassis_Task);
  Chassis_Timer_Id = osTimerCreate(osTimer(Chassis_Timer), osTimerPeriodic , NULL);

  osTimerDef(Gimbal_Timer, Gimbal_Task);
  Gimbal_Timer_Id  = osTimerCreate(osTimer(Gimbal_Timer), osTimerPeriodic, NULL);
  
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */
  
  
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  //从这里开始创建各种任务
  osThreadDef(RemoteMsg_Receive_ModeSw_Task,RemoteMsg_Receive_ModeSw_TaskStart,osPriorityAboveNormal,0,128);
  RemoteMsg_Receive_ModeSw_TaskHandle = osThreadCreate(osThread(RemoteMsg_Receive_ModeSw_Task),NULL);
  
  osThreadDef(CANMsg_Send_Task,CanMsg_Send_TaskStart , osPriorityAboveNormal , 0 , 128);
  CanMsg_Send_TaskHandle = osThreadCreate(osThread(CANMsg_Send_Task),NULL);
  
  osThreadDef(IMU_Task, IMU_TaskStart, osPriorityNormal, 0 ,128);
  IMU_TaskHandle = osThreadCreate(osThread(IMU_Task),NULL);

  osThreadDef(Shoot_Task, Shoot_TaskStart, osPriorityNormal , 0 , 128);
  Shoot_TaskHandle = osThreadCreate(osThread(Shoot_Task),NULL);
  /* USER CODE END RTOS_THREADS */

  //两个定时器的启动其实还是放到ModeSw_Task下面比较好？
  //osTimerStart(Chassis_Timer_Id, CHASSIS_PERIOD);
  //osTimerStart(Gimbal_Timer_Id, GIMBAL_PERIOD)

  taskEXIT_CRITICAL();
  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Application */    
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
