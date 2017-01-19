
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "proto.h"
#include "IO.h"

#include "los_task.h"

//这是我创建的一个任务, 任务中反复读取虚拟机指令并执行

//移植LiteOS时遇到定时器冲突, 导致OS调度不正常, 时间有限所以临时屏蔽了系统调度, 只有这一个任务在执行,
//任务中等待电脑图形化上位机发送串口字节码序列, 收到之后就存储到RAM中并开始调用虚拟机解释执行
//收到的字节码序列.

UINT32 myTask(void)
{
	Timer_Init();
	IO_Init();
	Serial_Init(115200);
	//Serial_Init(9600);
	
	Timer_delay_ms(500);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	Timer_delay_ms(500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	
	setup();
	while(1) {
		
		//反复读取虚拟机指令并执行
		loop();
		
		
		//这里不需要延时, 程序永不退出
		/*
		uwRet = LOS_TaskDelay(50);
		if (uwRet != LOS_OK)
		{
				printf("Delay balancecar Failed.\r\n");
				return LOS_NOK;
		}
		*/
	}
}




