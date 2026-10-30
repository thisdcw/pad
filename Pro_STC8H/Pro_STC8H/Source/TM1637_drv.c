
#include "TM1637_drv.h"

void Delay2us()		//@11.0592MHz  STC15Fxx
{
	unsigned char i;
	
	i = 3;
	while (--i);
}

/**
  * 函数功能: 发送开始信号.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无 
  */
void TM1637_Start(void)
{
    CLK(1);
	DIO(1);
	Delay2us();
	Delay2us();
	DIO(0);
	Delay2us();
	Delay2us();
    CLK(0);	
}
/**
  * 函数功能: 发送结束信号.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无 
  */
void TM1637_Stop(void)
{
    CLK(0);
	Delay2us();
	DIO(0);
	Delay2us();
	CLK(1);
	Delay2us();
	Delay2us();
	DIO(1);
	Delay2us();
	Delay2us();
	//DIO(0);
}

uint8_t TM1637_Wait_Ack(void)
{
    uint8_t time_count = 0;
	uint8_t ack_number = 0;  //读取到底电平次数
	uint8_t ack_flag = 0;    //0:未收到ACK，1:接收到ACK
    //DIO_Mode_Input();
	
	CLK(0);  //拉低时钟（第8个时钟下降沿）
 
	while(time_count < 6)
	{
		Delay2us();
		time_count++;
	    if(DIO_IN == 0)
		{
		    ack_number++;		
		}
	    if(ack_number > 3)  //读取到4次低电平认为获取到了ACK
		{
            ack_flag = 1;
		}
	}
	CLK(1);
	Delay2us();
	CLK(0);  //拉低时钟（第9个时钟下降沿）释放总线

	if(ack_flag)
	    return HAL_OK;
	else
        return HAL_ERROR;
}

//写TM1637，从低位开始传送
void TM1637_Write(uint8_t dat)
{
    uint8_t i;

    for(i = 0; i < 8; i++){
	    CLK(0);
		Delay2us();  //时钟拉低
		
		/* 输出 */
	    if(dat & 0x01)  //该位为1
		    DIO(1);
		else  //该位为0
		    DIO(0);	
	
		Delay2us();
		dat  >>= 1;  //先发低位
		CLK(1);
		Delay2us();  //时钟拉高
	}
 
	/* 获取ACK应答 */
	if(TM1637_Wait_Ack() != HAL_OK){
        
	    //printf("TM1637发送错误!\r\n"); 
    }        
}

code uint8_t number[] = 
{
    //0    1    2    3    4    5    6    7   8    9   NULL   -   F    E
    0xeb,0x82,0xd9,0xda,0xb2,0x7a,0x7b,0xc2,0xFb,0xFa,0x00,0x10,0x71,0x79
};


void TM1637_Init(void)
{
	TM1637_Start();
	TM1637_Write(0x87);  //显示控制--关闭显示
	TM1637_Stop();
}



void TM1637_Test(void)
{
	/* 数码管显示 */
	TM1637_Start();
	TM1637_Write(0x40);  //数据控制（0x44--固定地址，0x40--地址自增），固定地址需另TM1637_Stop();
	 
	TM1637_Start();
	TM1637_Write(0xC0);    //地址控制（0xC0--C3:数码管1-4）
	TM1637_Write(0);  //data
	TM1637_Write(0);  //data
	TM1637_Write(0);  //data
	TM1637_Write(0);  //data
	TM1637_Write(0);  //data
	TM1637_Write(0);  //data
	TM1637_Stop();
	 
	TM1637_Start();
	TM1637_Write(0x8F);  //显示控制(0x88/89/8B/8C/8D/8E/8F亮度从低到高，0x87关闭显示)
	TM1637_Stop();
}

void TM1637_update(int8u *buf)
{
    TM1637_Start();
	TM1637_Write(0xC0);    //地址控制（0xC0--C3:数码管1-4）
	TM1637_Write(buf[0]);  //data
	TM1637_Write(buf[1]);  //data
	TM1637_Write(buf[2]);  //data
	TM1637_Write(buf[3]);  //data
	TM1637_Write(buf[4]);  //data
	TM1637_Write(buf[5]);  //data
	TM1637_Stop();
}



