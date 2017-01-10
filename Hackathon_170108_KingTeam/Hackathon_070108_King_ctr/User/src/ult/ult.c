/*--------------------------------------
文件：ult.c
功能：实现延时函数，以及一些常见的字符串操作函数
当前版本：1.01   删除不必要的注释和调试信息

----------------------------------------*/
#include "ult.h"
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "types.h"


//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void Systick_Init (u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8    
}								    
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,time_ms<=1864 
//当需要延时s以上时，请调用Delay_s函数
void delay_s( uint32 time_s )
{
  for(;time_s>0;time_s--)
    delay_ms(1000);
}
void delay_ms( uint32 time )
{	 		  	  
	uint16_t i=0;
	while(time--)
	{
		i=12000;
		while(i--);
	}  	    
}   
//延时nus
//nus为要延时的us数.		    								   
void delay_us( uint32 time )
{		
	uint16_t i=0;
	while(time--)
	{
		i=10;
		while(i--);
	} 
}



uint16 ATOI(char* str,uint16 base	)
{
  unsigned int num = 0;
  while (*str !=0)
          num = num * base + C2D(*str++);
  return num;
}

uint32 ATOI32(char* str,uint16 base	)
{
  uint32 num = 0;
  while (*str !=0)
          num = num * base + C2D(*str++);
  return num;
}


void itoa(uint16 n,uint8 str[5], uint8 len)
{
  
  uint8 i=len-1;

  memset(str,0x20,len);
  do{
  str[i--]=n%10+'0';
  
 }while((n/=10)>0);

 return;
}
char *itoa1(int32_t value, char *string, int radix)
{
        uint32_t     i, d;
        int     flag = 0;
        char    *ptr = string;

        /* This implementation only works for decimal numbers. */
        if (radix != 10)
        {
            *ptr = 0;
            return string;
        }

        if (!value)
        {
            *ptr++ = 0x30;
            *ptr = 0;
            return string;
        }
        /* if this is a negative value insert the minus sign. */
        if (value < 0)
        {
            *ptr++ = '-';

            /* Make the value positive. */
            value *= -1;
        }

        for (i = 1000000000; i > 0; i /= 10)
        {
            d = value / i;

            if (d || flag)
            {
                *ptr++ = (char)(d + 0x30);
                value -= (d * i);
                flag = 1;
            }
        }

        /* Null terminate the string. */
        *ptr = 0;
        return string;

} /* NCL_Itoa */
int ValidATOI(char* str, int base,int* ret)
{
  int c;
  char* tstr = str;
  if(str == 0 || *str == '\0') return 0;
  while(*tstr != '\0')
  {
    c = C2D(*tstr);
    if( c >= 0 && c < base) tstr++;
    else    return 0;
  }
  
  *ret = ATOI(str,base);
  return 1;
}
 
void replacetochar(char * str,	char oldchar,char newchar	)
{
  int x;
  for (x = 0; str[x]; x++) 
    if (str[x] == oldchar) str[x] = newchar;	
}

char C2D(uint8 c	)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c -'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c -'A';

	return (char)c;
}

uint16 swaps(uint16 i)
{
  uint16 ret=0;
  ret = (i & 0xFF) << 8;
  ret |= ((i >> 8)& 0xFF);
  return ret;	
}

uint32 swapl(uint32 l)
{
  uint32 ret=0;
  ret = (l & 0xFF) << 24;
  ret |= ((l >> 8) & 0xFF) << 16;
  ret |= ((l >> 16) & 0xFF) << 8;
  ret |= ((l >> 24) & 0xFF);
  return ret;
}

//get mid str
void mid(int8* src, int8* s1, int8* s2, int8* sub)
{
	int8* sub1;
	int8* sub2;
	uint16 n;

  sub1=strstr(src,s1);
  sub1+=strlen(s1);
  sub2=strstr(sub1,s2);
  n=sub2-sub1;
  strncpy(sub,sub1,n);
  sub[n]=0;
}
void inet_addr_(unsigned char* addr,unsigned char *ip)
{
	int i;
//	u_long inetaddr = 0;
	char taddr[30];
	char * nexttok;
	char num;
	strcpy(taddr,(char *)addr);
	
	nexttok = taddr;
	for(i = 0; i < 4 ; i++)
	{
		nexttok = strtok(nexttok,".");
		if(nexttok[0] == '0' && nexttok[1] == 'x') num = ATOI(nexttok+2,0x10);
		else num = ATOI(nexttok,10);
		
		ip[i] = num;
		nexttok = NULL;
	}
}	
#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
 
  while (1)
  {
  }
}

#endif



