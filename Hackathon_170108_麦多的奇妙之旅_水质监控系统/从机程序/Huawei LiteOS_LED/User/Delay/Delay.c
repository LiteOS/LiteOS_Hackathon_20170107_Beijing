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


