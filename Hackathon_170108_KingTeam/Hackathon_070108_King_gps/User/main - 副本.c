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
#include "usart2.h"
#include "mcu_init.h"
#include "ult.h"
#include "sht10.h"
#include "xassert.h"
#include "bh1750_s.h"
#include "usart.h"
#include "umqtt.h"
#include <string.h>
void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);
UINT32 g_TestTskHandle;
int16_t air_h, air_t;
uint16_t lux = 0;;

#define MQTT_KEEP_ALIVE			30
#define MQTT_CLIENT_ID			"123"
static void handle_message(struct umqtt_connection __attribute__((unused))*conn,
		char *topic, uint8_t *data, int len)
{
	//usart2_printf("%s %s\n", topic, data);
}

static uint8_t mqtt_txbuff[64];
static uint8_t mqtt_rxbuff[2];

static struct umqtt_connection mqtt = {
	.txbuff = {
		.start = mqtt_txbuff,
		.length = sizeof(mqtt_txbuff),
	},
	.rxbuff = {
		.start = mqtt_rxbuff,
		.length = sizeof(mqtt_rxbuff),
	},
	.message_callback = handle_message,
};

void network_umqtt_init(struct umqtt_connection* conn)
{
    umqtt_init(conn);
    umqtt_circ_init(&conn->txbuff);
    umqtt_circ_init(&conn->rxbuff);
    umqtt_connect(conn, MQTT_KEEP_ALIVE, MQTT_CLIENT_ID);
}

void Delay(__IO uint32_t nCount) //简单延时
{
	for(; nCount != 0; nCount--);
}
void get_air_temp_humi(void)
{
        
	uint16_t humi_val, temp_val;
	uint8_t err = 0, checksum = 0;
	float humi_val_real = 0.0; 
	float temp_val_real = 0.0;
	err += sht10_measure(&temp_val, &checksum, TEMP);		  //获取温度测量值
	err += sht10_measure(&humi_val, &checksum, HUMI);		  //获取湿度测量值
	xassert(err == 0);
	if(err != 0)
	{
		sht10_conreset();
	}
	else
	{
		sht10_calculate(temp_val, humi_val, &temp_val_real, &humi_val_real); //计算实际的温湿度值
	} 
	air_h = (int16_t)(humi_val_real*10);
	air_t = (int16_t)(temp_val_real*10);
	//sensor_assem1.air_t1 = (int16_t)(temp_val_real*10);
	//sensor_assem1.air_h1 = (int16_t)(humi_val_real*10);
	err = 0;
}
void hardware_init(void)
{
	//LED_GPIO_Config();
	//USART_Config();
	RCC_Configuration(); /* 配置单片机系统时钟*/
	NVIC_Configuration();/* 配置嵌套中断向量*/
	Systick_Init(72);    /*初始化Systick工作时钟*/
	usart2_init(9600);
	usart1_init(9600);
	sht10_init();
    
}
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
    while(1)
    {
        lux = get_lux_val();
        usart2_printf("lux = %d\r\n",lux);
        uwRet = LOS_TaskDelay(5000);
        if(uwRet !=LOS_OK){
            return;
        }
    }

    
}
UINT32 creat_task1(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 4;//任务优先级，0最高
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
	while(1)
	{
		get_air_temp_humi();
        usart2_printf("air_t:%d,air_h:%d\r\n", air_t, air_h);
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

VOID task3(void)
{
	UINT32 uwRet = LOS_OK;
	while(1)
	{
		
        //printf("task3\r\n");
		uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task3(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 2;
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
	char *topic = "cmd";
	network_umqtt_init(&mqtt);
	int ret;
	static int cnt = 0;
	uint8_t buff[64];
	char* msg = "hello,world";
    
	while(1)
	{
		umqtt_publish(&mqtt, topic, (uint8_t*)msg, strlen(msg));
        //umqtt_publish(&mqtt, topic, (uint8_t*)mix_msg[cnt], strlen(mix_msg[cnt]));
        if(!umqtt_circ_is_empty(&mqtt.txbuff)){
             ret = umqtt_circ_pop(&mqtt.txbuff, buff, 64);
			 usart1_send_nbyte((char*)buff, ret);
			 //printf("123");
             //writeToSerialPort(h, (char*)buff, ret);
             //send(sockfd, (const char*)buff, ret, 0);
        }
        if(++cnt > 5){
            cnt = 0;
            umqtt_ping(&mqtt);
        }
        //umqtt_circ_push(&mqtt.rxbuff, buff, ret);
        umqtt_process(&mqtt);
        
        //printf("ret = %d\r\n",ret);
        //Sleep(2000);
		uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task4(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
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
