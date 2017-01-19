#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
#include "los_queue.h"


#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_led.h"
#include "WIFI.h"
#include "bsp_usart1.h"
#include "bsp_esp8266.h"
#include "pm2d5.h"
#include "bsp_key.h" 
/**********************************************************************************/
//WIFI数据
//volatile uint8_t ucTcpClosedFlag = 0;

/***********************************************************************************/
void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);
UINT32 g_TestTskHandle;

//事件控制结构体
EVENT_CB_S  example_event;
EVENT_CB_S  WIFI_event;
//等待的事件类型
#define event_wait 					0x00000001
#define event_PM2d5_finish 	0x00000002

//消息队列
UINT32 g_uwQueue;


void Delay(__IO uint32_t nCount) 
{
	for(; nCount != 0; nCount--);
}



void hardware_init(void)
{
	LED_GPIO_Config();
	ESP8266_Init (); 
	USARTx_Config();
	ADCx_Init();
	Key_GPIO_Config();
	interruptInit();
}

VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
  static float pm2d5 = 0;
	
	float temperature = 0;
	while(1)
	{
		count++;
		pm2d5 = Get_PM2d5();
		uwRet = LOS_QueueWrite(g_uwQueue, &pm2d5, 4, 0);
		uwRet = LOS_EventWrite(&WIFI_event, event_PM2d5_finish); //发送WiFi事件
		if(uwRet != LOS_OK)
		{
				printf("event write failed .\n");
				return;
		}
		LOS_EventClear(&WIFI_event, ~example_event.uwEventID);
		
		uwRet = LOS_TaskDelay(6000);
		if(uwRet !=LOS_OK)
			return;
	}
}
UINT32 creat_task1(void)		//PM2.5传感器
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 3;
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
//	UINT32 uwEvent;
	UINT32 count=0;
	
	//UINT32 tmp = 0;
	//float tmp = 0;
	while(1)
	{
		count++; 
		//printf("event_PM2d5_start\r\n");
		//uwEvent = LOS_EventRead(&WIFI_event, event_PM2d5_finish, LOS_WAITMODE_OR, 1500);//接收WiFi事件同步工作
		//printf("call_PM2d5_finish\r\n");
		//if(uwEvent == event_PM2d5_finish)
		//{
			//printf("event_PM2d5_finish\r\n");
			ESP8266_StaTcpClient_UnvarnishTest();
			
		//}
		if(uwRet !=LOS_OK)
		return;
	} 
}
UINT32 creat_task2(void)		//WiFi模块
{
	UINT32 uwRet = LOS_OK;
	uwRet = LOS_EventInit(&WIFI_event);				//初始化WiFi事件
  if(uwRet != LOS_OK)
	{
			printf("init event failed .\n");
			return -1;
	}
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 1;
	task_init_param.pcName = "task2";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}


VOID task3(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 uwEvent;
	UINT32 count=0;
	while(1)
	{
		count++;
		uwEvent = LOS_EventRead(&example_event, event_wait, LOS_WAITMODE_OR, 1500);
		if(uwEvent == event_wait)
		{
			UINT8 uwReadbuf;
			//uwRet = LOS_QueueRead(g_uwQueue, &uwReadbuf, 1, 0);
      if(uwRet != LOS_OK)
      {
          break;
      }
			if(uwReadbuf)
			{
				LED1_TOGGLE;
				uwRet = LOS_TaskDelay(1000);
				LED2_TOGGLE;
				uwRet = LOS_TaskDelay(1000);
			}
			
		}
		uwRet = LOS_TaskDelay(1000);
		if(uwRet !=LOS_OK)
			return;
} }
UINT32 creat_task3(void)	//小灯
{
	UINT32 uwRet = LOS_OK;
	
	uwRet = LOS_EventInit(&example_event);
  if(uwRet != LOS_OK)
	{
			printf("init event failed .\n");
			return -1;
	}
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task3";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task3;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

VOID task4(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
	while(1)
	{
		UINT8 keyValue = 0;
		count++;	

		keyValue = Key_Scan( macKEY1_GPIO_PORT, macKEY1_GPIO_PIN );
		if(keyValue)
		{
				//LED3_TOGGLE;
			uwRet = LOS_EventWrite(&example_event, event_wait);
			if(uwRet != LOS_OK)
			{
					printf("event write failed .\n");
					return;
			}
			LOS_EventClear(&example_event, ~example_event.uwEventID);
			
			//uwRet = LOS_QueueWrite(g_uwQueue, &keyValue, 1, 0);

      if(uwRet != LOS_OK)
      {
      }

		}
		uwRet = LOS_TaskDelay(600);
		if(uwRet !=LOS_OK)
			return;
	} 
}
UINT32 creat_task4(void)	//按键
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 2;
	task_init_param.pcName = "task4";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task4;
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
	
	uwRet = LOS_QueueCreate("queue", 1, &g_uwQueue, 0, 4); //创建消息队列
	if(uwRet != LOS_OK)
	{
	}

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
	uwRet = creat_task3();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	uwRet = creat_task4();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return LOS_OK;
}


/*********************************************END OF FILE**********************/

