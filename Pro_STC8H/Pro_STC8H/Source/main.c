
#include "bz.h"
#include "timer.h"
#include "GlobalVar.h"
//#include "108-DS1302.h"
//#include "RC522.h"
//#include "lcd1602.h"
//#include "key.h"
#include "Adc_C51.h"
#include "main.h"
#include "Binary.h"
#include "pwm.h"
#include "key.h"
//#include "led_drv.h"
#include "TM1637_drv.h"
#include "rf_ctrl.h"
#include "uart1.h"
#include "stdio.h"
#include "e2prom.h"

//disbuf[4]的赋值
#define FUNC_RF   1
#define FUNC_EMS  2

xdata int8u disbuf[6]={11,11,11,11,FUNC_EMS,0};
//int8u systemState;   //0 logo定时 1工作定时 2时钟调节
//int8u WarmTimer;     //定时报警计数器
//int8u WarmTimerCnt;  //定时报警计数器
int8u min;
int8u sec;
//int8u mode;
//int8u modeCnt;       //模式持续定时器
//int8u clockIndex;    //时钟参数索引 0时十，1时个，2分十，3分个，4年十，5年个，6月十，7月个，8日十，9日个

//int8u temp_Dis[]="25du";
xdata int8u times_DisTemp[]="000";
xdata int8u time_DisTemp[]="00:00";
xdata int8u power_DisTemp[]="00";

//char   checkCardFlag;  //读卡标识,0验卡失败，1验卡成功, 0x0f未寻到卡
xdata int8u  checkCardState; //读卡状态,0验卡失败，1验卡成功, 2进入工作系统
xdata int16u card_Times;   //卡次数
xdata int8u  test_delay=3;
xdata int8u  fans_delay=0;

int8u func;    //0三通大师  1磁浮
int8u RF_Power;//磁浮功率0-30
int8u BIO_Power;
int8u system_state;
int8u eng_tmp;

#define CHIPID_ADDR 0x6ff9        //STC8H1K28
char  *chip_id;
xdata char chip_id_tbl[7];
xdata char pass_id_tbl[7];//      pass_id_tbl[6]为checksum
xdata int8u check_sum;
xdata int8u key_code;
xdata int8u key_code_tmp1;
xdata int8u key_code_tmp2;
xdata int16u work_times=0;//工作次数
xdata int16u work_time=DEF_MIN; //工作时间
xdata int8u sys_chg_flag=0;
xdata int8u flash_flag=0;
xdata int8u e2prom_buf[6]={0};
xdata int8u times_dis_delay;
/*
如果机子保存有次数，那么开机直接到工作页面，如果没有剩余次数数码管就显示“E0 E0”，
然后工作5分钟后扣费一次次数，倒计时结束，如果次数没有就显示"E0 E0 "，还有剩余次数
像现在一样，恢复工作时间等待下次工作
*/

void no_card_test(void)
{
	checkCardState=2;
	min=DEF_MIN;
	sec=0;
	func=0;
	RF_SEL=0;
	disbuf[4]=FUNC_EMS;
	disbuf[0]=number[min/10];
	disbuf[1] &= 0x04;
	disbuf[1] |= number[min%10];
	if(eng_tmp>0){
		disbuf[2]=number[12];
		disbuf[3]=number[12];
		system_state=0;
		keyVoiceFlag=FOUR_SHORT_VOICE;
	}else{
		disbuf[2]=0;
		disbuf[3]=0;
		system_state=2;
		keyVoiceFlag=KEY_NOMAL_VOICE;
	}
	TM1637_update(disbuf);
	//JDQ=0;
	HOT_SW=1;
}

void main(void)
{
	int8u keyValue;
    sys_init();
	IapIdle();
	timer_Init();
    InitADC();
    //RC522_Init();
	//lcd_init();
	uart1_Init();
	pwm_init();
	TM1637_Init();
    TM1637_Test();
	SEI();
    //disInit();
    keyVoiceFlag=KEY_NOMAL_VOICE;
	//获取chipid
	chip_id=(char code *)CHIPID_ADDR;
	key_code_tmp1=0;
	for(keyValue=0;keyValue<6;keyValue++){
		chip_id_tbl[keyValue]=chip_id[keyValue];
		key_code_tmp1 ^= chip_id[keyValue];
	}
	for(keyValue=0;keyValue<6;keyValue++){
		e2prom_buf[keyValue]=IapReadByte(keyValue);
	}
	if((e2prom_buf[0]==0x5a)&&(e2prom_buf[1]==0xa5)){
		work_times=(e2prom_buf[2]<<8)|e2prom_buf[3];
		work_time =(e2prom_buf[4]<<8)|e2prom_buf[5];
	}else{
		//初次使用，恢复默认值
		IapEraseSector(0);
		IapWriteByte(0,0x5a);
		IapWriteByte(1,0xa5);
		IapWriteByte(2,0);
		IapWriteByte(3,0);
		IapWriteByte(4,0);
		IapWriteByte(5,0);
		work_times=0;
		work_time=0;
	}
	times_display_sub(3);
	while(1){
		if(sys_chg_flag==1){
			if(secFlag2){
				secFlag2=0;
				if(test_delay>0){
					test_delay--;
					if(test_delay==0){
						no_card_test();
					}
				}
				if(fans_delay>0){
					fans_delay--;
					if(fans_delay==0){
						FAN_SW=0;
					}
				}
			}
			ADC_Dis_Cvt();
			if(func==1){
				//pwm_ctrl();
			}
			//led_driver();
			keyValue=key_read();
			if(keyValue==KEY_ON_OFF_LONG){
				/*
				keyVoiceFlag=KEY_NOMAL_VOICE;
				system_state=0;
				rf_state_update(STOP_STA);
				if(func==0){
					func=1;
					RF_SEL=1;
					disbuf[4]=FUNC_RF;
				}else{
					func=0;
					RF_SEL=0;
					disbuf[4]=FUNC_EMS;
				}
				TM1637_update(disbuf);*/
			}else if(keyValue==KEY_ON_OFF_SHORT){
				rf_state_update(STOP_STA);
				if(eng_tmp>0){
					disbuf[1] &= ~0x04;
					disbuf[2]=number[12];
					disbuf[3]=number[12];
					system_state=0;
					fans_delay=90;
					keyVoiceFlag=FOUR_SHORT_VOICE;
					cmd_448k(0x2203,0);
				}else{
					keyVoiceFlag=KEY_NOMAL_VOICE;
					system_state=2;
					if(func==0){
						func=1;
						cmd_448k(0x2000,1);
						//cmd_448k(0x2200,0); 
						RF_SEL=1;
						FAN_SW=1;
						disbuf[4]=FUNC_RF;
						rf_state_update(STAR_STA);
						cmd_448k(0x2203,1);
					}else{
						func=0;
						fans_delay=90;
						RF_SEL=0;
						disbuf[4]=FUNC_EMS;
					}
					if(min==0){
						min=DEF_MIN;
						disbuf[0]=number[min/10];
						disbuf[1] &= 0x04;
						disbuf[1] |= number[min%10];
					}
					//TM1637_update(disbuf)
				}
			}
			timeDeal();
			if(t250msFlag){               //250ms detect once
				t250msFlag=0;
				if(ADC_ENG_Value>23){
					if(func==0){
						bio_flag=1;
					}
					eng_tmp = (ADC_ENG_Value+11)/34;                               //转为0-30
				}else{
					bio_flag=0;
					eng_tmp=0;
				}
				if(func==1){
					if(eng_tmp!=RF_Power){
						if(eng_tmp<RF_Power){
							RF_Power=eng_tmp;
							rf_state_update(PDEC_STA);
						}else if(eng_tmp>RF_Power){
							RF_Power=eng_tmp;
							rf_state_update(PINC_STA);
						}
						cmd_448k(0x2201,RF_Power);
					}
				}
				if(system_state==2){
					disbuf[2]=number[eng_tmp/10];
					disbuf[3]=number[eng_tmp%10];
				}else{
					if(eng_tmp==0){
						system_state=2;
						if(func==1){
							FAN_SW=1;
							cmd_448k(0x2203,1);
						}
						if(min==0){
							min=DEF_MIN;
							disbuf[0]=number[min/10];
							disbuf[1] &= 0x04;
							disbuf[1] |= number[min%10];
						}
					}
				}
				//TM1637_update(disbuf);
				/*测试ADC值用
				temp_Dis[0]=ADC_ENG_Value/1000+0x30;
				temp_Dis[1]=ADC_ENG_Value%1000/100+0x30;
				temp_Dis[2]=ADC_ENG_Value%100/10+0x30;
				temp_Dis[3]=ADC_ENG_Value%10+0x30;
				lcd_pos(6,2);
				wr_string(temp_Dis);
				temp_Dis[0]=ADC_MOD_Value/1000+0x30;
				temp_Dis[1]=ADC_MOD_Value%1000/100+0x30;
				temp_Dis[2]=ADC_MOD_Value%100/10+0x30;
				temp_Dis[3]=ADC_MOD_Value%10+0x30;
				lcd_pos(12,2);
				wr_string(temp_Dis);
				*/
				#if 0
				if(checkCardState==0){
					checkCardFlag=RC522_Handel();
					if(checkCardFlag==1){
						checkCardState=1;
						card_Times=(RFID[1]<<8)|RFID[2];
						//card_Times = 0x01ff;                          //限9位
						times_DisTemp[0]=card_Times/100+0x30;
						times_DisTemp[1]=card_Times%100/10+0x30;
						times_DisTemp[2]=card_Times%10+0x30;
						lcd_pos(13,2);
						wr_string(times_DisTemp);
						min=RFID[0];
						sec=0;
						time_DisTemp[0]	= min/10+0x30;
						time_DisTemp[1]	= min%10+0x30;
						time_DisTemp[3]	= sec/10+0x30;
						time_DisTemp[4]	= sec%10+0x30;
						lcd_pos(6,2);
						wr_string(time_DisTemp);
						keyVoiceFlag=KEY_NOMAL_VOICE;
						// JDQ=0;
						HOT_SW=0;
					}else if(checkCardFlag==0x0f){
						checkCardState=0;
						//keyVoiceFlag=KEY_WARM_VOICE;
					}else if(checkCardFlag==0x02){                     //无次数
						checkCardState=2;
						times_DisTemp[0]='H';
						times_DisTemp[1]='H';
						times_DisTemp[2]='H';
						lcd_pos(13,2);
						wr_string(times_DisTemp);
						min=RFID[0];
						sec=0;
						time_DisTemp[0]	= min/10+0x30;
						time_DisTemp[1]	= min%10+0x30;
						time_DisTemp[3]	= sec/10+0x30;
						time_DisTemp[4]	= sec%10+0x30;
						lcd_pos(6,2);
						wr_string(time_DisTemp);
						keyVoiceFlag=KEY_NOMAL_VOICE;
						//JDQ=0;
						HOT_SW=0;
					}
				}
				#endif
			}
		}else{
			if(t500msFlag){
				t500msFlag=0;
				if(flash_flag){
					disbuf[0]=number[13];
					disbuf[1]=number[0];
					disbuf[2]=number[13];
					disbuf[3]=number[0];
					flash_flag=0;
				}else{
					disbuf[0]=0;
					disbuf[1]=0;
					disbuf[2]=0;
					disbuf[3]=0;
					flash_flag=1;
				}
			}
		}
		if(Rx1EndFlag){
			Rx1EndFlag=0;
			if(S1RX_BUFFER[0]==0xf0){      //机器码获取
				check_sum=0;
				for(keyValue=0;keyValue<8;keyValue++){
					check_sum += S1RX_BUFFER[keyValue];
				}
				if(check_sum==S1RX_BUFFER[8]){
					key_code_tmp2=0;
					for(keyValue=0;keyValue<6;keyValue++){
						pass_id_tbl[keyValue] = S1RX_BUFFER[keyValue+2];
						key_code_tmp2 ^= pass_id_tbl[keyValue];
					}
					key_code=key_code_tmp1+key_code_tmp2;
					S1TX_BUFFER[0]=0xf0;
					S1TX_BUFFER[1]=6;
					check_sum=0xf6;
					for(keyValue=0;keyValue<6;keyValue++){
						check_sum += chip_id_tbl[keyValue];
						S1TX_BUFFER[keyValue+2] = chip_id_tbl[keyValue];
					}
					S1TX_BUFFER[8]=check_sum;
					//S1TX_BUFFER[9]=key_code;
					back_app(S1TX_BUFFER);
				}
			}else if(S1RX_BUFFER[0]==0xf1){//支付信息下发
				check_sum = 0xf1+S1RX_BUFFER[1];
				for(keyValue=2;keyValue<8;keyValue++){
					S1RX_BUFFER[keyValue] += key_code;
					check_sum += S1RX_BUFFER[keyValue];
				}
				if(check_sum==S1RX_BUFFER[8]){
					work_times=(S1RX_BUFFER[2]<<8)|S1RX_BUFFER[3];
					work_time =(S1RX_BUFFER[4]<<8)|S1RX_BUFFER[5];
					S1TX_BUFFER[0]=0xf1;
					S1TX_BUFFER[1]=6;
					check_sum=0xf7;
					for(keyValue=2;keyValue<8;keyValue++){
						check_sum += S1RX_BUFFER[keyValue];
						S1TX_BUFFER[keyValue] = S1RX_BUFFER[keyValue]-key_code;
					}
					S1TX_BUFFER[8]=check_sum;
					back_app(S1TX_BUFFER);
					IapEraseSector(0);
					IapWriteByte(0,0x5a);
					IapWriteByte(1,0xa5);
					IapWriteByte(2,work_times>>8);
					IapWriteByte(3,work_times);
					IapWriteByte(4,work_time>>8);
					IapWriteByte(5,work_time);
					times_display_sub(3);
				}
			}else if(S1RX_BUFFER[0]==0xf2){//支付信息读取
				check_sum = 0xf2+S1RX_BUFFER[1];
				for(keyValue=2;keyValue<8;keyValue++){
					S1RX_BUFFER[keyValue] += key_code;
					check_sum += S1RX_BUFFER[keyValue];
				}
				if(check_sum==S1RX_BUFFER[8]){
					S1TX_BUFFER[0]=0xf2;
					S1TX_BUFFER[1]=6;
					check_sum=0xf8;
					S1TX_BUFFER[2]=work_times>>8;
					S1TX_BUFFER[3]=work_times;
					S1TX_BUFFER[4]=work_time>>8;
					S1TX_BUFFER[5]=work_time;
					for(keyValue=2;keyValue<8;keyValue++){
						check_sum += S1TX_BUFFER[keyValue];
						S1TX_BUFFER[keyValue] = S1TX_BUFFER[keyValue]-key_code;
					}
					S1TX_BUFFER[8]=check_sum;
					back_app(S1TX_BUFFER);
				}
			}
		}
		if(t100msFlag1){              //定时刷新
			t100msFlag1=0;
			TM1637_update(disbuf);
			//测试打印
			//printf("adc_avg=%hu\r\n",adc_avg);
		}
		keyVoice();
	}
}

void times_display_sub(int8u dt)
{
	times_dis_delay=dt;
	disbuf[0]=number[work_times%10000/1000];
	disbuf[1]=number[work_times%1000/100];
	disbuf[2]=number[work_times%100/10];
	disbuf[3]=number[work_times%10];
	secFlag2=0;
	TM1637_update(disbuf);
	do{
		if(secFlag2){
			secFlag2=0;
			times_dis_delay--;
		}
		keyVoice();
	}while(times_dis_delay);
	if(work_times==0){ //无使用次数了
		sys_chg_flag=0;
		disbuf[0]=number[13];
		disbuf[1]=number[0];
		disbuf[2]=number[13];
		disbuf[3]=number[0];
		flash_flag=0;
	}else{
		sys_chg_flag=1;
	}
}
void sys_init(void)
{
    //JDQ=0;
	BZPORT=BZOFF;
	RF_SEL=0;
    HOT_SW=0;
	FAN_SW=0;
    system_state=0;
    checkCardState=0;
	BIO_P1=0;
	BIO_P2=0;
	BIO_PWM=0;
	LED_P1=0;
	LED_P2=0;
	LED_P3=0;
	LED_N1=0;
	LED_N2=0;
	LED_N3=0;
	LED_N4=0;
	RF_SW=1;
	P36=1;
	P37=1;
	
	P0M1 = B1111_0000;
	P0M0 = B0000_1111;
	
	P1M1 = B0010_1100;
	P1M0 = B0001_0010;
	
	P2M1 = B0000_0000;
	P2M0 = B0000_1100;
	
	P3M1 = B0000_0000;
	P3M0 = B0001_0100;
	
	P4M1 = B1111_1111;
	P4M0 = B0000_0000;

	P5M1 = B1110_1111;
	P5M0 = B0001_0000;
	
	func=0;
	test_delay=1;
}

void disInit(void)
{
	//默认func=0;
}

void timeDeal(void)
{
	if(system_state==2){
        if(secFlag1){        
            secFlag1=0; 
            if(sec<60){
                sec++;
            }else{
                sec=0;
                if(min>1){
                    min--;
                    disbuf[0]=number[min/10];
                    disbuf[1] &= 0x04;
                    disbuf[1] |= number[min%10];
					if(min==DEC_MIN){
						if(work_times>0){
							work_times--;
							IapEraseSector(0);
							IapWriteByte(0,0x5a);
							IapWriteByte(1,0xa5);
							IapWriteByte(2,work_times>>8);
							IapWriteByte(3,work_times);
							IapWriteByte(4,work_time>>8);
							IapWriteByte(5,work_time);
						}
					}
                }else{
                    system_state=0;
					fans_delay=90;
                    //checkCardState=0;
					rf_state_update(STOP_STA);
                    HOT_SW=0;
                    sec=0;
                    min=0;
                    disbuf[0]=number[11];
                    disbuf[1]=number[11];
                    disbuf[2]=number[12];
                    disbuf[3]=number[12];
                    //disbuf[4]=0;
                    //disbuf[5]=0;
                    TM1637_update(disbuf);
                    keyVoiceFlag=SYS_OVER_VOICE;
					cmd_448k(0x2203,0);
                    return;
                }
            }
        }
        if(t500msFlag){
            t500msFlag=0;
            disbuf[1] ^= 0x04;
            //测试adc代码开始
            //disbuf[0]=number[adc_avg/1000];
            //disbuf[1] &= 0x04;
            //disbuf[1] |= number[adc_avg%1000/100];
            //disbuf[2]=number[adc_avg%100/10];
            //disbuf[3]=number[adc_avg%10];
            //测试adc代码结束
            //TM1637_update(disbuf);
        }
    }
}
