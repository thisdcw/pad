#ifndef BZ_H
#define BZ_H

#include "STC8H.h"
#include "config.h"
#include "timer.h"

#define KEY_NOMAL_VOICE  0x81
#define KEY_WARM_VOICE   0x82
#define KEY_SHORT_VOICE  0x84
#define SYS_OVER_VOICE   0x88
#define THREE_LONG_VOICE 0x90
#define FOUR_SHORT_VOICE 0xA0

extern int8u keyVoiceFlag;

void keyVoice(void);


#endif