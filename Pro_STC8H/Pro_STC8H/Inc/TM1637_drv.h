#ifndef TM1637_DRV_H
#define TM1637_DRV_H


#include "config.h"
#include "STC8H.h"

#define HAL_OK       1
#define HAL_ERROR    0

#define   DIO_PORT       P17
#define   CLK_PORT       P16
 
#define DIO(x)     ((x)?(DIO_PORT=1):(DIO_PORT=0))
#define CLK(x)     ((x)?(CLK_PORT=1):(CLK_PORT=0))

#define DIO_IN     DIO_PORT

extern code uint8_t number[];

void TM1637_Init(void);
void TM1637_Test(void);
void TM1637_update(int8u *buf);

#endif
