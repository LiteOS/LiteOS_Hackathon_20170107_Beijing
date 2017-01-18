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
#include "bsp_ov7725.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_led.h"
#include "bsp_usart1.h"
#include "bsp_lcd.h"
#include "bsp_dht11.h"
#include "bsp_esp8266.h"
#include "wifi_8266.h"
#include "frame_type.h"

#define DELATY_MS          2 

extern VOID Example_Interrupt(VOID);

static UINT32 MsgQueueID;
char abuf[50] = "test is message x\0";
uint8_t ShiftBuf[50];
uint8_t ShiftBuf1[50];

void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);
UINT32 g_TestTskHandle;
DHT11_Data_TypeDef  DHT11_Data;

extern uint8_t Ov7725_vsync;

Msg_SendInfoBuf RecMsg;
#define SEND_SUCCESS				0x55
#define NOT_SEND_SUCCESS			0xAA

char send_result = SEND_SUCCESS;

void Delay(__IO uint32_t nCount) //简单延时
{
	for(; nCount != 0; nCount--);
}

void hardware_init(void)
{
		LED_GPIO_Config();
		USARTx_Config();
		DHT11_Init();
		LCD_Init ();  
		/* ov7725 gpio 初始化 */
		Ov7725_GPIO_Config();
		/* ov7725 寄存器配置初始化 */
		while(Ov7725_Init() != SUCCESS);
//		ILI9341_GramScan ( 2 );
//		ILI9341_Clear ( 0, 0, 240, 320, macWHITE);
//		ILI9341_DispString_EN ( 0, 10, "  Welcome to Huawei LiteOS!", macWHITE, macRED );
//		ILI9341_DispString_EN ( 10, 40, "Task1 Running Count is: ", macWHITE, macBLUE );
//		ILI9341_DispString_EN ( 10, 60, "Task2 Running Count is: ", macWHITE, macBLUE );
//		ILI9341_DispString_EN ( 45, 90, "Task3: DHT11 DEMO ", macWHITE, macBLUE );
//		ILI9341_DispString_EN ( 30, 110, "Temperature is:", macWHITE, macBLACK );
//		ILI9341_DispString_EN ( 30, 130, "Humidity is:", macWHITE, macBLACK );
}


int SendToMsg(Msg_SendInfoBuf *pMsgPass)
{
    int RetVal = OK;
   // static Msg_SendInfoBuf Msg[50];
    uint8_t i = 0;

//    Msg[i].DataAddr = DataAddr;
//    Msg[i].length = length;
//    Msg[i].MsgType = MsgType;

    RetVal = LOS_QueueWrite(MsgQueueID, (char *)pMsgPass, 4, 0);
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
VOID task1(void)
{	
		/* ov7725 场信号线初始化 */
	VSYNC_Init();	
	Ov7725_vsync = 0;
		
	while(1)
	{
		LED1_TOGGLE;
		LED2_TOGGLE;

////	ILI9341_DispString_EN ( 200, 40, str, macWHITE, macRED );

		
		if( Ov7725_vsync == 2 )
		{
			FIFO_PREPARE;  			/*FIFO准备*/					
			ImagDisp();					/*采集并显示*/
			Ov7725_vsync = 0;			
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

void recv_process(unsigned char * recv_data)
{
	switch (recv_data[0])
	{
	case TYPE_ACK:
		{
			send_result = SEND_SUCCESS;
			break;
		}
	default:
		{
			break;
		}
	}
}
unsigned char send_buf[1024];
UINT32  tmp_count=0;
void send_process(unsigned char type)
{
	UINT32 len_send=0;
	char * pCh;
	uint8_t ucId;
	
	unsigned char tmp_string[20];
	tmp_count ++;
	
	//sprintf(tmp_string, "wang%d", tmp_count);
	//if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD," ) ) != 0 ) 
	{
		//ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
		//ESP8266_SendString ( DISABLE, tmp_string, strlen ( tmp_string ), Multiple_ID_0 );
	}
			send_buf[0] = 'I';
			send_buf[1] = 'E';
			send_buf[2] = 'y';
			send_buf[3] = 'e';
			send_buf[4] = type;
			send_buf[5] = RecMsg.ucPkgNum;
			send_buf[6] = RecMsg.ucIsEnd;
			send_buf[7] = RecMsg.sStayTime>>8;
			send_buf[8] = RecMsg.sStayTime;
			memset(&send_buf[9], 0, 4);
			len_send = 16;
			send_buf[13] = len_send>>8;
			send_buf[14] = len_send>>8;
			memcpy(&send_buf[14], &RecMsg.cProductrID[0], len_send);
			send_result = NOT_SEND_SUCCESS;
			ESP8266_SendString ( DISABLE, send_buf, 16+15, Multiple_ID_0 );
}

void init_wifi(void)
{
		uint8_t ucId, ucLen;
	uint8_t ucLed1Status = 0, ucLed2Status = 0, ucLed3Status = 0, ucBuzzerStatus = 0;

	char cStr [ 100 ] = { 0 }, cCh;

  char * pCh, * pCh1;

	DHT11_Data_TypeDef DHT11_Data;
	
	LOS_TaskDelay(100);
  //printf ( "\r\n???? ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();

	ESP8266_AT_Test ();

	ESP8266_Net_Mode_Choose ( AP );

  while ( ! ESP8266_CIPAP ( macUser_ESP8266_TcpServer_IP ) );

  while ( ! ESP8266_BuildAP ( macUser_ESP8266_BulitApSsid, macUser_ESP8266_BulitApPwd, macUser_ESP8266_BulitApEcn ) );	
	ESP8266_Enable_MultipleId ( ENABLE );
	
	while ( !	ESP8266_StartOrShutServer ( ENABLE, macUser_ESP8266_TcpServer_Port, macUser_ESP8266_TcpServer_OverTime ) );
	ESP8266_Inquire_ApIp ( cStr, 20 );

	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
}

void MsgTask(void)
{
		char * pCh;
		UINT32  send_time_count = 0;
		ESP8266_Init ();   
		UINT32 uwRet = LOS_OK, Rec;
    uint8_t Len = sizeof(ShiftBuf);
		init_wifi();

	memset((void *)&RecMsg, 0, sizeof(RecMsg));
    while(1)
    {
        

        uwRet = LOS_QueueRead(MsgQueueID, &Rec, sizeof(abuf), 1);
        if(uwRet != LOS_OK)
        {
						if ( strEsp8266_Fram_Record .InfBit .FramFinishFlag )
						{
							USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //????????
							strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
							
							if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "IEye" ) ) != 0 )
							{
									recv_process(pCh+strlen("IEye"));
							}
							else
							{
							}
								
							
							strEsp8266_Fram_Record .InfBit .FramLength = 0;
							strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
							
							USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //????????
							
						}
						if (NOT_SEND_SUCCESS==send_result)
						{
							send_time_count ++;
							if (send_time_count>=1000)
							{
									send_time_count = 0;
									send_process(0x02);
							}
						}
						else
						{
							memset((void *)&RecMsg, 0, sizeof(RecMsg));
						}
            continue;
        }
        memcpy(&RecMsg, (char *)Rec, sizeof(RecMsg));
				send_process(0x02);
    }
}

//VOID task2(void)
//{
//	UINT32 uwRet = LOS_OK;
//	UINT32 count=0;
//	char str[10];
//	while(1)
//	{
//		count++;
//		sprintf(str,"%d",count);
//		ILI9341_DispString_EN ( 200, 60, str, macWHITE, macRED );
//		printf("This is task 2,count is %d\r\n",count);
//		LED2_TOGGLE;
//		LED3_TOGGLE;
//		uwRet = LOS_TaskDelay(2000);
//		if(uwRet !=LOS_OK)
//			return;
//	}
//}

//UINT32 creat_task2(void)
//{
//	UINT32 uwRet = LOS_OK;
//	TSK_INIT_PARAM_S task_init_param;
//	task_init_param.usTaskPrio = 1;
//	task_init_param.pcName = "task2";
//	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
//	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
//	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
//	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
//	if(uwRet !=LOS_OK)
//	{
//		return uwRet;
//	}
//	return uwRet;
//}

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

//VOID DHT11_task(void)
//{
//	UINT32 uwRet = LOS_OK;
//	char cStr1[10];
//	char cStr2[10];
//	while(1)
//	{
//		if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
//		{
//			sprintf(cStr1,"%d.%d C",DHT11_Data.temp_int,DHT11_Data.temp_deci);
//			sprintf(cStr2,"%d.%d %%RH",DHT11_Data.humi_int,DHT11_Data.humi_deci);
//			ILI9341_DispString_EN ( 160, 110, cStr1, macWHITE, macRED );
//			ILI9341_DispString_EN ( 140, 130, cStr2, macWHITE, macRED );
//			printf("\r\n ??DHT11??,???%d.%d C,??? %d.%d %%RH",\
//			DHT11_Data.temp_int,DHT11_Data.temp_deci,DHT11_Data.humi_int,DHT11_Data.humi_deci);
//		}
//		else
//		{
//			printf("DHT11 ERROR!\r\n");
//		}
//		
//		uwRet = LOS_TaskDelay(2000);
//		if(uwRet !=LOS_OK)
//			return;
//	}
//}

//UINT32 creat_DHT11_task(void)
//{
//	UINT32 uwRet = LOS_OK;
//	TSK_INIT_PARAM_S task_init_param;
//	task_init_param.usTaskPrio = 0;
//	task_init_param.pcName = "DHT11_task";
//	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)DHT11_task;
//	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
//	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
//	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
//	if(uwRet !=LOS_OK)
//	{
//		return uwRet;
//	}
//	return uwRet;
//}

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
    uwRet = creat_MsgTask();
    if(uwRet !=LOS_OK)
    {
    	return uwRet;
    }

    uwRet = LOS_QueueCreate("MsgQueue", 20, &MsgQueueID, 0, 50);
    if(uwRet != LOS_OK)
    {
        printf("create queue failure!,error:%x\n",uwRet);
    }
	
	return uwRet;
}
/*********************************************END OF FILE**********************/
