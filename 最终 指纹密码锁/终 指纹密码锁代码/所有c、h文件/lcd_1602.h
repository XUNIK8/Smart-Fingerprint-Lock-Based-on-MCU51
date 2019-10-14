#ifndef __LCD__
#define __LCD__

#include "reg52.h"
extern char local_date,base_date;
extern unsigned char code *Main_Menu[];

//LCD1602操作位定义
#define 	DataPort  	P0
sbit 		RS 			= P2^7;     //寄存器选择 0:指令寄存器 1:数据寄存器
sbit 		RW 			= P2^6;     //读写控制 0：写  1：读
sbit 		EN 			= P2^5;     //读写数据使能   0：停止 1：启动

//1602屏地址定义
#define LINE1 0x80				   //第一行地址
#define LINE2 0xc0				   //第二行地址


void LCD1602_WriteCMD(unsigned char cmd);
void LCD1602_WriteDAT(unsigned char dat);
void LCD1602_CheckBusy(void);
void LCD1602_Init(void);
void LCD1602_Display(unsigned char addr,unsigned char* pointer,unsigned char index,unsigned char num);


#endif