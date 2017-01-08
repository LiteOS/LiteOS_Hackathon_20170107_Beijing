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
#include "usart3.h"
#include "stdlib.h"
volatile int cid_num, lac_num; //lbs定位两个参数
extern volatile uint8_t rcv_jsonpack_flag; 
extern uint8_t rx_buf_len;
/*task handle*/
extern UINT32 g_TestTskHandle;
/*全局变量*/
volatile uint8_t rcv_jsonpack_flag = 0; 
/*create task*/
extern uint8 USART3_RX_BUF[USART3_MAX_RECV_LEN];

VOID get_lbs_station_task(void)
{
    UINT32 uwRet = LOS_OK;
    const char* dot_delim = ",";
    const char* equ_delim = "=";
    char *p, *cid, *cid_num_str, *lac_num_str;
	while(1)
	{
		printf("task_lbs\r\n");
		if(rcv_jsonpack_flag == 1){
            rcv_jsonpack_flag = 0;
            usart3_rx_disable();
            //USART3_RX_BUF[rx_buf_len] = '\0';
            printf("\r\nbuf = %s\r\n", USART3_RX_BUF);
            p = strtok((char*)USART3_RX_BUF, dot_delim);   /*+LBS: LAC =4245 前半部分*/
            cid = strtok(NULL, dot_delim);
            /*读取lac*/
            strtok(p, equ_delim);
            lac_num_str  = strtok(NULL, equ_delim);
            if(lac_num_str != NULL){
                lac_num = atoi(lac_num_str);
                printf("lac_num = %d\r\n", lac_num);
            }
            /*读取cid*/
            strtok(cid, equ_delim);
            cid_num_str = strtok(NULL, equ_delim);
            if(cid_num_str != NULL){
                cid_num = atoi(cid_num_str);
                printf("cid_num = %d\r\n", cid_num);
            }       
        }
        else{
            usart3_rx_enable();
			printf("send cmd\r\n");
            usart3_printf("foryu#AT+LBS\r\n");
            memset(USART3_RX_BUF, USART3_MAX_RECV_LEN, '\0');
        }
        //usart3_printf("hello");
        //printf("task3\r\n");
		uwRet = LOS_TaskDelay(5000);
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task_lbs(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 1;
	task_init_param.pcName = "lbs";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)get_lbs_station_task;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
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
  
	while(1){
        //usart3_printf("hello");
        printf("test\r\n");
		uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task_test(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 2;
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

