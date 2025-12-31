/*
Tseditor.c 
C Source file for C51 on STC chips.
Author:Some More
Date:2025-12-31
Copyright: (C) 2020-2025, Memory Dust Press(M.D.P.)All Rights Reserved.
*/
#include "Tseditor.h"
void Delay_ms(unsigned int xms)	//@11.0592MHz
{
	unsigned char data i, j;
	while(xms--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}
uint32_t next=0;
void seed(uint32_t s){
	next=s;
}
uint32_t rand(){
	next= next * 1103515245 + 12345;
	return ((unsigned)(next / 65536) % 32768);
}
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#define Sreial_available RI
void Serial_Init(void)	//9600bps@11.0592MHz on UART1
{
	PCON &= 0x7F;		//波特率不倍速
//	SCON = 0x50;		//8位数据,可变波特率 0101 0000
	SCON = 0x40;		//8位数据,可变波特率 0100 0000
//	AUXR &= 0xBF;		//定时器时钟12T模式
//	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式 0010 0000
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	//ES = 1;				//使能串口1中断
	//EA = 1;
}
void Serial_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
void Serial_SendString(uint8_t *s)
{
	char i=0;
	while(s[i]){
	Serial_SendByte(s[i]);
	i++;
	}
}

/*MAIN*/
int main(void)
{
  setup();
  for(;;){
    loop();
  }
  return 0;
}