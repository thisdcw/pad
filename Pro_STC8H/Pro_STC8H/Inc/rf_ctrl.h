
#ifndef RF_CTRL_H
#define RF_CTRL_H

#include "config.h"
#include "STC8H.h"

#define  STOP_STA   0
#define  STAR_STA   2
#define  PDEC_STA   1
#define  PINC_STA   3

void pwm_ctrl(void);
//dir=0时停止，1时功率减，2时启动,3功率加
void rf_state_update(int8u sta);	

#endif




