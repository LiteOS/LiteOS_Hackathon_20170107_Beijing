#include "task.h"
#include "usart.h"

/**MQTT************/
#include "umqtt.h"
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
extern UINT32 g_TestTskHandle;

extern volatile int cid_num, lac_num; //lbs定位两个参数

uint8_t json_buf[64];

VOID task_mqtt(void)
{
	UINT32 uwRet = LOS_OK;
	uint8_t len;
	while(1){
		if(cid_num != 0 && lac_num != 0){
			sprintf((char*)json_buf,"{\"cid\":%d,\"lac\":%d}", cid_num, lac_num);
			len = strlen((const char*)json_buf);
			json_buf[len] = '\0';  /*末尾加\0*/
			usart3_printf("%s", json_buf);
		}
		
		uwRet = LOS_TaskDelay(3000);
		if(uwRet !=LOS_OK)
			return;
		
	}
}


UINT32 creat_task_mqtt(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;  /*优先保证通信*/
	task_init_param.pcName = "mqtt";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_mqtt;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}
