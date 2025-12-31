#ifndef __DELAY_H
#define __DELAY_H

#include "../Tseditor/Tseditor.h"

#ifndef SYS_CLK 
#define SYS_CLK 11059200L
#endif

void delay(uint16 time);	//延迟time毫秒

void delay1us();			//延迟1微秒
void delay5us();			//延迟5微秒
void delay10us();			//延迟10微秒
void delay50us();			//延迟50微秒
void delay100us();			//延迟100微秒
void delay1ms();			//延迟1毫秒

#endif
