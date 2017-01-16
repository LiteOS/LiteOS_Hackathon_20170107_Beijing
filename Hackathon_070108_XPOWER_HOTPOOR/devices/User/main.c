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
void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);
UINT32 g_TestTskHandle;
UINT32 key_flag=0;
void Delay(__IO uint32_t nCount) //简单延时
{
	for(; nCount != 0; nCount--);
}
void hardware_init(void)
{
	  
	  KEY_Init();
	  Adc_Init();
		USART_Config();
	  LOS_HwiCreate(37, 0,0,USART1_IRQHandler,NULL);
}
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 ad_value=0;
	UINT32 ad_count=5;
  UINT32 play_flag=0;
	while(1)
	{	
		while(ad_count--)
		{
			if(Get_Adc_Average(3)>20)ad_value++;		      		
		}		
		ad_count=5;
		if(ad_value>3)
		{
			ad_value=0;
			GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5);
			delay_ms(3000);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5);
		}				
    if(KEY_Scan(0)==1)key_flag++;
		if(key_flag==1)
          {printf("02$1!");
					 key_flag=0;
					 GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5); 
					 delay_ms(100);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5);
					}
				
		uwRet = LOS_TaskDelay(200);//系统延时1S
		if(uwRet !=LOS_OK)
		return;
	} 
}
UINT32 creat_task1(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 1;//任务优先级，0最高
	task_init_param.pcName = "task1";//???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;//任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//创建任务
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}
VOID task2(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 shake_num=0;
	while(1)
	{
		if(((USART1_RX_BUF[1]=='1')||(USART1_RX_BUF[1]=='0'))&&(USART1_RX_BUF[2]=='$'))
				 {	
					 if(USART1_RX_BUF[3]=='x') {GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5);
																			delay_ms(3000);
																			GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5);}
					 
					 else{
           	shake_num=USART1_RX_BUF[3]-48;
            while(shake_num--)	
						{   
							 GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5); 
							 delay_ms(100);
					  	 GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5);
							 delay_ms(100);
						}
					}
					 USART1_RX_STA=0;
					 USART1_RX_BUF[2]='0';
				 }		
		uwRet = LOS_TaskDelay(200);
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

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	hardware_init();
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

	return LOS_OK;
}
/*********************************************END OF FILE**********************/
