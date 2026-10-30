
#include"pwm.h"

void pwm_init(void)
{
	P_SW2 |= 0x80;
	PWMB_PS &= ~0x30;   //PWM7
	PWMB_PS |= 0x20;    //PWM7 use P02
	PWMB_CR1 = 0x01;
	PWMB_CCER2 = 0x00;  //写CCMRx前必须先清0CCERx
	PWMB_CCMR3 = 0x68;  //设置CC1PWMB输出模式,输出比较预装功能
	PWMB_CCER2 = 0x01;  //使能CC2
	PWMB_PSCR = 0;        
	PWMB_CCR7 = 0;      //占空比
	PWMB_ARR = 110;     //周期
	PWMB_ENO |= 0x10;   //enable PWM7
	PWMB_BKR |= 0x80;   //主输出使能
}

void Set_PWM(unsigned char power)
{
	P_SW2 |= 0x80;
	PWMB_CCR7 = power;
}