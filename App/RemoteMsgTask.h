#ifndef _Remote_H_
#define _Remote_H_

#include "stm32f4xx_hal.h"
#include "usart.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！潤更悶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
typedef struct
{
    struct
    {
        uint16_t ch0;
        uint16_t ch1;
        uint16_t ch2;
        uint16_t ch3;

        uint8_t  s1;
        uint8_t  s2;
    }remote;

    struct
    {
        int16_t x;
        int16_t y;
        int16_t z;

        uint8_t left;
        uint8_t right;
    }mouse;

    
} rc_info_t;


/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
#define DBUS_MAX_LEN        50
#define DBUS_BUF_LEN        18

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

void rc_decode_handler(rc_info_t *rc , uint8_t *buff);

#endif /*_Remote_H_*/
