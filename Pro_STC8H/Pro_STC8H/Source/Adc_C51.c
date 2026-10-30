

#include "Adc_C51.h"
#include "timer.h"
#include"pwm.h"
#include "GlobalVar.h"

bit    adcCvtFlag;//ADC转换结束标志
int16u ADC_Value;
int16u tempValue1;
int8u  real_temp1;
int8u  chnl;             //ADC channel NO.

int16u  ADC_ENG_Value;
int16u  ADC_MOD_Value;

int8u   pwm_value1;
int8u   pwm_value2;

int16u adc_avg;
int8u  adc_cnt;
bit    dt_adc_flag;

void InitADC(void)
{
    P_SW2 |= 0x80;
	ADCTIM = 0x3f;
	P_SW2 &= ~0x80;
	ADCCFG = 0x27;     //设计ADC时钟为系统时钟/2/8
	ADC_CONTR = 0x82;  //打开ADC电源，并选择第2通道
	EADC = 1;          //允许ADC中断
    chnl=ADC_MOD;
}
void start_adc(void)
{
	//可能有BUG 有可能转换中
	ADC_CONTR &= 0xf0;
    ADC_CONTR |= ADC_START|ADC_RFT;
    adc_cnt=0;
    dt_adc_flag=1;
}

float code temp_tb[]={
    32.7547,31.1243,29.5847,28.1301,26.7556,
    25.4562,24.2274,23.065,21.965,20.9239,19.938,19.0041,18.1193,17.2807,16.4857,15.7317,15.0164,14.3376,
    13.6933,13.0816,12.5005,11.9485,11.4239,10.9252,10.451,10,9.5709,9.1626,8.7738,8.4037,8.0512,7.7154,
    7.3953,7.0903,6.7995,6.5221,6.2576,6.0051,5.7642,5.5342,5.3146,5.1049,4.9045,4.713,4.53,4.3551,4.1878,
    4.0278,3.8748,3.7283,3.5882,3.454,3.3255,3.2025,3.0846,2.9717,2.8635,2.7597,2.6603,2.5649,2.4734,2.3856,
    2.3014,2.2206,2.1431,2.0686,1.997,1.9283,1.8623,1.7989,1.738
};

int8u ADC_to_TMEP(uint16_t adc_value)
{
	uint8_t i;
	float res_value;
	adc_value = (uint16_t)(((uint32_t)adc_value*Vref)>>10);                 //电压
	res_value = (((float)(adc_value*10))/(Vref-adc_value));                 //电阻
	for(i=0;i<sizeof(temp_tb)/sizeof(temp_tb[0]);i++)
	{
		if(res_value>temp_tb[i])break;
	}
	return i;
}

void ADC_Dis_Cvt(void)
{
	static int8u adcnt1=0,adcnt2=0;
	static int16u adctemp1=0,adctemp2=0;
	static int16u adc_sum=0;
	if(t10msFlag2&&(!dt_adc_flag)){
		t10msFlag2=0;
		ADC_CONTR &= 0xf0;
		ADC_CONTR |= ADC_START|chnl;
	}
	if(adcCvtFlag){
		adcCvtFlag=0;
		if(dt_adc_flag){                 //反馈检测
            adc_cnt++;
            if(adc_cnt<3){
                adc_sum=0;
            }else{
                adc_sum += ADC_Value;
                if(adc_cnt>=6){          //完成6次转换，第2次丢弃
                    adc_avg = adc_sum>>2;
                    dt_adc_flag=0;
                }
            }
            if(adc_cnt<6){
				ADC_CONTR &= 0xf0;
                ADC_CONTR |= ADC_START|ADC_RFT;
            }
		}else{
			if(chnl==ADC_ENG){
				adctemp1 += ADC_Value;
				adcnt1++;
				if(adcnt1>=4){
					ADC_ENG_Value=adctemp1>>2;
					if(func==0){
						pwm_value1=ADC_ENG_Value/18;
						Set_PWM(pwm_value1+pwm_value2);
					}else if(func==1){
						
					}
					adcnt1=0;
					adctemp1=0;
				}	
				chnl=ADC_MOD;
			}else if(chnl==ADC_MOD){
				adctemp2 += ADC_Value;
				adcnt2++;
				if(adcnt2>=4){
					ADC_MOD_Value=adctemp2>>2;
					if(func==0){
						pwm_value2=ADC_MOD_Value/146;
						Set_PWM(pwm_value1+pwm_value2);
						if(ADC_MOD_Value>762){
							freq_counterH=(ADC_MOD_Value-762)/5+227;
						}else{
							freq_counterH=ADC_MOD_Value/6+100;
						}
					}
					adcnt2=0;
					adctemp2=0;
				}	
				chnl=ADC_ENG;
			}
		}
	}
}



/*----------------------------------------
ADC interrupt service routine
-----------------------------------------*/
void adc_isr() interrupt 5 using 2
{
	ADC_CONTR &= ~ADC_FLAG;                                 //清中断标识
	ADC_Value = (ADC_RES<<8)|ADC_RESL;	                    //读ADC结果
	adcCvtFlag = 1;	                                        //标志ADC已完成
	ADC_RES  = 0;
	ADC_RESL = 0;
}
