#include<reg52.h>
#include<intrins.h>
#include"buzz.h"
#include"key.h"
#include"uart.h"
#include"delay.h"
#include"FPM10A.h"
#include"lcd_1602.h"
#include"kaisuo.h"
#include"i2c.h"

unsigned int w=0;//设置四位密码的初始值
unsigned int x=0;
unsigned int y=0;
unsigned int z=0;
unsigned int find_fingerid;


/***确认密码函数***/
void confirmpassword()
{
	if(w==1&&x==2&&y==3&&z==4)
	{
    LCD1602_Display(0xc0,"  Suceeded ^_^  ",0,16);
		Delay_Ms(1000);
		Buzz_Times(1);
	  kaisuo();    
	}
	else
  {
    LCD1602_Display(0xc0,"  Failed QAQ    ",0,16);
		Delay_Ms(1000);
		Buzz_Times(3);
  }
	x=0;
	y=0;
	z=0;
	w=0;
}


/****密码到10变0函数****/
void change()
{
	if(w>9)
	{
		w-=10;
	}
	if(x>9)
	{
		x-=10;
	}
	if(y>9)
	{
		y-=10;
	}
	if(z>9)
	{
		z-=10;
	}	
}

/****第1.1界面的函数（输入密码解锁）****/
 void display11(void)
 {
	 LCD1602_WriteCMD(0x01);  //清屏
	 change();
	 LCD1602_Display(0x80,"   Input Code   ",0,16);	//Input Code
	 LCD1602_WriteCMD(0xc0+6);
	 LCD1602_WriteDAT(w+'0');
	 LCD1602_WriteDAT(x+'0');
	 LCD1602_WriteDAT(y+'0');
   LCD1602_WriteDAT(z+'0');
 }
 
 
 /****第1.3界面的函数（进入管理员模式、输入密码界面）****/
 void display13(void)
 { 
	 LCD1602_WriteCMD(0x01);  //清屏
	 change();
	 LCD1602_Display(0x80,"  Input ADCode  ",0,16);	//Input Code
	 LCD1602_WriteCMD(0xc0+6);
	 LCD1602_WriteDAT(w+'0');
	 LCD1602_WriteDAT(x+'0');
	 LCD1602_WriteDAT(y+'0');
	 LCD1602_WriteDAT(z+'0');

 }

 /****第1.3.1界面的函数（管理员密码正确后选择添加/删除指纹）****/
 void display131(void)
 {
	 LCD1602_WriteCMD(0x01);  //清屏 	
	 LCD1602_Display(0x80,"  ADMINISTRATOR ",0,16);	 //第一排显示搜索指纹
	 LCD1602_Display(0xc0,"  Add     Delete",0,16);	 //添加和删除指纹
 }



 
 //自己的main函数
void main()
{	
	LCD1602_Init();			//初始化液晶
	LCD1602_Display(0x80,"     By XHJ     ",0,16);	 //液晶开机显示界面
  Delay_Ms(500);
  Uart_Init();			//初始化串口
	Key_Init();				//初始化按键
 	Delay_Ms(500);          //延时500MS，等待指纹模块复位
	Device_Check();		   	//校对指纹模块是否接入正确，液晶做出相应的提示//
	Delay_Ms(1000);			//对接成功界面停留一定时间
	Buzz_Times(2);
	
	/******进入主要页面*****/
	begin:
	LCD1602_Display(0x80,"    WELCOME     ",0,16);  //第一排显示welcome
	LCD1602_Display(0xc0,"1.P 2.F 3.A 4.C  ",0,16);	 //1.Password 2.Finger 3.Administrator
	x=0; //初始化密码
	y=0;
	z=0;
	w=0;
	
	
while(1)
{
//第一个：输入密码解锁界面
	if(KEY1 == 0)
	{	
		Delay_Ms(10);//延迟10ms（空过抖动）   
    if (KEY1==0)        //判断是否还按着
		{ 
			Delay_Ms(200);
			display11();//执行命令
		}
		while(1)
		{ 
			if(KEY1==0)
			{	
				Delay_Ms(10);
				if(KEY1==0)
				{
					w+=1;
					Delay_Ms(200);
					display11(); //显示第1.1个界面
			  }
			}
			if(KEY2==0)
			{
				Delay_Ms(10);
				if(KEY2==0)
				{
					x+=1;				
					Delay_Ms(200);
					display11();
				}
			}
			if(KEY3==0)
			{		
				Delay_Ms(10);				
				if(KEY3==0)
				{
					y+=1;
					Delay_Ms(200);
					display11();
				}
			}
			if(KEY4==0)
			{			
				Delay_Ms(10);
				if(KEY4==0)
				{
					z+=1;
					Delay_Ms(200);
					display11();
				}
			}
			if(KEYOK==0)
			{				
				Delay_Ms(10);
				if(KEYOK==0)
				{
					confirmpassword();	//确认密码
					goto begin;         //回到初始界面
				}
			}
			if(KEYCANCEL==0)
			{				
				Delay_Ms(10);
				if(KEYCANCEL==0)				
				goto begin;
			}
	  }
  }

//第二个：输入指纹解锁界面
	if(KEY2 == 0)
	{	
		Delay_Ms(10);
		if(KEY2==0)
		{
			Delay_Ms(200);
			while(1)
			{
				if(KEYCANCEL==0)
				{
					Delay_Ms(10);
					if(KEYCANCEL==0)
					{
					  goto begin;
					}
				}
				else
					FPM10A_Find_Fingerprint();	//识别指纹（其中包括了页面跳转和开锁函数）
			}
		}
	}


//第三个：管理员界面
	if(KEY3 == 0)
	{
		Delay_Ms(10);
		if(KEY3==0)
		{
			Delay_Ms(200);
			AD:             //设置另一个标识点，避免cancel直接跳回主页面
			w=0;
			x=0;
			y=0;
			z=0;
			display13();			//显示第1.3界面（输入管理员密码）
		while(1)
		{ 
			if(KEY1==0)
			{			
				Delay_Ms(10);
				if(KEY1==0)
				{
					w+=1;
					Delay_Ms(200);
					display13();
				}
			}
			if(KEY2==0)
			{			
				Delay_Ms(10);
				if(KEY2==0)
				{
					x+=1;
					Delay_Ms(200);
					display13();
				}
			}
			if(KEY3==0)
			{			
				Delay_Ms(10);
				if(KEY3==0)
				{
					y+=1;
					Delay_Ms(200);
					display13();
				}
			}
			if(KEY4==0)
			{			
				Delay_Ms(10);
				if(KEY4==0)
				{
					z+=1;
					Delay_Ms(200);
					display13();
				}
			}
			if(KEYOK==0) //进入1.3.1界面（管理员界面）
			{			
				Delay_Ms(10);
				if(KEYOK==0)
				{
					if(w==4&&x==3&&y==2&&z==1)
					{
						LCD1602_Display(0xc0,"  Suceeded ^_^  ",0,16);
						Delay_Ms(1500);
					 	LCD1602_WriteCMD(0x01);		
					  display131();         //回到1.3.1界面
						while(1)
	          {   		     								        
							if(local_date==0)                       //local date作为屏幕上 “*” 位置的选择变量
							{
								LCD1602_Display(0x80,  "  ",0,2);
								LCD1602_Display(0xc0,  " *",0,2);
								LCD1602_Display(0xc0+8,"  ",0,2);		
		          }
		          else if(local_date==1)
		          {
								LCD1602_Display(0x80,  "  ",0,2);
								LCD1602_Display(0xc0,  "  ",0,2);
								LCD1602_Display(0xc0+8," *",0,2);		
		          }			
		        //确认键
		          if(KEYOK == 0)
		          {	 
								while(KEYOK == 0);//等待松开按键								
								switch(local_date)
								{																																					
									case 0:	 //添加指纹
									FPM10A_Add_Fingerprint();     //添加指纹（包括成功和失败两种情况的界面显示）
									goto AD; 									
									case 1:	//清空指纹
									FPM10A_Delete_All_Fingerprint();//删除指纹（包括确认删除、成功和失败的界面显示）
									goto AD; //回到管理员界面（避免直接返回初始界面）
								}
							}
		    //切换键(切换“*” 的位置)
							if(KEY1 == 0)
							{
								while(KEY1 == 0); //等待松开按键				
								if(local_date<=1)
								{
									local_date++;
									if(local_date==2) //只有0、1两种情况，到2归零
										local_date=0;						
								}		
			        }	
							if(KEYCANCEL==0)
							{			
								Delay_Ms(10);
								if(KEYCANCEL==0)
									goto begin;    //回到初始界面
							}
							Delay_Ms(100); //延时判断100MS检测一次		
						}
					}
			else
			{
				LCD1602_Display(0xc0,"  Failed QAQ    ",0,16);
		    Delay_Ms(1500);					
      }
			x=0;
			y=0;
			z=0;
			w=0;
      goto begin; 
		}
	}		
			if(KEYCANCEL==0)
			{					
				Delay_Ms(10);
				if(KEYCANCEL==0)
				goto begin;
			}
		}
   }
 }
/***第四个界面(查询上一次用指纹解锁的指纹id号)***/	
	if(KEY4==0)
	{
		Delay_Ms(10);
		if(KEY4==0)
		{
			Delay_Ms(200);
			LCD1602_WriteCMD(0x01);
			LCD1602_Display(0x80,"      CHECK     ",0,16);
		}
		while(1)
		{
			if(KEYOK==0)
			{
				Delay_Ms(10);
			  if(KEYOK==0)
				{
					find_fingerid=At24c02Read(1); //从24C02中读取ID号
					LCD1602_WriteCMD(0xc0+7);
		  		LCD1602_WriteDAT(find_fingerid/100+48);
				  LCD1602_WriteDAT(find_fingerid%100/10+48);
				  LCD1602_WriteDAT(find_fingerid%100%10+48);	
				}
			}
			if(KEYCANCEL==0)
			{
				Delay_Ms(10);
				if(KEYCANCEL==0)
					goto begin;
			}
		}
	}
}
}