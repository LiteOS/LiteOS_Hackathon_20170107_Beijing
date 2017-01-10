#include "task.h"
/**system**********/
#include "usart2.h"
#include "mcu_init.h"
#include "ult.h"
#include "sht10.h"
#include "xassert.h"
#include "bh1750_s.h"
#include "usart.h"
/**liteos**********/
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
/*task handle*/
extern UINT32 g_TestTskHandle;
/*全局变量*/
volatile int16_t air_h, air_t;
volatile uint16_t lux;
/*create task*/
VOID task_get_air_temp_humi(void)
{
	UINT32 uwRet = LOS_OK;
	while(1)
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
        usart2_printf("air_t:%d,air_h:%d\r\n", air_t, air_h);
		uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
            return;
	} 
}
UINT32 creat_task_get_air_temp_humi(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 1;
	task_init_param.pcName = "get_th";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_get_air_temp_humi;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

VOID task_get_lux(void)
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
UINT32 creat_task_get_lux(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 1;//任务优先级，0最高
	task_init_param.pcName = "get_lux";//???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_get_lux;//任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//创建任务
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}
/*for the test*/
VOID task_test(void)
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

UINT32 creat_task_test(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 3;
	task_init_param.pcName = "test";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_test;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

