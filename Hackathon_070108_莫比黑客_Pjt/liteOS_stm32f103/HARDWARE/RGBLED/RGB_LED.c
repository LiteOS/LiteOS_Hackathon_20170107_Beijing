#include "RGB_LED.h"
#include "delay.h"
void send_8G_8R_8B()
{
	u8 i;
	for(i=0;i<8;i++)
		{
			
			send_GRB(255,0,0);
		}
	for(i=0;i<8;i++)
		{
			send_GRB(0,255,0);
		}
	for(i=0;i<8;i++)
		{
			send_GRB(0,0,255);
		}
		reset();
	LED1=0;
}
void send_4G_4R_4B_1()
{
	u8 i;
	for(i=0;i<8;i++)
		{
			if(i%2==0)
			send_GRB(255,0,0);
			else
			send_GRB(0,0,0);
		}
	for(i=0;i<8;i++)
		{
			if(i%2==0)
			send_GRB(0,255,0);
			else
			send_GRB(0,0,0);
		}
	for(i=0;i<8;i++)
		{
			if(i%2==0)
			send_GRB(0,0,255);
			else
			send_GRB(0,0,0);
		}
		reset();
	LED1=0;
}

void send_4G_4R_4B_2()
{
	u8 i;
	for(i=0;i<8;i++)
		{
			if(i%2==1)
			send_GRB(255,0,0);
			else
			send_GRB(0,0,0);
		}
	for(i=0;i<8;i++)
		{
			if(i%2==1)
			send_GRB(0,255,0);
			else
			send_GRB(0,0,0);
		}
	for(i=0;i<8;i++)
		{
			if(i%2==1)
			send_GRB(0,0,255);
			else
			send_GRB(0,0,0);
		}
		reset();
	LED1=0;
}
void sendi_GRB(u8 Green,u8 Red,u8 Blue,u8 j)
{
	u8 i;
	for(i=0;i<j;i++)
		{
			send_GRB(Green,Red,Blue);
		}
		reset();
	LED1=0;
}
void send_circle_cycle(u8 cycle,u8 delay_time)
{
	u8 i,j,k;
	for(k=0;k<cycle;k++)
	{
		for(i=0;i<19;i++)
		{
			for(j=0;j<i;j++)
			{
				send_GRB(0,0,0);
			}
			send_GRB(2,2,2);
			send_GRB(5,5,5);
			send_GRB(10,10,10);
			send_GRB(15,15,15);
			send_GRB(20,20,20);
			send_GRB(25,25,25);
			delay_ms(delay_time);
		}
		i=18;
		send_GRB(25,25,25);
		for(j=0;j<i;j++)
		{
			send_GRB(0,0,0);
		}
		send_GRB(2,2,2);
		send_GRB(5,5,5);
		send_GRB(10,10,10);
		send_GRB(15,15,15);
		send_GRB(20,20,20);
		delay_ms(delay_time);
		
		send_GRB(20,20,20);
		send_GRB(25,25,25);
		for(j=0;j<i;j++)
		{
			send_GRB(0,0,0);
		}
		send_GRB(2,2,2);
		send_GRB(5,5,5);
		send_GRB(10,10,10);
		send_GRB(15,15,15);
		delay_ms(delay_time);
		
		send_GRB(15,15,15);
		send_GRB(20,20,20);
		send_GRB(25,25,25);
		for(j=0;j<i;j++)
		{
			send_GRB(0,0,0);
		}
		send_GRB(2,2,2);
		send_GRB(5,5,5);
		send_GRB(10,10,10);
		delay_ms(delay_time);
		
		
		send_GRB(10,10,10);
		send_GRB(15,15,15);
		send_GRB(20,20,20);
		send_GRB(25,25,25);
		for(j=0;j<i;j++)
		{
			send_GRB(0,0,0);
		}
		send_GRB(2,2,2);
		send_GRB(5,5,5);
		delay_ms(delay_time);
		
		
		send_GRB(5,5,5);
		send_GRB(10,10,10);
		send_GRB(15,15,15);
		send_GRB(20,20,20);
		send_GRB(25,25,25);
		for(j=0;j<i;j++)
		{
			send_GRB(0,0,0);
		}
		send_GRB(2,2,2);
		delay_ms(delay_time);

		send_GRB(2,2,2);
		send_GRB(5,5,5);
		send_GRB(10,10,10);
		send_GRB(15,15,15);
		send_GRB(20,20,20);
		send_GRB(25,25,25);
		for(j=0;j<i;j++)
		{
			send_GRB(0,0,0);
		}
	}
	delay_ms(20);  
	send24_GRB(0,0,0);
	delay_ms(20);
}
void send_circle()
{
	u8 i,j,k;
	for(k=2;k>0;k--)
	{
		for(i=0;i<18;i++)
		{
			for(j=0;j<i;j++)
			{
				send_GRB(0,0,0);
			}
			send_GRB(2,2,2);
			send_GRB(5,5,5);
			send_GRB(10,10,10);
			send_GRB(15,15,15);
			send_GRB(20,20,20);
			send_GRB(25,25,25);
			reset();
			LED1=1;
			delay_ms(45);
		}
			send24_GRB(0,0,0);
			delay_ms(1000);
	}
}
/*
 *  灯环半圆跑马灯
*/

void send_semicircle()
{
	u8 i,j;
	static u8 m=70;
	static u8 n=70;
	for(j=0;j<12;j++)
	{
			for(i=0;i<j;i++)
				send_GRB(m,m,0);
			for(i=j;i<12+j;i++)
				send_GRB(0,m,n);
		for(i=12+j;i<24+j;i++)
			send_GRB(m,m,0);
		reset();
		LED1=0;
		delay_ms(50);
	}
	for(j=0;j<12;j++)
	{
			for(i=0;i<j;i++)
				send_GRB(0,m,n);
			for(i=j;i<12+j;i++)
				send_GRB(m,m,0);
		for(i=12+j;i<24+j;i++)
			send_GRB(0,m,n);
		reset();
		LED1=0;
		delay_ms(50);
	}
	m+=5;
	n+=7;
}
void double_GRB()
{
	u8 i;
	for(i=0;i<48;i++)
	{
		sendi_GRB(0,125,255,i);
		delay_ms(50);
	}
}
void send24_GRB(u8 Green,u8 Red,u8 Blue)
{
	u8 i;
	LED1=0;
	for(i=0;i<24;i++)
		{
			send_GRB(Green,Red,Blue);
		}
		reset();
	LED1=0;
}
void send_GRB(u8 Green,u8 Red,u8 Blue)//RGB 可以显示  
{
	u8 color_i;
	for(color_i=0;color_i<8;color_i++)
	{
		if(((Green&0x80)>>7)==1)  //1000 0000
			send_bit1();
		else
			send_bit0();
		Green=Green<<1;
	}
	for(color_i=0;color_i<8;color_i++)
	{
		if(((Red&0x80)>>7)==1)
			send_bit1();
		else
			send_bit0();
		Red=Red<<1;
	}
	for(color_i=0;color_i<8;color_i++)
	{
		if(((Blue&0x80)>>7)==1)
			send_bit1();
		else
			send_bit0();
		Blue=Blue<<1;
	}
	LED1=0;
}
//可以封装了
void send_Green()//显示绿色
{
	u8 green_i;
	for(green_i=0;green_i<8;green_i++)
	{
		send_bit1();
	}
	for(green_i=0;green_i<16;green_i++)
	{
		send_bit0();
	}
}
void send_Red()//显示红色
{
	u8 red_i;
	for(red_i=0;red_i<8;red_i++)
	{
		send_bit0();
	}
	for(red_i=0;red_i<8;red_i++)
	{
		send_bit1();
	}
	for(red_i=0;red_i<8;red_i++)
	{
		send_bit0();
	}
}
void send_Blue()//显示蓝色
{
	u8 blue_i;
	for(blue_i=0;blue_i<8;blue_i++)
	{
		send_bit0();
	}
	for(blue_i=0;blue_i<8;blue_i++)
	{
		send_bit0();
	}
	for(blue_i=0;blue_i<8;blue_i++)
	{
		send_bit1();
	}
}
void send_bit0()//发送位0  1.264us  占空比31.82%
{
	LED1=1;
	LED1=1;
	LED1=1;
	LED1=0;
	LED1=0;
	LED1=0;
	LED1=0;
	LED1=0;
	LED1=0;
	LED1=0;
	LED1=0;
}
void send_bit1()//发送1   1.264us   高电平占空比58.2%
{
	//__nop();
	LED1=1;
	LED1=1;
	LED1=1;	
	LED1=1;
	LED1=1;
	LED1=1;
	LED1=1;
	LED1=0;
	LED1=0;
	LED1=0;
	LED1=0;
}
void reset()//复位  周期52us  高电平占空比 1.3%
{
	u8 res_i;
	LED1=0;
	LED1=1;
	for(res_i=0;res_i<195;res_i++)
		//LED1=0; //??????
	LED1=1;
}