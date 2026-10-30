
#ifndef LCD1602_H
#define LCD1602_H

#include "config.h" 
#include "STC8H.h"

void delayms(int8u ms);
void lcd_init(void);
void lcd_pos(int8u xpos, int8u ypos);
void wr_string(int8u *p);
void lcd_wcmd(int8u cmd, bit Check);
void lcd_wdat(int8u dat);

#endif