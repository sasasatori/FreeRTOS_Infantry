#ifndef _Remote_H_
#define _Remote_H_

#include "stm32f4xx_hal.h"
#include "usart.h"

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！潤更悶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
typedef __packed struct
{
    __packed struct
    {
        int16_t ch0;
        int16_t ch1;
        int16_t ch2;
        int16_t ch3;

        uint8_t  s1;
        uint8_t  s2;
    }remote;

    __packed struct
    {
        int16_t x;
        int16_t y;
        int16_t z;

        uint8_t left;
        uint8_t right;
    }mouse;

    __packed union
    {
        uint16_t key;
        __packed struct
        {
            uint16_t W:1;
            uint16_t S:1;
            uint16_t A:1;
            uint16_t D:1;
            uint16_t SHIFT:1;
            uint16_t CTRL:1;
            uint16_t Q:1;
            uint16_t E:1;
            uint16_t R:1;
            uint16_t F:1;
            uint16_t G:1;
            uint16_t Z:1;
            uint16_t X:1;
            uint16_t C:1;
            uint16_t V:1;
            uint16_t B:1;
        }bit;
    }keyboard;

} remote_info_t;


/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！崎協吶！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

#define DBUS_MAX_LENTH        30
#define DBUS_BUF_LENTH        18

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！延楚！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

extern uint8_t          dbus_buff[];
extern remote_info_t    remote_data;

/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！痕方！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

void RemoteMsg_decode_handler(remote_info_t *rc , uint8_t *buff);

#endif /*_Remote_H_*/
