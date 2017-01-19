
#include "stm32f4xx_conf.h"
#include "typedef.h"

__IO u16 ntime;

void Timer_Init()
{
	ntime = 0;
}

void Timer_SetTimeMs( uint16 t )
{
	ntime = t;
	SysTick_Config(500000);//1ms产生一次中断
}

bool Timer_isTimeOut()
{
	return ntime == 0;
}

void Timer_delay_ms(u16 nms)
{
	ntime=nms;
	SysTick_Config(100000);//1ms产生一次中断
	while(ntime);
	SysTick->CTRL=0x00;
}
void Timer_delay_us(u32 nus)
{
	ntime=nus;
	SysTick_Config(168);//1us产生一次中断
	while(ntime);
	SysTick->CTRL=0x00;
}


