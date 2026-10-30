#include "timer.h"
#include "Adc_C51.h"
#include "GlobalVar.h"



bit   msFlag;

bit   secFlag1;
bit   secFlag2;
bit   secFlag3;


int8u time_conter0;
int8u time_conter1;
int8u time_conter2;
int8u time_conter3;
int8u time_conter4;
int8u time_conter5;

bit t2msFlag;
bit t10msFlag;       //bz
bit t10msFlag1;      //key
bit t10msFlag2;      //adc
bit t10msFlag3;      //adc
bit t100msFlag;      //RF控制
bit t100msFlag1;     //显示刷新
bit t300msFlag;
bit t250msFlag;
bit t250msFlag1;
bit t600msFlag;
bit t500msFlag;
bit t500msFlag1;
bit t500msFlag2;

int16u freq_counterL;
int16u freq_counterH;
int16u freq_cnt;
int8u sec_pulse_delay;
bit pulse_flag;
bit bio_flag;               //微电启动输出标识 当能量adc>23时，置标识位

int8u  mscnt;
int8u  pwmcnt;
int8u  pwmmax;
code   int8u  POWER_VALUE[31]={4,12,14,16,18,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};
	

     

void timer_Init(void)
{
   /*定时器0*/
   AUXR |= 0xc0;     //set T0/T1 1T

   TMOD = 0x01;      //T0/T1设置为16位模式  T1自动重装
   
   TL0  = T500US;     //用于定时输出500US脉冲
   TH0  = T500US>>8;
   TF0  = 0;
   ET0  = 1;
   PT0  =1;          //优先级1
   TR0  = 0;    
    
   TL1  = T200US;
   TH1  = T200US>>8;   //set timer1 conter 500us init
   TF1  = 0;           //clear timer1 interrupt flag
   ET1  = 1;
   TR1  = 1;          //stop timer1
	
	freq_counterL=100;
	freq_counterH=100;
}

void Timer0_Routine(void) interrupt 1 using 1
{
	BIO_P1=0;
	BIO_P2=0;
	TR0  = 0; 
}

void Timer1_Routine(void) interrupt 3 using 1
{
	time_conter0++;
	mscnt++;
	if(mscnt>=10){
		mscnt=0;
		t2msFlag=1;
	}
	if(time_conter0==12){
		t10msFlag=1;
	}else if(time_conter0==24){
		t10msFlag2=1;
	}else if(time_conter0==36){
		t10msFlag1=1;
    }else if(time_conter0>=50){//10ms
		time_conter0=0;
		t10msFlag3=1;
		time_conter1++;
		time_conter3++;
		time_conter4++;
		time_conter5++;
		if(time_conter3>=25){
			time_conter3=0;
			t250msFlag=1;
		}
		if(time_conter5==5){
			t100msFlag1=1;
		}else if(time_conter5>=10){
			time_conter5=0;
			t100msFlag=1;
		}
		if(time_conter4>=50){
			time_conter4=0;
			t500msFlag=1;
			t500msFlag1=1;
            t500msFlag2=1;
		}
		if(time_conter1>=100){
			time_conter1=0;
			secFlag1=1;
			secFlag2=1;
			//secFlag3=1;
		}
	}
	if(func==0){
		if(system_state==2){
			freq_cnt++;
			if(freq_cnt>=freq_counterL){
				freq_cnt=0;
				freq_counterL=freq_counterH;
				if(bio_flag){
					TL0  = T700US;     
					TH0  = T700US>>8;
					BIO_P1=1;
					TR0  = 1;  
					pulse_flag=1;
					sec_pulse_delay=0;
				}
			}
			if(pulse_flag==1){
				sec_pulse_delay++;
				if(sec_pulse_delay>=30){
					sec_pulse_delay=0;
					TL0  = T600US;     
					TH0  = T600US>>8;
					BIO_P2=1;
					TR0  = 1; 
					pulse_flag=0;				
				}
			}
		}
		RF_SW=1;
	}else if(func==1){
		if((system_state==2)&&(RF_Power>0)){                                              //RF调功率
			if(pwmmax>0){
				if(pwmcnt<49){
					pwmcnt++;
					if(pwmcnt==pwmmax){
						RF_SW=1;
					}
				}else{
					pwmcnt=0;
					RF_SW=0;
					start_adc();
				}
			}else{
				RF_SW=1;
			}
		}else{
			RF_SW=1;
			if(t10msFlag3){
				t10msFlag3=0;
				start_adc();
			}
		}
	}else{
		RF_SW=1;
    }
}

