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
#include "usart2.h"
#include "stdlib.h"
#include "jsmn.h"
#include "io_ctr.h"
#include "ult.h"
volatile int cid_num, lac_num; //lbs定位两个参数
extern volatile uint8_t rcv_jsonpack_flag; 
extern uint8_t rx_buf_len;
/*task handle*/
extern UINT32 g_TestTskHandle;
/*全局变量*/
volatile uint8_t rcv_jsonpack_flag = 0; 
/*create task*/
extern uint8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

VOID get_ctr_cmd_task(void)
{
    UINT32 uwRet = LOS_OK;
	int i;
	int r;
	static uint8_t cmd_ligth;
	jsmn_parser p;
	jsmntok_t t[16]; /* We expect no more than 128 tokens */
	char *str[2];  /*这里还没搞明白，为什么是2*/
	jsmn_init(&p);
	while(1){
		//usart3_printf("1\r\n");
		if(rcv_jsonpack_flag == 1){
			rcv_jsonpack_flag = 0;
			/*每次解析的数据长度应该为接收的长度，而不是缓冲区的总长度*/
			r = jsmn_parse(&p, (const char *)USART2_RX_BUF, rx_buf_len, t, sizeof(t)/sizeof(t[0]));
			//把长度清零，重新开始接收一帧数据
			rx_buf_len = 0;
			//加入，接收到一帧数据后，先进行处理，是否需要关闭串口接收中断
			/* Assume the top-level element is an object */
			if (r < 1 || t[0].type != JSMN_OBJECT) {
					break;
			}
			/* Loop over all keys of the root object */
			for (i = 1; i < r; i++) {
				 if (jsoneq((const char *)USART2_RX_BUF, &t[i], "ctr_light") == 0) {
						/* We may want to do strtol() here to get numeric value */
						cmd_ligth = strtol((const char *)USART2_RX_BUF + t[i+1].start, str, 10);
						i++;
				}
				 
			}
		}
		usart2_printf("cmd_light:%d\r\n",cmd_ligth);
		if(cmd_ligth == 1){
			open_dev();
			
			
		}
			
		if(cmd_ligth == 0)
			close_dev();
		uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
			return;
		
	}

}

UINT32 creat_task_ctr(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "ctr";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)get_ctr_cmd_task;
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
	task_init_param.usTaskPrio = 1;
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

