/****************************Copyright (c)****************************
**                          技美电子工作室
**                     http://lintar.taobao.com 
**                  tel:13527786389  QQ:1049624740
**                  E-mail:lintar1368@126.com
**                    
**---------File Info--------------------------------------------------
**                     STC单片机片内E2PROM读写驱动程序
**                     (e2prom.c)
**    对e2prom进行写操作前，必须对扇区进行擦除操作；
**    故对e2prom内数据进行一次更新应执行读取，擦除，写入的操作；
**
**--------------------------------------------------------------------
** Created by:          肖林发
** Created date:        2010-9-6
** Version:             V1.0
** Descriptions:        原程序编写调试
**
*********************************************************************/
       
#include "e2prom.h"

/*--------------------------------------
Disable ISP/IAP/EEPROM fuction
Make MCU in a safe state
---------------------------------------*/
void IapIdle(void)
{
	IAP_CONTR = 0;      //Close IAP function
	IAP_CMD = 0;        //Clear command to standby
	IAP_TRIG = 0;       //Clear trigger register
	IAP_ADDRH = 0x80;   //Data ptr point to non-EEPROM area
	IAP_ADDRL = 0x00;   //Clear IAP address to prevent misuse
}

/*--------------------------------------
Read one byte from ISP/IAP/EEPROM area
Input: addr(ISP/IAP/EEPROM address)
Output: Flash data
---------------------------------------*/
int8u IapReadByte(int16u addr)
{
	int8u dat;                     //data buffer
	F0=EA;
	EA=0;
	IAP_CONTR = ENABLE_IAP;        //Open IAP function and set wait time
    IAP_TPS   =12;
	IAP_CMD = CMD_READ;            //Set ISP/IAP/EEPROM READ command
	IAP_ADDRL = (int8u)addr;       //Set ISP/IAP/EEPROM low address
	IAP_ADDRH = (int8u)(addr>>8);  //Set ISP/IAP/EEPROM high address
	IAP_TRIG = 0x5a;               //Send trigger command1(0x5a)
	IAP_TRIG = 0xa5;               //Send trigger command2(0xa5)
	_nop_();                       //MCU will hold here until ISP/IAP/EEPROM operation complete
	dat = IAP_DATA;                //Read ISP/IAP/EEPROM data
	IapIdle();                     //Close ISP/IAP/EEPROM function
	EA=F0;
	return dat;                    //Return Flash data
}

/*--------------------------------------
Write one byte to ISP/IAP/EEPROM area
Input: addr(ISP/IAP/EEPROM address) 
       dat(ISP/IAP/EEPROM data)
Output: non
---------------------------------------*/
void IapWriteByte(int16u addr,int8u dat)
{
	F0=EA;
	EA=0;
	IAP_CONTR = ENABLE_IAP;        //Open IAP function and set wait time
    IAP_TPS   =12;
	IAP_CMD = CMD_WRITE;           //Set ISP/IAP/EEPROM write command
	IAP_ADDRL = (int8u)addr;       //Set ISP/IAP/EEPROM low address
	IAP_ADDRH = (int8u)(addr>>8);  //Set ISP/IAP/EEPROM high address
	IAP_DATA = dat;                //Write ISP/IAP/EEPROM data
	IAP_TRIG = 0x5a;               //Send trigger command1(0x5a)
	IAP_TRIG = 0xa5;               //Send trigger command2(0xa5)
	_nop_();                       //MCU will hold here until ISP/IAP/EEPROM operation complete
	IapIdle();                     //Close ISP/IAP/EEPROM function
	EA=F0;
}

/*--------------------------------------
Erase one sector area
Input: addr(ISP/IAP/EEPROM address)
Output: non
--------------------------------------*/
void IapEraseSector(int16u addr)
{
	F0=EA;
	EA=0;
	IAP_CONTR = ENABLE_IAP;        //Open IAP function and set wait time
    IAP_TPS   =12;
	IAP_CMD = CMD_ERASE;           //Set ISP/IAP/EEPROM erase command
	IAP_ADDRL = (int8u)addr;       //Set ISP/IAP/EEPROM low address
	IAP_ADDRH = (int8u)(addr>>8);  //Set ISP/IAP/EEPROM high address
	IAP_TRIG = 0x5a;               //Send trigger command1(0x5a)
	IAP_TRIG = 0xa5;               //Send trigger command2(0xa5)
	_nop_();                       //MCU will hold here until ISP/IAP/EEPROM operation complete
	IapIdle();                     //Close ISP/IAP/EEPROM function
	EA=F0;
}

/***************************存取字数*************************/
//四个字节
 /********************************************************************
函数：STC 向指定地址写一字数据
********************************************************************/
void EROM_write_word(int16u addr,  int32u dat)
{     
    IapWriteByte(addr++, ((int8u *)&dat)[0]); 
    IapWriteByte(addr++, ((int8u *)&dat)[1]); 
    IapWriteByte(addr++, ((int8u *)&dat)[2]); 
    IapWriteByte(addr,   ((int8u *)&dat)[3]);   
}

/********************************************************************
函数：STC 读指定地址一字数据        
********************************************************************/
int32u EROM_read_word(int16u addr)
{                      
    int32u dat;
 
    ((int8u *)&dat)[0] = IapReadByte(addr++);
    ((int8u *)&dat)[1] = IapReadByte(addr++);
    ((int8u *)&dat)[2] = IapReadByte(addr++);
    ((int8u *)&dat)[3] = IapReadByte(addr);
 
    return dat;                        
}
