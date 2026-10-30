

#include "led_drv.h"
#include "timer.h"

int16u led_var;
int8u  led_state;

//P16 P15 P14为正极段码口
#define LOW3BIT  0x07

void led_driver(void)
{
	int8u led_tmp;
	if(t2msFlag){
		t2msFlag=0;
		LED_N1=0;
		LED_N2=0;
		LED_N3=0;
		LED_N4=0;
		P1 &= 0x8f;
		switch(led_state){
		case 0:
			led_tmp=(int8u)led_var;
			led_tmp &= LOW3BIT;
			led_tmp <<= 4;
			P1 |= led_tmp;
			LED_N1=1;
			led_state=1;
			break;
		case 1:
			led_tmp=(int8u)(led_var>>3);
			led_tmp &= LOW3BIT;
			led_tmp <<= 4;
			P1 |= led_tmp;
			LED_N2=1;
			led_state=2;
			break;
		case 2:
			led_tmp=(int8u)(led_var>>6);
			led_tmp &= LOW3BIT;
			led_tmp <<= 4;
			P1 |= led_tmp;
			LED_N3=1;
			led_state=3;
			break;
		case 3:
			led_tmp=(int8u)(led_var>>9);
			led_tmp &= LOW3BIT;
			led_tmp <<= 4;
			P1 |= led_tmp;
			LED_N4=1;
			led_state=0;
			break;
		default:
			break;
		}
	}
}