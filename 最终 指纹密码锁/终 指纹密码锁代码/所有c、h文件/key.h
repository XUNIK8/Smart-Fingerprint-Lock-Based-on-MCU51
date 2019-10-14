#ifndef _KEY_H_
#define _KEY_H_
#include<reg52.h>


//定义按键输入端口
sbit KEY1=P1^0;
sbit KEY2=P1^1;
sbit KEY3=P1^2;
sbit KEY4=P1^3;
sbit KEYOK=P1^4;
sbit KEYCANCEL=P1^5;

void Key_Init(void);


#endif
