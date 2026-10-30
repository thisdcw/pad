/****************************Copyright (c)****************************
**
**                          技美电子工作室
**                     http://lintar.taobao.com 
**                  tel:13527786389  QQ:1049624740
**                  E-mail:lintar1368@126.com
**                    
*********************************************************************/

/**********************************************
ADC读按键。
**********************************************/

#ifndef ADC_C51_H 
#define ADC_C51_H

#include "config.h"
#include "STC8H.h"


/*********************************************
特殊功能寄存器定义（STC12C5A32S2系列单片机）
*********************************************/
//sfr     ADC_CONTR    =   0xBC;        //ADC control register
//sfr     ADC_RES      =   0xBD;        //ADC hight 8bit result register
//sfr     ADC_RESL     =   0xBE;        //ADC low 2bit result register
//sfr     P1ASF        =   0x9D;        //P1 secondary function control register
//sfr     AUXR1        =   0xA2;
/********************************************/

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER       0x80
#define ADC_FLAG        0x20
#define ADC_START       0x40
#define ADC_SPEEDLL     0x00
#define ADC_SPEEDL      0x20
#define ADC_SPEEDH      0x40
#define ADC_SPEEDHH     0x60
#define ADRJ            0x04



#define KEY_INTENA   0x81
#define KEY_INTEND   0x82
#define KEY_FUNCA    0x83
#define KEY_FUNCD    0x84
#define KEY_CONFIRM  0x85
#define KEY_EXIT     0x86
#define KEY_TIMEA    0x87
#define KEY_TIMED    0x88
#define KEY_ENTER    0x89
#define KEY_NULL     0x00

#define  Vref   5000

extern int8u   real_temp1;
extern int16u  ADC_ENG_Value;
extern int16u  ADC_MOD_Value;
extern int16u  adc_avg;

/*********************************************
初始化ADC
*********************************************/
void InitADC(void);
void ADC_Dis_Cvt(void);
void start_adc(void);

#endif
