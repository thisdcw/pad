
#ifndef UART1_H
#define UART1_H

#include "STC8H.h"
#include "config.h"

#define cmd_play  0x11
#define cmd_pause 0x12
#define cmd_front 0x13
#define cmd_back  0x14
#define cmd_volA  0x15
#define cmd_volD  0x16

extern xdata int8u S1RX_BUFFER[10];     //
extern xdata int8u S1TX_BUFFER[20];     //
extern xdata int8u Rx1EndFlag;

void uart1_Init(void);
void cmd_448k(int16u add,int8u value);
void back_app(int8u* buf);

#endif