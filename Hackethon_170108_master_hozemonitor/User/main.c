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
#include "gsm.h"

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
		LCD_Init();  	
		GSM_Init();	  
	
		ILI9341_GramScan ( 1 );
		ILI9341_Clear ( 0, 0, 240, 320, macWHITE);
		ILI9341_DispString_EN ( 0, 10, "   Welcome to Huawei LiteOS ", macWHITE, macRED );
	ILI9341_DispString_EN ( 10, 40, "Service Time (MM:SS.ms):", macWHITE, macBLACK );
		ILI9341_DispString_EN ( 10, 60, "Receive: ", macWHITE, macBLACK );  
		ILI9341_DispString_EN ( 10, 160, "Send:", macWHITE, macBLACK );
	
		ILI9341_DispString_EN ( 30, 240, "Temperature is:", macWHITE, macBLACK );
		ILI9341_DispString_EN ( 30, 260, "Humidity is:", macWHITE, macBLACK );
}
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	//UINT32 count=0;
	//char str[64];
	//char ch;

	while(1)
	{
		Receive();
		/*
		memset(str,0,64);
		count++;
		ch = fgetc(NULL);
		sprintf(str,"%d:%s",count,str);	
		//scanf("%s",str);
		ILI9341_DispString_EN ( 200, 40, str, macWHITE, macRED );
		//printf("This is task 1,count is %d\r\n",count);
		LED1_TOGGLE;
		LED2_TOGGLE;
		*/
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
		
		if( count / 60 > 0)
			sprintf(str,"%2d.%2d",count /60, count %60);
		else
			sprintf(str,"%2d",count);
		
		ILI9341_DispString_EN ( 200, 40, str, macWHITE, macRED );
		//printf("This is task 2,count is %d\r\n",count);
		LED2_TOGGLE;
		LED3_TOGGLE;
		uwRet = LOS_TaskDelay(1000);
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
	char cStr3[64];
	
	Connect();
	uwRet = LOS_TaskDelay(12000);
	
	Connect();
	uwRet = LOS_TaskDelay(10000);
	
	Connect();
	uwRet = LOS_TaskDelay(8000);
	
	while(1)
	{
		if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
		{
			memset(cStr1,0,sizeof(cStr1));
			memset(cStr2,0,sizeof(cStr2));
			memset(cStr3,0,sizeof(cStr3));
			sprintf(cStr1,"%d.%d C",DHT11_Data.temp_int,DHT11_Data.temp_deci);
			sprintf(cStr2,"%d.%d %%RH",DHT11_Data.humi_int,DHT11_Data.humi_deci);
			sprintf(cStr3,"%d.%d,%d.%d",DHT11_Data.temp_int, DHT11_Data.temp_deci, DHT11_Data.humi_int, DHT11_Data.humi_deci);
			ILI9341_DispString_EN ( 160, 240, cStr1, macWHITE, macBLUE );
			ILI9341_DispString_EN ( 140, 260, cStr2, macWHITE, macBLUE );
			//printf("\r\n ??DHT11??,???%d.%d C,??? %d.%d %%RH",\
			// DHT11_Data.temp_int,DHT11_Data.temp_deci,DHT11_Data.humi_int,DHT11_Data.humi_deci);
			
			Send(cStr3);
			//SendByte(DHT11_Data.temp_int);
			//SendByte(DHT11_Data.temp_deci);
			//uwRet = LOS_TaskDelay(1000);
			//SendByte(DHT11_Data.humi_int);
			//SendByte(DHT11_Data.humi_deci);
		}
		else
		{
			printf("DHT11 ERROR!\r\n");
		}
		uwRet = LOS_TaskDelay(3000);
		if(uwRet != LOS_OK)
		{
			Disconnect();
			return;
		}
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
