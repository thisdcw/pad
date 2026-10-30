
#include "108-DS1302.h"

//DS1302写一个字节
void DS1302_Write(int8u add, int8u Dat)
{
	int8u i;
	
	DS1302_CLK=0;        //ckl拉低，很重要
	DS1302_RST=1;        //RST拉高
	add &= 0xfe;         //地址LSB设置为0
	
	for(i=0;i<8;i++){    //写地址，上升沿有效      
		DS1302_CLK=0;    //ckl拉低
		if(add&0x01){
			DS1302_DAT=1;
		}else{
			DS1302_DAT=0;
		}
		DS1302_CLK=1;    //ckl拉高
		add >>= 1;
	}
	
	for(i=0;i<8;i++){    //写数据，上升沿有效      
		DS1302_CLK=0;    //ckl拉低
		if(Dat&0x01){
			DS1302_DAT=1;
		}else{
			DS1302_DAT=0;
		}
		DS1302_CLK=1;    //ckl拉高
		Dat >>= 1;
	}
	DS1302_RST=0;        //RST拉低，结束写
}


//DS1302 读一个字节
int8u DS1302_Read(int8u add)
{
	int8u i,Dat;
	DS1302_CLK=0;        //ckl拉低，很重要
	DS1302_RST=1;        //RST拉高
	add |= 0x01;         //地址LSB设置为1
	
	for(i=0;i<8;i++){    //写地址，上升沿有效      
		DS1302_CLK=0;    //ckl拉低
		if(add&0x01){
			DS1302_DAT=1;
		}else{
			DS1302_DAT=0;
		}
		DS1302_CLK=1;    //ckl拉高
		add >>= 1;
	}
	
	for(i=0;i<8;i++){    //读数据，上升沿有效      
		DS1302_CLK=0;    //ckl拉低
        Dat >>= 1;
		if(DS1302_DAT){
			Dat |= 0x80;
		}else{
			Dat &= 0x7f;
		}
		DS1302_CLK=1;    //ckl拉高
	}
	DS1302_RST=0;        //RST拉低，结束读
	return Dat;
}

void read_nowtime(unsigned char addr, unsigned char kn[], unsigned char k)
{
  int8u n;
  int8u b;
  for (n=0;n<k;n++)
  {
    b=DS1302_Read(addr);
	if(b!=0xff)
    kn[n] = b;
    addr = addr + 2;
  }
}

void init_ds1302()
{
  DS1302_RST = 0; 
  DS1302_CLK = 0;
  DS1302_Write(YLB_CTRL, UNLOCK); 
  DS1302_Write(YLB_SEC, 0x00); 
  DS1302_Write(YLB_CHRG, CHG_LV4); 
  DS1302_Write(YLB_CTRL, LOCK); 
}

void write_settime( unsigned char *p)
{
	DS1302_Write(YLB_CTRL,UNLOCK); 
	DS1302_Write(YLB_MIN,   p[1]);
	DS1302_Write(YLB_HOUR,  p[2]);
	DS1302_Write(YLB_DAY,   p[3]);
	DS1302_Write(YLB_MONTH, p[4]);
	DS1302_Write(YLB_YEAR,  p[6]);
	DS1302_Write(YLB_CHRG, CHG_LV4); 
	DS1302_Write(YLB_CTRL,  LOCK);
}