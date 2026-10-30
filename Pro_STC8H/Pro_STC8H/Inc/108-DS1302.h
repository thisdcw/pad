
#ifndef DS1302_H
#define DS1302_H

#include "STC8H.h"
#include "config.h"

#define YLB_SEC      0x80
#define YLB_MIN      0x82
#define YLB_HOUR     0x84
#define YLB_DAY      0x86
#define YLB_MONTH    0x88
#define YLB_WEEK     0x8A
#define YLB_YEAR     0x8C
#define YLB_CTRL     0x8E
#define YLB_CHRG     0x90
#define YLB_BRUST    0xBE
#define YLB_RAM      0xC0

#define CLKOFF       0x80
#define CLKON        0x00
#define T12          0x80
#define T24          0x00
#define LOCK         0x80
#define UNLOCK       0x00
#define CHG_LV6      0xA5         //0.7V½µÑ¹,2K×èÖµ,2.15mA
#define CHG_LV5      0xA9         //1.4V½µÑ¹,2K×èÖµ,1.80mA
#define CHG_LV4      0xA4         //0.7V½µÑ¹,4K×èÖµ,1.07mA
#define CHG_LV3      0xAA         //1.4V½µÑ¹,4K×èÖµ,0.90mA
#define CHG_LV2      0xA7		  //0.7V½µÑ¹,8K×èÖµ,0.50mA
#define CHG_LV1      0xAB         //1.4V½µÑ¹,8K×èÖµ,0.45mA


void init_ds1302();
void read_nowtime(unsigned char addr, unsigned char kn[], unsigned char k);
void write_settime( unsigned char *p);
#endif