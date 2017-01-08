#include "sys.h"
#include "usart.h" 
#include "led.h"
/* stm32 hardware and driver*/

#include "string.h" 
#include "los_base.h" 
#include "los_config.h" 
#include "los_typedef.h" 
#include "los_hwi.h" 
#include "los_task.ph" 
#include "los_sem.h" 
#include "los_event.h" 
#include "los_memory.h" 
#include "los_queue.ph" 
/*lite OS header*/

void Delay(__IO u32 nCount); 
#define SOFT_DELAY Delay(0x0FFFFF); 
UINT32 g_TestTskHandle;

//int main(void)
//{ 
//	u8 t=0;
//	Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz
//	delay_init(168);		//初始化延时函数
//	uart_init(84,115200);	//串口初始化为115200
//	while(1)
//	{
//		printf("t:%d\r\n",t);
//		delay_ms(500);
//		t++;
//	}
//}
void Delay(__IO uint32_t nCount) {
	for(; nCount != 0; nCount--);
}

void task1(void){
	UINT32 uwRet = LOS_OK;
	UINT32 count = 0;
	while(1){
		count++;
		LED0 = !LED0;	
		uwRet = LOS_TaskDelay(1000);
		if(uwRet != LOS_OK)return;
	}
}

UINT32 creat_task1(void) { 
	UINT32 uwRet = LOS_OK; 
	TSK_INIT_PARAM_S task_init_param; 
	task_init_param.usTaskPrio = 2	;//????? 
	task_init_param.pcName = "task1";//??? 
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;//???????? 
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//???????? 
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED; 
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//???????? 
		if(uwRet !=LOS_OK) { 
			return uwRet; 
		} 
	return uwRet; 
}

void uart_task(void){
	UINT32 uwRet = LOS_OK;
	while(1){
		if(USART2_RX_STA&0x8000)
		{					   
			u8 len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
			if(USART2_RX_BUF[0] == 0x14&USART2_RX_BUF[1] == 0x1a){
				printf("find it");//插入换行
			}
			USART2_RX_STA=0;
			//printf("\r\n");//插入换行
		}
		uwRet = LOS_TaskDelay(100);
		if(uwRet != LOS_OK)return;
	}
}



UINT32 creat_uart_task(void){
	UINT32 uwRet = LOS_OK; 
	TSK_INIT_PARAM_S task_init_param; 
	task_init_param.usTaskPrio = 1;//????? 
	task_init_param.pcName = "uart_task";//??? 
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)uart_task;//???????? 
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//???????? 
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED; 
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//???????? 
		if(uwRet !=LOS_OK) { 
			return uwRet; 
		} 
	return uwRet; 
}

void command_received(void){
	UINT32 uwRet = LOS_OK;
	while(1){
		if(USART_RX_STA&0x8000)
		{					   
			u8 len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(u8 t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);//等待发送结束
			}
			printf("\r\n");//插入换行
			USART_RX_STA=0;
		}
		uwRet = LOS_TaskDelay(100);
		if(uwRet != LOS_OK)return;
	}
}

UINT32 creat_command_task(void){
	UINT32 uwRet = LOS_OK; 
	TSK_INIT_PARAM_S task_init_param; 
	task_init_param.usTaskPrio = 1;//????? 
	task_init_param.pcName = "command_received";//??? 
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)command_received;//???????? 
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//???????? 
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED; 
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//???????? 
		if(uwRet !=LOS_OK) { 
			return uwRet; 
		} 
	return uwRet; 
}

UINT32 osAppInit(void) { 
	UINT32 uwRet = 0; 
	LED_Init();
	Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz 
	uart_init(84,115200);	//串口初始化为115200
	//hardware_init(); 
	LED0 = 0;
	SOFT_DELAY; 
	uwRet = creat_task1(); 
	if(uwRet !=LOS_OK) { 
		return uwRet; 
	} 
	uwRet = creat_uart_task(); 
	if(uwRet !=LOS_OK) { 
		return uwRet; 
	} 
//	uwRet = creat_command_task(); 
//	if(uwRet !=LOS_OK) { 
//		return uwRet; 
//	} 
	return LOS_OK; 
} 

int main1(void){
	Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz 
	uart_init(84,115200);	//串口初始化为115200
	u8 len;
	u8 t;
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}
	}
}
