#include"key.h"
#include<reg52.h>
#include"delay.h"

/****°´¼ü³õÊ¼»¯****/
void Key_Init(void)
{ 
 KEY1=1;
 KEY2=1;
 KEY3=1;
 KEY4=1;
 KEYOK=1;
 KEYCANCEL=1;
}

