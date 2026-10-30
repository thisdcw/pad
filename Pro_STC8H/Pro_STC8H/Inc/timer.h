
#ifndef TIMER_H
#define TIMER_H

#include "config.h"
#include "STC8H.h"

#define T1D2MS (65536-FOSC/800)            //1T
#define T1MS   (65536-FOSC/1000)           //1T
#define T400US (65536-FOSC/2500)           //1T
#define T500US (65536-FOSC/2000)           //1T
#define T600US (65536-FOSC/1666)           //1T
#define T700US (65536-FOSC/1428)           //1T
//#define T250US  (65536-FOSC/4000)
#define T200US  (65536-FOSC/5000)
#define T100US  (65536-FOSC/10000)
//#define T1MS  (65536-FOSC/12/1000)        //12T
#define T250US  (65536-FOSC/4000)
//#define T200US  FOSC/5000

extern  bit   t10msFlag;          //bz.c 按键音使用
extern  bit   bio_flag;           //微电启动输出标识 当能量adc>23时，置标识位

extern  bit   t2msFlag;
extern  bit   t10msFlag1;         //key
extern  bit   t10msFlag2;      //adc
extern  bit   t10msFlag3; 
extern  bit   t100msFlag;
extern  bit   t100msFlag1;     //显示刷新
extern  bit   t300msFlag;
extern  bit   t250msFlag;
extern  bit   t250msFlag1;
extern  bit   t600msFlag;         //main.c 保护警告音定时
extern  bit   t500msFlag;
extern  bit   t500msFlag1;
extern  bit   t500msFlag2;
extern  bit   secFlag1;
extern  bit   secFlag2;
extern  bit   secFlag3;
extern  int8u sec;
extern  int8u min;
extern  int8u pwmmax;

extern int16u freq_counterH;
extern code   int8u  POWER_VALUE[31];

void timer_Init(void);

#endif