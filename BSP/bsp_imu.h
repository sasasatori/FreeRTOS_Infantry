#ifdef __IMU_TASK_H__
#define __IMU_TASK_H__

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"


uint8_t mpu_device_init(void);
void    mpu_get_data(void);
void    mpu_offset_cal(void);

#endif
