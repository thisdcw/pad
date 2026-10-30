
#ifndef LED_DRV_H
#define LED_DRV_H

#include "config.h"
#include "STC8H.h"

/*
#define  LED1   9
#define  LED2   6
#define  LED3   7
#define  LED4   8
#define  LED5   3
#define  LED6   4
#define  LED7   5
#define  LED8   2
#define  LED9   1
#define  LED10  0
*/

#define  LED_MASK   0x3f


extern int16u led_var;

void led_driver(void);

#endif
