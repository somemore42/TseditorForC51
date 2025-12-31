#include "../Tseditor/Tseditor.h"
#include "Delay.h"
// IO引脚定义
#ifndef LCD12864_RS
#define LCD12864_RS 		    P2_6
#endif

#ifndef LCD12864_RW
#define LCD12864_RW 		    P2_5
#endif

#ifndef LCD12864_EN
#define LCD12864_EN 		    P2_7
#endif

// #ifndef LCD12864_RST
// #define LCD12864_RST             P1_3
// #endif

#ifndef LCD12864_DATA
#define LCD12864_DATA            P0
#endif
//========================================================================
// 描述: lcd12864测忙碌
// 参数: none.
// 返回: 0，空闲，可写指令与数据; 非0, 忙，等待; .
//========================================================================
uint8 lcd12864_chek_busy()
{ 
    uint8 temp1;//状态信息（判断是否忙） 
    LCD12864_RS = 0;
    LCD12864_RW = 1;
    LCD12864_EN = 1;
    temp1 =(uint8)(LCD12864_DATA&0x80);
    LCD12864_EN = 0;
    return temp1;
}

//========================================================================
// 描述: lcd12864写命令
// 参数: cmd:写入的命令.
// 返回: none.
//========================================================================
void lcd12864_write_command(uint8 cmd)/*写命令*/
{ 
    while(lcd12864_chek_busy());
    LCD12864_RS = 0;
    LCD12864_RW = 0;
    LCD12864_EN = 0;
    LCD12864_DATA = cmd;
    delay50us();
    LCD12864_EN = 1;
    delay50us();
    LCD12864_EN = 0;
}

//========================================================================
// 描述: lcd12864写数据
// 参数: dat:写入的数据.
// 返回: none.
//========================================================================
void lcd12864_write_data(uint8 dat)
{ 
    while(lcd12864_chek_busy());
    LCD12864_RS = 1;
    LCD12864_RW = 0;
    LCD12864_EN = 0;
    LCD12864_DATA = dat;
    delay50us();
    LCD12864_EN = 1;
    delay50us();
    LCD12864_EN = 0;
}

//========================================================================
// 描述: lcd12864读数据
// 参数: none
// 返回: 读取到的数据.
//========================================================================
uint8 lcd12864_read_data()
{ 
    uint8 read;
    while(lcd12864_chek_busy());
    LCD12864_RS = 1;
    LCD12864_RW = 1;
    LCD12864_EN = 0;
    LCD12864_DATA = 0xff;
    // delay50us();
    LCD12864_EN = 1;
    delay50us();
    read = LCD12864_DATA;
    LCD12864_EN = 0;
    // delay50us();
    return read;
}


//========================================================================
// 描述: lcd12864初始化
// 参数: none.
// 返回: none.
//========================================================================
void lcd12864_init()
{ 
    // LCD12864_RST = 1;  
    // LCD12864_RST = 0;             
    // LCD12864_RST = 1;                //复位RST=1 
    lcd12864_write_command(0x34);         //34H--扩充指令操作
    delay5us();
    lcd12864_write_command(0x30);         //功能设置，一次送8位数据，基本指令集
    delay5us();
    lcd12864_write_command(0x0C);         //显示状态开关：整体显示开，光标显示关，光标显示反白关
    delay5us();
    lcd12864_write_command(0x01);         //清DDRAM
    delay5us();      
    lcd12864_write_command(0x02);         //DDRAM地址归位
    delay5us(); 
    lcd12864_write_command(0x80);         //设定DDRAM 7位地址000，0000到地址计数器AC
    delay5us();
}

//========================================================================
// 描述: lcd12864设定显示位置
// 参数: X,Y:坐标.
// 返回: none.
//========================================================================
void lcd12864_display_pos(uint8 X,uint8 Y)
{                          
   uint8  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd12864_write_command(pos);     //显示地址
}

//========================================================================
// 描述: lcd12864清屏
// 参数: none.
// 返回: none.
//========================================================================
void lcd12864_clear()
{
    lcd12864_write_command(0x30);//
    lcd12864_write_command(0x01);//清除显示
    delay(1);
}

//========================================================================
// 描述: lcd12864写单个字符(使用内部字库)
// 参数: X,Y;坐标; sig:要显示的字符.
// 返回: none.
//========================================================================
void lcd12864_show_char(uint8 x, uint8 y, uint8 sig)
{    
    lcd12864_display_pos(x,y); 
    lcd12864_write_data(sig);             //输出单个字符
}

//========================================================================
// 描述: lcd12864写字符串(使用内部字库)
// 参数: X(0~3)行,Y(0~7)个字符; str:要显示的字符串;
// 返回: none.
//========================================================================
void lcd12864_show_string(uint8 x, uint8 y, uint8 *str)
{ 
    uint8 i = 0;
    lcd12864_display_pos(x,y);
    while(str[i] != '\0')
    {
        lcd12864_write_data(str[i]);
        i++;
    }
}
/*FIX for _itoa*/
void _itoa(int num,char* str,char radix){
  const char index[40] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned unum; char i = 0, j, k; char temp;
		if (radix == 10 && num < 0) { unum = (unsigned)-num; str[i++] = '-'; }
		else unum = (unsigned)num; do { str[i++] = index[unum % (unsigned)radix]; unum /= radix; } while (unum);
		str[i] = '\0'; if (str[0] == '-') k = 1; else k = 0;
		for (j = k; j <= (i - 1) / 2; j++) { temp = str[j]; str[j] = str[i - 1 + k - j]; str[i - 1 + k - j] = temp; }
}
//========================================================================
// 描述: lcd12864写数字(使用内部字库)
// 参数: X(0~3)行,Y(0~7)个字符; num:要显示的数字;
// 返回: none.
//========================================================================
void lcd12864_show_num(uint8 x, uint8 y, int num)
{
	char men[15];
    _itoa(num,men,10);
	lcd12864_show_string(x,y,men);
} 

//========================================================================
// 描述: 显示小数(使用内部字库)
// 参数: x,y:起点坐标;num:要显示的数值;precision:精度.
// 返回: none.
//========================================================================
void lcd12864_show_float(uint8 x, uint8 y, float num, uint8 precision)
{
    char men[15];
    uint8 i;
    uint8 float_bit;
    int16 int_part;
	int_part = (int16)num/1;
    lcd12864_display_pos(x,y);
    
    if(num >= 0)			//整数部分
    {
        _itoa(int_part,men,10);
		
        for(i=1;i<10;i++)
        {
            int_part=int_part/10;
            if(int_part == 0)
            {
                break;
            }
        }
        men[i] = '.';
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+1],10);
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+1],10);
				float_bit = (int32)(num*100)%10;
				_itoa(float_bit,&men[i+2],10);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+1],10);
				float_bit = (int32)(num*100)%10;
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*1000)%10;
				_itoa(float_bit,&men[i+3],10);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				_itoa(float_bit,&men[i+1],10);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				_itoa(float_bit,&men[i+3],10);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				_itoa(float_bit,&men[i+4],10);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+1],10);
			break;//保留1位精度
		}

        i = 0;
        while(men[i] != '\0')
        {
            lcd12864_write_data(men[i]);
            i++;
        }
    }
    else
    {
        if(int_part == 0){
            men[0] = '-';
            men[1] = '0';
            i = 1;
        }else{
            _itoa(int_part,men,10);
            for(i=1;i<10;i++)
            {
                int_part=int_part/10;
                if(int_part == 0)
                {
                    break;
                }
            }
        }
        num = -num;
        men[i+1] = '.';
		switch(precision)
		{
			case 1:									//保留1位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+2],10);
			break;
			case 2:									//保留2位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*100)%10;
				_itoa(float_bit,&men[i+3],10);
			break;
			case 3:									   //保留3位精度
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*100)%10;
				_itoa(float_bit,&men[i+3],10);
				float_bit = (int32)(num*1000)%10;
				_itoa(float_bit,&men[i+4],10);
			break;
			case 4:										//保留4位精度
				float_bit = (int32)(num*10)%10;			//显示小数部分第一位
				_itoa(float_bit,&men[i+2],10);
				float_bit = (int32)(num*100)%10;		//显示小数部分第二位
				_itoa(float_bit,&men[i+3],10);
				float_bit = (int32)(num*1000)%10;		//显示小数部分第三位
				_itoa(float_bit,&men[i+4],10);
				float_bit = (int32)(num*10000)%10;		//显示小数部分第四位
				_itoa(float_bit,&men[i+5],10);
			break;
			default:
				float_bit = (int32)(num*10)%10;
				_itoa(float_bit,&men[i+2],10);
			break;//保留1位精度
		}
        i = 0;
        while(men[i] != '\0')
        {
            lcd12864_write_data(men[i]);
            i++;
        }
    }
} 

//========================================================================
// 描述: lcd12864显示图像
// 参数: p:要显示的图像.
// 返回: none.
//========================================================================
void lcd12864_show_bmp(uint8 *p)
{
    int16 ygroup,x,y,i;
    int16 temp;
    int16 tmp;
             
    for(ygroup=0;ygroup<64;ygroup++)           
    {                   
        if(ygroup<32)                   //写入液晶上半图象部分
        {
            x=0x80;
            y=ygroup+0x80;
        }else 
        {
            x=0x88;
            y=ygroup-32+0x80;    
        }         
        lcd12864_write_command(0x34);        //写入扩充指令命令
        lcd12864_write_command(y);           //写入y轴坐标
        lcd12864_write_command(x);           //写入x轴坐标
        lcd12864_write_command(0x30);        //写入基本指令命令
        tmp=ygroup*16;
        for(i=0;i<16;i++)
		{
		    temp=p[tmp++];
		    lcd12864_write_data(temp);
        }
    }
    lcd12864_write_command(0x34);        //写入扩充指令命令
    lcd12864_write_command(0x36);        //显示图象
}
