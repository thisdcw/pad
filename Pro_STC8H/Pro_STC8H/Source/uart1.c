#include "uart1.h"
#include "stdio.h"			// 串口重定向需要添加头文件stdio.h

      int8u S1RX_Status;        //Rx state
xdata int8u S1RX_Count;
xdata int8u S1RX_BUFFER[10];     //
xdata int8u S1TX_BUFFER[20];     //
xdata int8u Tx1EndFlag;
xdata int8u Rx1EndFlag;
void uart1_Init(void)
{
    //串口1初始
	SCON=0x50;                                //1开始位+8数据位+1停止位，波特率可变               
	P_SW1 &= 0x3f;                            //串口在P3.0和P3.1
	//P_SW1 |= 0x40;							  //串口在P3.6和P3.7
    T2L = (65536-(FOSC/4/BAUD_S1));           //设置波特率重装值
	T2H = (65536-(FOSC/4/BAUD_S1))>>8;
	AUXR |= 0x14;                             //T2为1T模式，并启动T2
    AUXR |= 0x01;                             //选择T2为波特率发生器
    ES=1; 									  //允许串口中断
	Tx1EndFlag=0;
	Rx1EndFlag=0;
	S1RX_Status=0;
}

char putchar(char c)
{
	SBUF = c;
	Tx1EndFlag=1;
	while(Tx1EndFlag);
	return c;
}


void UART1_Transmit(int8u tdata)
{
    SBUF=tdata;
    Tx1EndFlag=1;
    while(Tx1EndFlag);
}

//5A A5 06 83 2000 01 XXXX
void cmd_448k(int16u add,int8u value)
{
	P_SW1 |= 0x40;							  //串口在P3.6和P3.7
	UART1_Transmit(0x5a);
	UART1_Transmit(0xa5);
	UART1_Transmit(0x06);
	UART1_Transmit(0x83);
	UART1_Transmit(add>>8);
	UART1_Transmit(add);
	UART1_Transmit(0x01);
	UART1_Transmit(0x00);
	UART1_Transmit(value);
	P_SW1 &= 0x3f;                            //串口在P3.0和P3.1
}

//#define srv_send(s,c) {(s)+=(c);UART1_Transmit(c);}

void back_app(int8u* buf)
{
	int8u i;
	UART1_Transmit(0xbf);
	UART1_Transmit(0xfb);
	for(i=0;i<9;i++){
		UART1_Transmit(buf[i]);
	}
}

void Rx1_Data(void) interrupt 4 
{
    volatile int8u tmp;
    if(TI){                 //发送字节完成
        TI=0;
        Tx1EndFlag=0;
    }
    if(RI){
        RI=0;
        tmp=SBUF;
		switch(S1RX_Status){
		case 0:
			if(tmp==0xbf){
				S1RX_Status=1;
			}else{
				S1RX_Status=0;
			}
			break;
		case 1:
			if(tmp==0xfb){
				S1RX_Status=2;
			}else{
				S1RX_Status=0;
			}
			break;
		case 2:
			S1RX_BUFFER[0]=tmp;
			S1RX_Status=3;
			break;
		case 3:
			S1RX_BUFFER[1]=tmp;
			S1RX_Count=2;
			S1RX_Status=4;
			break;
		case 4:
			S1RX_BUFFER[S1RX_Count]=tmp;
			S1RX_Count++;
			if(S1RX_Count>=9){
				Rx1EndFlag=1;
				S1RX_Status=0;
			}
			break;
		default:
			S1RX_Status=0;
			break;
		}
    }
}