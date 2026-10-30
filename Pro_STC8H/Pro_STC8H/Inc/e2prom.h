/****************************Copyright (c)****************************
**
**                          技美电子工作室
**                     http://lintar.taobao.com 
**                  tel:13527786389  QQ:1049624740
**                  E-mail:lintar1368@126.com
**                    
*********************************************************************/

//////////////////////////////////////////////////////////////////////

//STC15系列

//////////////////////////////////////////////////////////////////////

#ifndef E2PROM_H
#define E2PROM_H

#include <INTRINS.h>
#include "config.h"
#include "STC8H.h"

/*Declare SFR associated with the IAP*/
//sfr IAP_DATA     =   0xc2 ;
//sfr IAP_ADDRH    =   0xc3 ;
//sfr IAP_ADDRL    =   0xc4 ;
//sfr IAP_CMD      =   0xc5 ;
//sfr IAP_TRIG     =   0xc6 ;
//sfr IAP_CONTR    =   0xc7 ;

/*Define ISP/IAP/EEPROM command*/
#define   CMD_IDLE     0    //Stand-By
#define   CMD_READ     1    //Byte-Read
#define   CMD_WRITE    2    //Byte-Write
#define   CMD_ERASE    3    //Sector-Erase

/*Define ISP/IAP/EEPROM operation const for IAP_CONTR*/
//#define   ENABLE_IAP   0x80     //if SYSCLK<30MHz
//#define   ENABLE_IAP   0x81     //if SYSCLK<24MHz
//#define   ENABLE_IAP   0x82     //if SYSCLK<20MHz
#define   ENABLE_IAP   0x80     //if SYSCLK<12MHz
//#define   ENABLE_IAP   0x84     //if SYSCLK<6MHz
//#define   ENABLE_IAP   0x85     //if SYSCLK<3MHz
//#define   ENABLE_IAP   0x86     //if SYSCLK<2MHz
//#define   ENABLE_IAP   0x87     //if SYSCLK<1MHz

/*Start address for STC15F408xx EEPROM*/
#define   IAP_ADDRESS  0x0200
#define   IAP_ADDRESS1 0x0400

/*Subroutine redefine*/
void  IapIdle(void);
int8u IapReadByte(int16u addr);
void  IapWriteByte(int16u addr,int8u dat);
void  IapEraseSector(int16u addr);

/***************************存取字数*************************/
//四个字节
 /********************************************************************
函数：STC 向指定地址写一浮点数数据
********************************************************************/
void EROM_write_word(int16u addr,  int32u dat);
/********************************************************************
函数：STC 读指定地址一字数据        
********************************************************************/
int32u EROM_read_word(int16u addr);

#endif
