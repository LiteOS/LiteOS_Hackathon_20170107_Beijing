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
#include "Msg.h"
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
#include "bsp_SysTick.h"
#include "test.h"
#include "bsp_beep.h"



#define DELATY_MS          2 


extern VOID Example_Interrupt(VOID);

static UINT32 MsgQueueID;
char abuf[] = "test is message x";
char bbuf[] = "TEST is MESSAGE x";
uint8_t ShiftBuf[50];
uint8_t ShiftBuf1[50];

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
    ILI9341_DispString_EN ( 0, 10, "  Welcome to Huawei LiteOS!", macWHITE, macRED );
    ILI9341_DispString_EN ( 10, 40, "Task1 Running Count is: ", macWHITE, macBLUE );
    ILI9341_DispString_EN ( 10, 60, "Task2 Running Count is: ", macWHITE, macBLUE );
    ILI9341_DispString_EN ( 45, 90, "Task3: DHT11 DEMO ", macWHITE, macBLUE );
    ILI9341_DispString_EN ( 30, 110, "Temperature is:", macWHITE, macBLACK );
    ILI9341_DispString_EN ( 30, 130, "Humidity is:", macWHITE, macBLACK );
}


int SendToMsg(uint32_t DataAddr, uint32_t length, uint32_t MsgType)
{
    int RetVal = OK;
    static MsgStr_t Msg[50];
    uint8_t i = 0;

    Msg[i].DataAddr = DataAddr;
    Msg[i].length = length;
    Msg[i].MsgType = MsgType;

    RetVal = LOS_QueueWrite(MsgQueueID, (char *)&Msg[i], 4, 0);
    if (RetVal != LOS_OK)
    {
        RetVal = ERROR;
    }

    i++;
    if (i >= 50)
    {
        i = 0;
    }

    return RetVal;
}


void task1(void)
{ 
	char str[10];
	UINT32 uwRet = LOS_OK;
	UINT32 count=0, i=0;
    UINT32 uwlen = sizeof(abuf);

	while(1)
	{
		LED1_TOGGLE;
		LED2_TOGGLE;

		count++;
		sprintf(str,"%d",count);
////	ILI9341_DispString_EN ( 200, 40, str, macWHITE, macRED );

        abuf[uwlen -2] = '0' + i;
        i++;

        uwRet = SendToMsg((uint32_t)abuf, uwlen, DISP_TEST);
        if(uwRet != LOS_OK)
        {
            uwRet = ERROR;
        }

        LOS_TaskDelay(DELATY_MS);
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


void MsgTask(void)
{
	UINT32 uwRet = LOS_OK, Rec;
    MsgStr_t RecMsg;
    uint8_t Len = sizeof(ShiftBuf);

    while(1)
    {
        memset((void *)&RecMsg, 0, sizeof(RecMsg));

    	LED2_TOGGLE;
    	LED3_TOGGLE;

        uwRet = LOS_QueueRead(MsgQueueID, &Rec, sizeof(abuf), 1);
        if(uwRet != LOS_OK)
        {
            continue;
        }
        memcpy(&RecMsg, (char *)Rec, sizeof(RecMsg));

        if (Len > RecMsg.length)
        {
            Len = RecMsg.length;
        }


        switch (RecMsg.MsgType)
        {
            case    DISP_INFO:
                memcpy(ShiftBuf, (char *)(RecMsg.DataAddr), Len);
//              ILI9341_DispString_EN (30, 150, (void *)(ShiftBuf), macWHITE, macRED );
                break;

            case    DISP_TEST:
                memcpy(ShiftBuf1, (char *)(RecMsg.DataAddr), Len);
//              ILI9341_DispString_EN (30, 170, (void *)(ShiftBuf1), macWHITE, macRED );
                break;

            default:
                break;
        }
    }
}


UINT32 creat_MsgTask(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;

	task_init_param.usTaskPrio = 1;
	task_init_param.pcName = "MsgTask";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)MsgTask;
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
	char str[10];
	UINT32 uwRet = LOS_OK;
	UINT32 count=0, i=0;
    UINT32 uwlen = sizeof(bbuf);

	while(1)
	{
		LED1_TOGGLE;
		LED2_TOGGLE;

		count++;
		sprintf(str,"%d",count);
////	ILI9341_DispString_EN ( 200, 40, str, macWHITE, macRED );

        bbuf[uwlen -2] = '0' + i;
        i++;

        uwRet = SendToMsg((uint32_t)bbuf, uwlen, DISP_INFO);
        if(uwRet != LOS_OK)
        {
            uwRet = ERROR;
        }

        LOS_TaskDelay(DELATY_MS);
	}
}


UINT32 creat_DHT11_task(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 1;
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

//  LOS_TaskLock();

	uwRet = creat_task1();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}

    uwRet = creat_MsgTask();
    if(uwRet !=LOS_OK)
    {
    	return uwRet;
    }

    uwRet = creat_DHT11_task();
    if(uwRet !=LOS_OK)
    {
    	return uwRet;
    }

    uwRet = LOS_QueueCreate("MsgQueue", 20, &MsgQueueID, 0, 50);
    if(uwRet != LOS_OK)
    {
        printf("create queue failure!,error:%x\n",uwRet);
    }

//  LOS_TaskUnlock();

	return LOS_OK;
}
/*********************************************END OF FILE**********************/
