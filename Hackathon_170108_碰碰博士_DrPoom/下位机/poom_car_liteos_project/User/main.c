/**
  ******************************************************************************
  * @file    main.c
  * @author  Supowang  any question please send mail to 512965803@qq.com
  * @version V1.0
  * @date    2016-12-07
  * @brief   Huawei LiteOS第三方开发板移植DEMO 
  ******************************************************************************
  * @attention
  *	Huawei LiteOS Source Code 下载:http://developer.huawei.com/ict/cn/site-iot/product/liteos
	* GitHub地址：https://github.com/LiteOS
  * 实验平台:秉火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *	
	* 本例程仅供学习参考，Huawei LiteOS和所有驱动代码版权由原作者享有。
  ******************************************************************************
  */
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
#include "bsp_esp8266.h"
#include "bsp_usart.h"
#include "bsp_beep.h"
#include "bsp_pmap.h"
#include "bsp_psensor.h"

void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);
UINT32 g_TestTskHandle;
UINT32 g_TestTskHandle1;
UINT32 g_TestTskHandle2;
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
		ESP8266_Init ();
		Beep_Init ();
		PMap_Init();
		ILI9341_GramScan ( 1 );
		ILI9341_Clear ( 0, 0, 240, 320, macWHITE);
		ILI9341_DispString_EN ( 0, 10, "  Welcome to Huawei LiteOS!", macWHITE, macRED );
		ILI9341_DispString_EN ( 10, 40, "Current Position is: ", macWHITE, macBLUE );
		ILI9341_DispString_EN ( 10, 60, "Left Sensor Stat is: ", macWHITE, macBLUE );
		ILI9341_DispString_EN ( 10, 80, "Forward Sensor Stat is: ", macWHITE, macBLUE );
		ILI9341_DispString_EN ( 10, 100, "Back Sensor Stat is: ", macWHITE, macBLUE );
		ILI9341_DispString_EN ( 10, 120, "Right Sensor Stat is: ", macWHITE, macBLUE );
		
		//ILI9341_DispString_EN ( 30, 110, "Temperature is:", macWHITE, macBLACK );
		//ILI9341_DispString_EN ( 30, 130, "Humidity is:", macWHITE, macBLACK );
	
		LED_BLUE;
	
}
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
	char str[10];
	while(1)
	{
		sprintf(str,"%d",count);
		ILI9341_DispString_EN ( 200, 40, str, macWHITE, macRED );
		//printf("This is task 1,count is %d\r\n",count);
		if(count < 13)
		{
			count++;
			LED_GREEN;
			Usart_SendByte(USART1, getPMapCmd(count).dir );
			ILI9341_DispString_EN ( 30, 160, "Running~~", macWHITE, macBLACK );
		}
		else
		{
			ILI9341_DispString_EN ( 30, 160, "Arrived!!", macWHITE, macBLACK );
			LED_BLUE;
			Usart_SendByte(USART1, 50 );
		}
		//LED1_TOGGLE;
		//LED2_TOGGLE;
		uwRet = LOS_TaskDelay(3500);
		if(uwRet !=LOS_OK)
			return;
	}
}
UINT32 creat_task1(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 5;
	task_init_param.pcName = "task1";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle1,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

static uint16_t task_stat = 1;

VOID task2(void)
{
	UINT32 uwRet = LOS_OK;
	uint16_t stat=0;
	char str[10];
	while(1)
	{
		uint16_t sensor_stat = 0;
		stat = Get_Sensor_Stat(0);
		sprintf(str,"%d",stat);
		ILI9341_DispString_EN ( 200, 60, str, macWHITE, macRED );
		
		sensor_stat |= stat;
		
		stat = Get_Sensor_Stat(1);
		sprintf(str,"%d",stat);
		ILI9341_DispString_EN ( 200, 80, str, macWHITE, macRED );
		
		sensor_stat |= stat;
		
		stat = Get_Sensor_Stat(2);
		sprintf(str,"%d",stat);
		ILI9341_DispString_EN ( 200, 100, str, macWHITE, macRED );
		
		sensor_stat |= stat;
		
		stat = Get_Sensor_Stat(3);
		sprintf(str,"%d",stat);
		ILI9341_DispString_EN ( 200, 120, str, macWHITE, macRED );
		
		sensor_stat |= stat;
		
		if(sensor_stat && task_stat)
		{
			LED_RED;
			macBEEP_ON();
			Usart_SendByte(USART1, 50 );
			task_stat = 0;
			uwRet = LOS_TaskSuspend(g_TestTskHandle1);
			if (uwRet != LOS_OK)
			{
					printf("Suspend TaskHi Failed.\r\n");
					return;
			}
		}
		else if(!task_stat)
		{
			task_stat = 1;
			macBEEP_OFF();
			uwRet = LOS_TaskResume(g_TestTskHandle1);
			if (uwRet != LOS_OK)
			{
					printf("Resume TaskHi Failed.\r\n");
					return;
			}
			/*
			if(!task_stat)
			{
				task_stat = 1;
			}
			*/
		}
		
		//LED2_TOGGLE;
		//LED3_TOGGLE;
		uwRet = LOS_TaskDelay(100);
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task2(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 6;
	task_init_param.pcName = "task2";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle2,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}
/*
VOID WiFi_task(void)
{
	UINT32 uwRet = LOS_OK;
	ESP8266_StaTcpClient_UnvarnishTest();
	while(1)
	{
		uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_WiFi_task(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 4;
	task_init_param.pcName = "WiFi_task";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)WiFi_task;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE * 2;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}
*/
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
			ILI9341_DispString_EN ( 160, 110, cStr1, macWHITE, macRED );
			ILI9341_DispString_EN ( 140, 130, cStr2, macWHITE, macRED );
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
	
	//printf("Wifi connection \r\n");
	//ESP8266_StaTcpClient_UnvarnishTest();
	
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
	
	/***
	uwRet = creat_WiFi_task();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	***/
	return LOS_OK;
}
/*********************************************END OF FILE**********************/
