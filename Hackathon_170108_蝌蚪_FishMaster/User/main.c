/*----------------------------------------------------------------------------------------------------
	                  养鱼专家  FishMaster
------------------------------------------------------------------------------------------------------
  名称: 养鱼专家
  团队: 蝌蚪
  队员: 蔡石磊	email: 15512180705@163.com			WeChat: 15512180705
		包永迪	email: 1693779179@qq.com			WeChat: 15712858060
		顾孙成	email: gu332523602@gmail.com		WeChat: 18330115758
		张通	email: zhangtong@southjun.com		WeChat: 15830192870
  日期: 2017-01-07
  版本: v1.0
------------------------------------------------------------------------------------------------------*/

#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
#include "los_hwi.h"

#include "stm32f10x.h"
#include "bsp_led.h"
#include "usart.h"	
#include "usart2.h"
#include "OLED_I2C.h"  
#include "DS18B20.h"
#include "Do.h"
#include "stdio.h"

void Delay(__IO u32 nCount);						//简单延时函数

UINT8 CMD[3] = {0xFD, 0xFD, 0xFD};					//TDS传感器发送命令
UINT8 Tx_Data[3] = {0};
u8 TEMPL[2] = {0};
u8 TEMPH[2] = {0};
u8 TDS1[2] = {0};									//个位TDS
u8 TDS2[2] = {0};									//十位TDS
u8 TDS3[2] = {0};									//百位TDS
u8 TDS4[2] = {0};									//千位TDS
u32 TDSNUM; //TDS数字
UINT32 g_TestTskHandle;

/*******************************************************************************
* 函数名	: Delay()
* 描述	    : 简单延时
* 输入参数  : __IO uint32_t nCount
* 返回参数  : 无
********************************************************************************/
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

/*******************************************************************************
* 函数名	: hardware_init()
* 描述	    : 外围硬件初始化
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
void hardware_init(void)
{
	LED_GPIO_Config();
	USART1_Config(57600);									//USART1串口初始化 
	USART2_Config(9600);									//USART2串口初始化 
	LOS_HwiCreate(37, 0, 0, USART1_IRQHandler, NULL);		//创建串口1接收中断
	LOS_HwiCreate(38, 0, 0, USART2_IRQHandler, NULL);		//创建串口2接收中断
	DS18B20_IN1_GPIO_Config(); 								//初始化18B20
	DO_Config();											//继电器初始化
    I2C_Configuration();									//初始化IIC
	OLED_Init();											//初始化OLED
	OLED_Start();											//OLED开机界面
	Delay(9999999); 										//延时一段时间为了显示开机界面
}

/*******************************************************************************
* 函数名	: task1()
* 描述	    : 任务1获取温度上报
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 TX_NUM;
	
	while(1)
	{
		Tx_Data[0] = DS18B20_IN1_Get_Temp();						//获取温度
		if(receiveOK == 1)											//串口接收完成
		{
			Tx_Data[1] = USART2_RX_BUF[1];      					//中断中获取TDS值低位
			Tx_Data[2] = USART2_RX_BUF[2];
			receiveOK = 0;
		}
		TX_NUM = Tx_Data[2] + (Tx_Data[1] << 8) + (Tx_Data[0] << 16); 
		printf("%X", TX_NUM); 										//发送数据给WIFI模块这里一定要发字符串，否则云平台不认识
        OLED_CLS();
		
		OLED_ShowStr(2, 0, (unsigned char*)"TEMP:", 2);
		OLED_ShowStr(75, 0, (unsigned char*)"C", 2);				//温度界面
	    OLED_ShowStr(2, 4, (unsigned char*)"TDS:", 2);
		OLED_ShowStr(75, 4, (unsigned char*)"PPM", 2);				//温度界面
		TEMPL[0] = (Tx_Data[0] / 10) + 0x30;
		TEMPH[0] = (Tx_Data[0] % 10) + 0x30;
	    TDS4[0] = (Tx_Data[2] / 10) + 0x30;							//TDS千位
		TDS3[0] = (Tx_Data[1] / 100) + 0x30;						//TDS百位
		TDS2[0] = ((Tx_Data[1] % 100) / 10) + 0x30;					//TDS十位
		TDS1[0] = ((Tx_Data[1] % 100) % 10) + 0x30;					//TDS个位
		
		OLED_ShowStr(50, 0, (unsigned char*)TEMPL, 2);
		OLED_ShowStr(58, 0, (unsigned char*)TEMPH, 2);
		
		OLED_ShowStr(42, 4, (unsigned char*)TDS3, 2);				//显示百位
		OLED_ShowStr(50, 4, (unsigned char*)TDS2, 2);				//显示十位
		OLED_ShowStr(58, 4, (unsigned char*)TDS1, 2);				//显示个位
		
		if(UART1_CMD == '5')
		{
			Relay1ON;
		}
		else
		{
			Relay1OFF;
		}
		
		LED1_TOGGLE;
		
		uwRet = LOS_TaskDelay(1000);								//操作系统延时
		if(uwRet !=LOS_OK)
			return;
	}
}

/*******************************************************************************
* 函数名	: creat_task1()
* 描述	    : 创建任务1
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
UINT32 creat_task1(void) 
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task1";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle, &task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

/*******************************************************************************
* 函数名	: task2()
* 描述	    : 任务2 TDS水质监测
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
VOID task2(void)
{
	UINT32 uwRet = LOS_OK;
	while(1)
	{
		UART2_Send_Array(CMD, 3); 										//发送FD FD FD 获取TDS
		LED2_TOGGLE;
		uwRet = LOS_TaskDelay(3000);
		if(uwRet !=LOS_OK)
			return;
	}
}

/*******************************************************************************
* 函数名	: creat_task2()
* 描述	    : 创建任务2
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
UINT32 creat_task2(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task2";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle, &task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

/*******************************************************************************
* 函数名	: osAppInit()
* 描述	    : 应用初始化
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	hardware_init();
	LED2_ON;
	Delay(0x0FFFFF);
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
