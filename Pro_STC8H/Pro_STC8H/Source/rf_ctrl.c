
#include "rf_ctrl.h"
#include "timer.h"
#include "bz.h"
#include "Adc_C51.h"
#include "GlobalVar.h"

#define  START_TOUCH_PWM_VALUE    50
#define  STOP_TOUCH_PWM_VALUE    100

int8u   enery_dec;
int8u   TouchFlag;      //接触状态基：0无接触 1正常接触 2短路接触
int8u   TouchCnt;       //接触与脱离计数器
int16u  adc_null_value;         //RF不工作时的ADC值
int16u  adc_max_vlaue;          //RF工作中检测到最大的ADC值

code int16u  CMP_MIN[31]={100,   150,170,180,190,200,   210,210,230,240,250,   250,255,260,265,270,   275,285,285,295,295,  305,305,305,315,325,  350,365,380,385,400};
    
//dir=0时停止，1时功率减，2时启动,3功率加
void rf_state_update(int8u sta)	
{
	TouchCnt=0;
	if(sta==0){
		TouchFlag=0;
		enery_dec=0;
		pwmmax=0;
	}else if(sta==1){
		enery_dec++;
		if((system_state==2)&&((TouchFlag==1)||(TouchFlag==2))){
			pwmmax=POWER_VALUE[RF_Power];
		}
	}else if(sta==2){
		TouchFlag=0;
		enery_dec=0;
		pwmmax=TEST_PWM;
	}else if(sta==3){
		if((system_state==2)&&((TouchFlag==1)||(TouchFlag==2))){
			pwmmax=POWER_VALUE[RF_Power];
		}
	}
}
	
void pwm_ctrl(void)
{
    if(t100msFlag){
        t100msFlag=0;
        if((system_state==2)&&(RF_Power>0)){
            switch(TouchFlag){
            case 0:
                pwmmax=TEST_PWM;
                adc_max_vlaue=STOP_TOUCH_PWM_VALUE;
                if(adc_avg > (adc_null_value+START_TOUCH_PWM_VALUE)){
                    TouchCnt++;
                    if(TouchCnt>=2){
                        TouchFlag=1;                 //上升
                        TouchCnt=0;
                        pwmmax=POWER_VALUE[RF_Power];
                    }
                }else{
                    TouchCnt=0;
                }
                break;
            case 1:
                TouchCnt++;
                if(TouchCnt>=5){                   //0.1*(2+5)=0.7s后稳定输出
                    TouchCnt=0;
                    TouchFlag=2;
                    //txbuf[0]=0;
                    //txbuf[1]=1;
                    //DataRegWrite(0x4000,2,txbuf);
                    keyVoiceFlag=KEY_NOMAL_VOICE;
                    //rootPlay(0x0001);
                }
                if(adc_avg>adc_max_vlaue){
                    adc_max_vlaue=adc_avg;
                }
                if(adc_avg<CMP_MIN[RF_Power]){
                    pwmmax=TEST_PWM;
                    TouchFlag=0;
                    TouchCnt=0;
                }
                break;
            case 2:
                if(enery_dec>0){
                    TouchCnt++;
                    if(TouchCnt>=5){
                        TouchCnt=0;
                        enery_dec=0;
                    }
                    adc_max_vlaue=adc_avg;
                }else{
                    if(adc_avg < (adc_max_vlaue-STOP_TOUCH_PWM_VALUE)){        //有移开
                        TouchCnt++;
                        if(TouchCnt>=2){
                            pwmmax=0;
                            TouchCnt=0;
                            //system_lock=0;
                            TouchFlag=3;
                            keyVoiceFlag=KEY_NOMAL_VOICE;
                            //rootPlay(0x0002);
                            //txbuf[0]=0;
                            //txbuf[1]=2;
                            //DataRegWrite(0x4000,2,txbuf);
                        }
                    }else{
                        TouchCnt=0;
                    }
                    if(adc_avg>adc_max_vlaue){
                        adc_max_vlaue=adc_avg;
                    }
                }
                break;
            case 3:                               //移开延时重新给最小输出                          
                TouchCnt++;
                if(TouchCnt>=5){
                    TouchCnt=0;
                    TouchFlag=0;
                    TouchCnt=0;
                    pwmmax=TEST_PWM;
                    //system_lock=1;
                    adc_max_vlaue=STOP_TOUCH_PWM_VALUE;
                }
                break;
            default:
                break;
            }
        }else{
            TouchFlag=0;
            TouchCnt=0;
            pwmmax=0;
            //adc_null_value += adc_avg;
            //adc_null_value >>= 1;
            adc_null_value=adc_avg;
            adc_max_vlaue=STOP_TOUCH_PWM_VALUE;    //确保adc_max_vlaue>=STOP_TOUCH_PWM_VALUE
        }
    }
}