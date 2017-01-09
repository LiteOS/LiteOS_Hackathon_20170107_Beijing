/* Includes ------------------------------------------------------------------*/
// Huawei LiteOS头文件
#include "stdio.h"
#include "stdlib.h"
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
// STM32硬件相关头文件
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_lcd.h"
#include "bsp_dht11.h"
void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);
UINT32 g_TestTskHandle;
DHT11_Data_TypeDef  DHT11_Data;
void Delay(__IO uint32_t nCount) //简单延时
{
	for(; nCount != 0; nCount--);
}
void hardware_init(void)
{
		LED_GPIO_Config();
		USART_Config();
		DHT11_Init();
		LCD_Init ();  
		ILI9341_GramScan ( 1 );
		ILI9341_Clear ( 0, 0, 240, 320, macWHITE);
		ILI9341_DispString_EN ( 50, 60, "  Wifi is Ready !", macWHITE, macRED );

}
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
	char str[10];
	while(1)
	{
		count++;
		sprintf(str,"%d",count);
//		ILI9341_DispString_EN ( 200, 40, str, macWHITE, macRED );
		printf("This is task 1,count is %d\r\n",count);
		LED1_TOGGLE;
		LED2_TOGGLE;
		uwRet = LOS_TaskDelay(1000);
		if(uwRet !=LOS_OK)
			return;
	}
}
UINT32 creat_task1(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 1;
	task_init_param.pcName = "task1";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

VOID task2(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
	char str[10];
	while(1)
	{
		count++;
		sprintf(str,"%d",count);
//		ILI9341_DispString_EN ( 200, 60, str, macWHITE, macRED );
		printf("This is task 2,count is %d\r\n",count);
		LED2_TOGGLE;
		LED3_TOGGLE;
		uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task2(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 1;
	task_init_param.pcName = "task2";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}


VOID DHT11_task(void)
{
	UINT32 uwRet = LOS_OK;
	char cStr1[10];
	char cStr2[10];
	while(1)
	{
		if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
		{
			sprintf(cStr1,"%d.%d C",DHT11_Data.temp_int,DHT11_Data.temp_deci);
			sprintf(cStr2,"%d.%d %%RH",DHT11_Data.humi_int,DHT11_Data.humi_deci);
//			ILI9341_DispString_EN ( 160, 110, cStr1, macWHITE, macRED );
//			ILI9341_DispString_EN ( 140, 130, cStr2, macWHITE, macRED );
			printf("\r\n ??DHT11??,???%d.%d C,??? %d.%d %%RH",\
			DHT11_Data.temp_int,DHT11_Data.temp_deci,DHT11_Data.humi_int,DHT11_Data.humi_deci);
		}
		else
		{
			printf("DHT11 ERROR!\r\n");
		}
		uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_DHT11_task(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "DHT11_task";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)DHT11_task;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	hardware_init();
	LED1_ON;
	SOFT_DELAY;
	uwRet = creat_task1();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	uwRet = creat_task2();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	uwRet = creat_DHT11_task();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return LOS_OK;
}
/*********************************************END OF FILE**********************/
