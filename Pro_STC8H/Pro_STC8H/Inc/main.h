#ifndef MAIN_H
#define MAIN_H

#include "STC8H.h"
#include "config.h"

#define T_TOTAL   0
#define T_CURRENT 1
#define T_NEXT    2



void sys_init(void);
void disInit(void);
void timeDeal(void);
void times_display_sub(int8u dt);

#endif

