#include<reg52.h>
#include <intrins.h>
#include "delay.h"
#include"lcd_1602.h"
#include <intrins.h>




char local_date=0,base_date=0;//全局变量，当前箭头位置



/***********************************************
函数名称：Delay_us
功    能：STC 1T单片机1us延时程序
入口参数：us:延时的微秒数
返 回 值：无	
备    注：内部时钟11.0592MHz
************************************************/
void Delayus(unsigned int us)
{
  	while( us--)
   	{
    	_nop_(); 
   	}             
}
/***********************************************
函数名称：Delay_ms
功    能：STC 1T单片机1ms延时程序
入口参数：ms:延时的毫秒数
返 回 值：无	
备    注：示波器实测1.05ms 内部时钟11.0592MHz
************************************************/
void Delayms(unsigned int ms)
{
  	unsigned int i;
  	while( (ms--) != 0)
   	{
    	for(i = 0; i < 600; i++); 
   	}             
}
/*************************************
* 函 数 名: LCD1602_WriteCMD
* 函数功能: 向lcd1602写指令
* 入口参数: cmd:指令
* 返    回: 无
* 备    注：无
**************************************/
void LCD1602_WriteCMD(unsigned char cmd)
{
  	EN=0;
  	RS=0;
  	RW=0;
  	Delayus(10);
  	EN=1; 
  	Delayus(10);
  	DataPort=cmd;
  	Delayus(10);
  	EN=0;
}
/***********************************************
函数名称：LCD1602_WriteDAT
功    能：向lcd1602写数据
入口参数：dat：数据
返 回 值：无	
备    注：无
************************************************/
void LCD1602_WriteDAT(unsigned char dat)
{
  	EN=0;
  	RS=1;
  	RW=0;
  	Delayus(10);
  	EN=1; 
  	Delayus(10);
  	DataPort=dat;
  	Delayus(10);
  	EN=0;
}
/***********************************************
函数名称：LCD1602_CheckBusy
功    能：检测lcd1602忙信号
入口参数：无
返 回 值：无	
备    注：无
************************************************/
void LCD1602_CheckBusy(void)
{
  unsigned char temp;
	DataPort=0xff;		  //做输入先置高,12c系列单片机需转换端口模式
  	while(1)
  	{    
    	EN=0;			   
    	RS=0;
    	RW=1;
    	Delayus(10);
    	EN=1;
    	Delayus(10);
    	temp=DataPort;    //读取忙通道数据
    	Delayus(10);
    	EN=0;
    	if((temp&0x80)!=0x80)
    	{    
				break;
    	}
  	}
}
 /***********************************************
函数名称：LCD1602_Init
功    能：LCD1602初始化
入口参数：无
返 回 值：无	
备    注：详见数据手册定义。
************************************************/
void LCD1602_Init(void)  
{
  	Delayms(15);      		//上电延时15ms
  	LCD1602_WriteCMD(0x38); //写显示指令(不检测忙信号)
  	Delayms(5);
  	LCD1602_WriteCMD(0x38); //写显示指令(不检测忙信号)
  	Delayms(5);
  	LCD1602_WriteCMD(0x38); //写显示指令(不检测忙信号)
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x38); //写显示指令
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x08); //显示关闭
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x01); //清屏
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x06); //显示光标移动设置
  	LCD1602_CheckBusy();
  	LCD1602_WriteCMD(0x0c); //显示开及光标设置  
}
 /***********************************************
函数名称：LCD1602_Display
功    能：写多字节字符
入口参数：addr起始地址，pointer指针地址，index下标，num个数
返 回 值：无	
备    注：无
************************************************/
 void LCD1602_Display(unsigned char addr,unsigned char* pointer,unsigned char index,unsigned char num)
{
  	unsigned char i;
  	LCD1602_CheckBusy();	//判断忙信号
  	LCD1602_WriteCMD(addr);	//写入地址
  	for(i=0;i<num;i++)		//写入数据
  	{
     	LCD1602_CheckBusy();			   //判断忙信号
     	LCD1602_WriteDAT(pointer[index+i]);//写入数据     
  	}
}
