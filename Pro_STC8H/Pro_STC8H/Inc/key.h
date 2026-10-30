/****************************Copyright (c)****************************
**
**                          技美电子工作室
**                     http://lintar.taobao.com 
**                  tel:13527786389  QQ:1049624740
**                  E-mail:lintar1368@126.com
**                    
*********************************************************************/

#ifndef KEY_H 
#define KEY_H

#include "config.h"
#include "STC8H.h"

#define KEY_INPUT P3  // 按键IO     P3.4
#define KEY_MASK    0x10
#define KEY_ON_OFF  0x00
#define KEY_UP      0x01
#define KEY_DOWN    0x02
#define KEY_UP_DOWN 0x08

#define KEY_ON_OFF_SIGLE    0x81
#define KEY_ON_OFF_LONG     0x82
#define KEY_ON_OFF_SHORT    0x87
#define KEY_UP_SIGLE        0x83
#define KEY_DOWN_SIGLE      0x84
#define KEY_DOWN_LONG       0x85
#define KEY_DOWN_INTERVAL   0x86
#define KEY_DOWN_RELEASE    0x80



#define KEY_STATE_0 0 // 按键状态
#define KEY_STATE_1 1
#define KEY_STATE_2 2
#define KEY_STATE_3 3
#define KEY_STATE_4 4

#define SINGLE_KEY_TIME 5    // SINGLE_KEY_TIME*10MS = 50MS 判定单击的时间长度，软件消抖
#define KEY_INTERVAL    40   // KEY_INTERVAL*10MS = 400MS 判定双击的时间间隔
#define LONG_KEY_TIME   200  // LONG_KEY_TIME*10MS = 2S 判定长按的时间长度

#define N_KEY 0  // no click




/*********************************************
键盘扫描，返回对应键值。须用到约10ms中断定时
标志名为：keytimeFlag或t10msFlag
*********************************************/
unsigned char key_read(void);

#endif
