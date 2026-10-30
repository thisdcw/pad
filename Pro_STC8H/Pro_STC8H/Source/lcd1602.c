
#include <intrins.h>
#include "lcd1602.h"

int8u ByteTurn(int8u bt)
{
	int8u des=0;
	if(bt&0x01) des |= 0x80;
	if(bt&0x02) des |= 0x40;
	if(bt&0x04) des |= 0x20;
	if(bt&0x08) des |= 0x10;
	if(bt&0x10) des |= 0x08;
	if(bt&0x20) des |= 0x04;
	if(bt&0x40) des |= 0x02;
	if(bt&0x80) des |= 0x01;
	return des;
}

void Delay1us()		//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 1;
	while (--i);
}

/**********************************************************

检查LCD忙状态

lcd_busy为1时，忙，等待。
lcd-busy为0时,闲，可写指令与数据。

 **********************************************************/
void lcd_busy()
{
    int8u busy;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 0;
    while(1){
		Delay1us();
        DATA_PORT=0xff;
        LCD_EN=1;
		Delay1us();
        busy = DATA_PORT;
        LCD_EN = 0;
        if((busy&0x80)==0){
            break;
        }
    }
}

/**********************************************************

写指令数据到LCD

RS=L，RW=L，EN 下降沿执行写操作。D0-D7=指令码。
Check=1，进行忙检测。

 **********************************************************/
void lcd_wcmd(int8u cmd, bit Check)
{
	int8u cmd_tmp;
	// cmd_tmp=ByteTurn(cmd);
	cmd_tmp=cmd;
	if (Check)
	lcd_busy();		  //进行忙检测
	LCD_EN = 0;
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 1;
	DATA_PORT = cmd_tmp;
	Delay1us();
	LCD_EN = 0;
}

/**********************************************************

写显示数据到LCD

RS=H，RW=L，EN 下降沿执行写操作。D0-D7=数据。

 **********************************************************/
void lcd_wdat(int8u dat)
{
	int8u dat_tmp;
	//dat_tmp=ByteTurn(dat);
	dat_tmp=dat;
	lcd_busy();         //进行忙检测
	LCD_EN = 0;
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN = 1;
	DATA_PORT = dat_tmp;
	Delay1us();
	LCD_EN = 0;
}

/**********************************************************

LCD初始化设定

 **********************************************************/
void lcd_init()
{
  lcd_wcmd(0x38, 1); //进行忙检测
  lcd_wcmd(0x0c, 1); //显示开，关光标
  lcd_wcmd(0x06, 1); //移动光标
  lcd_wcmd(0x01, 1); //清除LCD的显示内容
}

/**********************************************************

设定显示位置

 **********************************************************/
void lcd_pos(int8u xpos, int8u ypos)
{
  if (ypos == 0x01)
    lcd_wcmd((xpos | 0x80), 1);
  if (ypos == 0x02)
    lcd_wcmd((xpos | 0xc0), 1);
}

/**********************************************************

写字符串子函数

 **********************************************************/
void wr_string(int8u *p)
{
  while (*p)
  {
    lcd_wdat(*p++);
  }
}


