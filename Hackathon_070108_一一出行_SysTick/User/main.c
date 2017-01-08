/**
 * 引入头文件
 */
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"

/**
 * 用户选择时长开始充电以后，电路通电
 * 经过选定的时长以后，停止充电，电路自动断电
 */
int main(void)
{	
	// 初始化LED端口
	LED_GPIO_Config();
	SysTick_Init();
	// 用户选择完成，线路开始通电
	LED1( ON );
	// 充电中，延时特定时间，这里以1s为例
	// 选定的时长以分钟为单位
		Delay_us(100000);
	// 充电时间到，线路自动断电
	LED1( OFF );
	// 后续回传处理。。。
}
