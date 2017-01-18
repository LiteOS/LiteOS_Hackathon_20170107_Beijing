#include "Common.h"

//*************全局变量*************//
//*********************************//

/*
 * 函数名：Delay_us
 * 描述  ：us延时
 * 输入  ：time：延时长度
 * 返回  ：无
 */
void Delay_us (u16 time)
{
	u16 i = 0;
	while(time--)
		{
			i = 10;
			while(i--);
		}
}


/*
 * 函数名：Delay_ms
 * 描述  ：ms延时
 * 输入  ：time：延时长度
 * 返回  ：无
 */
void Delay_ms (u16 time)
{
	u16 i = 0;
	while(time--)
		{
			i = 12000;
			while(i--);
		}
}

extern uint8_t OSStatue;
void UserDelay_KeyScan (u16 time)
{
//  TimingDelay = time;	
//  SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
//  
//  while(TimingDelay != 0)
//  {
//    Key_Scan();
//  }
//  
//  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
  
  
//  if (OSStatue == 0)
    Delay_ms(time * 0.6);
//  else
//  {
//    LOS_TaskDelay(time);//系统延时1S
//  }
}


