#ifndef __C51_LCD12864_H
#define __C51_LCD12864_H
#include "../Tseditor/Tseditor.h"

void lcd12864_init();    //初始化12864
void lcd12864_clear();   //12864清屏
void lcd12864_show_char(uint8 x, uint8 y, uint8 sig);   //12864在指定位置显示一个字符
void lcd12864_show_string(uint8 x, uint8 y, uint8 *str);    //12864在指定位置显示字符串
void lcd12864_show_num(uint8 x, uint8 y, int num);   //12864在指定位置显示数字
void lcd12864_show_float(uint8 x, uint8 y, float num, uint8 precision); //显示小数
void lcd12864_show_bmp(uint8 *p);    //12864显示图片

#endif
