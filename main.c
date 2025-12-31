#include "Tseditor.h"
#include "LCD1602.h"

uint8_t NUMA=0;
uint8_t NUMB=0;
uint8_t NUMR=0;
void setup()
{
	LCD_Init();
	Serial_Init();
	LCD_ShowString(1,1,"Booting...");
	LCD_ShowString(2,1,"That Summer Gone");
	Delay_ms(499);
	Serial_SendString("OK");
}
void loop()
{
	if(P3_3==1){
	NUMA=P1;LCD_ShowNum(1,1,NUMA,4);
	NUMB=P2;LCD_ShowNum(1,7,NUMB,4);
	if(P3_4&&P3_5&&P3_6&&P3_7){NUMR=NUMA+NUMB;LCD_ShowChar(1,5,'+');}
	if(P3_4&&P3_5&&P3_6&&!P3_7){NUMR=NUMA-NUMB;LCD_ShowChar(1,5,'-');}
	if(P3_4&&P3_5&&!P3_6&&P3_7){NUMR=NUMA*NUMB;LCD_ShowChar(1,5,'x');}
	if(P3_4&&P3_5&&!P3_6&&!P3_7){NUMR=NUMA/NUMB;LCD_ShowChar(1,5,'/');}
	if(P3_4&&!P3_5&&P3_6&&P3_7){NUMR=NUMA%NUMB;LCD_ShowChar(1,5,'%');}
	if(P3_4&&!P3_5&&P3_6&&!P3_7){NUMR=NUMA<<NUMB;LCD_ShowChar(1,5,'<');}
	if(P3_4&&!P3_5&&!P3_6&&!P3_7){NUMR=NUMA>>NUMB;LCD_ShowChar(1,5,'>');}
	if(!P3_4&&P3_5&&P3_6&&P3_7){NUMR=NUMA^NUMB;LCD_ShowChar(1,5,'^');}
	if(!P3_4&&P3_5&&P3_6&&!P3_7){NUMR=~NUMA;LCD_ShowChar(1,5,'~');}
	//P0=NUMR;
	LCD_ShowChar(2,1,'=');
	
	LCD_ShowNum(2,2,NUMR,4);
  }
	while(P3_3==1){Delay_ms(1);Serial_SendByte('.');}
}
