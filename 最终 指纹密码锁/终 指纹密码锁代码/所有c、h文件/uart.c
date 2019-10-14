#include"uart.h"
#include<reg52.h>

void Uart_Init(void)
{
    SCON=0x50;   //UART方式1:8位UART;   REN=1:允许接收 
    PCON=0x00;   //SMOD=0:波特率不加倍 
    TMOD=0x20;   //T1方式2,用于UART波特率 
    TH1=0xFD; 
    TL1=0xFD;   //UART波特率设置:FDFD，9600;FFFF,57600
    TR1=1; //允许T1计数 
    EA=1;
	  ES=1;
}


void Uart_Send_Byte(unsigned char c)//UART Send a byte
{
	SBUF = c;
	while(!TI);		//发送完为1 
	TI = 0;
}

unsigned char Uart_Receive_Byte()//UART Receive a byteg
{	
	unsigned char dat;
	while(!RI);	 //接收完为1 
	RI = 0;
	dat = SBUF;
	return (dat);
}
