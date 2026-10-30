 
 #ifndef Config_H
 #define Config_H


 #ifdef  _GLOBAL
 #define _EXTERN 
 #else
 #define _EXTERN extern
 #endif
 
 #define CLI() EA = 0
 #define SEI() EA = 1

 #define   int8u unsigned char
 #define   int16u unsigned int
 #define   int32u unsigned long
   
 #define   uint8_t  unsigned char
 #define   uint16_t unsigned int
 #define   int16_t  signed int
 #define   uint32_t unsigned long

 #define   setbit(a,b)    a |= (1<<b);
 #define   clrbit(a,b)    a &= ~(1<<b);

 #define FOSC 11059200L        //系统时钟
 #define F_PWMCKS 11059200L    //PWM时钟
 
 #define BAUD_S1  38400

//#define DS1302_CLK     P20
//#define DS1302_DAT     P21
//#define DS1302_RST     P22

#define BZPORT         P33     //高电平有效
#define BZON           0
#define BZOFF          1

#define DATA_PORT      P2
#define LCD_RS         P35
#define LCD_RW         P36
#define LCD_EN         P37
 

                              
#define HOT_SW          P23   //24V热  H  暂时不用，会影响RF负载检测
#define FAN_SW          P22   //12V热  H
#define RF_SEL          P11   //低电平微电，高电平RF
#define RF_SW           P10   //L

#define BIO_P1          P01   //H
#define BIO_P2          P03   //H
#define BIO_PWM         P02   //H

#define LED_P1          P14   //H
#define LED_P2          P15   //H
#define LED_P3          P16   //H

#define LED_N1          P17   //H
#define LED_N2          P54   //H
#define LED_N3          P32   //H
#define LED_N4          P33   //H

//adc
//#define ADC_TMP       0   //测温10K 暂无用
#define ADC_ENG         3
#define ADC_MOD         2
#define ADC_RFT         5

#define  TEST_PWM       2

#define  DEF_MIN        60  //默认时间
#define  DEC_MIN        55  //减次数时间

#endif
