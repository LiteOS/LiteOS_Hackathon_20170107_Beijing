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
#include "usart2.h"
extern UINT32 g_TestTskHandle;
extern volatile int16_t air_h, air_t;  /*from task*/
extern volatile uint16_t lux;          /*from task*/
#define MQTT_KEEP_ALIVE			60
#define MQTT_CLIENT_ID			"wea"
uint8_t json_buf[64];
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
/*inti...*/
void usr_umqtt_init(void)
{
    network_umqtt_init(&mqtt);
}

VOID task_mqtt(void)
{
	UINT32 uwRet = LOS_OK;
    uint8_t len;
	char *topic = "wea"; /*bugs:topic过长，通信失败*/
	//network_umqtt_init(&mqtt);
	int ret;
	static int cnt = 0;
	uint8_t buff[64];
	//char* msg = "hello,world";
    //usart2_printf("%d %d %d\r\n", air_t, air_h, lux);
		
   
	while(1)
	{
        if(!umqtt_circ_is_empty(&mqtt.txbuff)){
             ret = umqtt_circ_pop(&mqtt.txbuff, buff, 64);
			 usart1_send_nbyte((char*)buff, ret);
             //umqtt_process(&mqtt);
        }
        else
        {
                
            if(cnt == 5){
                cnt = 0;
                //network_umqtt_init(&mqtt);
                umqtt_ping(&mqtt);
            }
            else{
                sprintf((char*)json_buf,"{\"air_t\":%.1f,\"air_h\":%.1f,\"air_l\":%d}", air_t/10.0, air_h/10.0, lux);
                len = strlen((const char*)json_buf);
                json_buf[len] = '\0';  /*末尾加\0*/
                umqtt_publish(&mqtt, topic, (uint8_t*)json_buf, strlen((const char*)json_buf));  
            }
            cnt++;
                
            
        }   
        
        uwRet = LOS_TaskDelay(5000);
        if(uwRet !=LOS_OK)
            return;
        //umqtt_circ_push(&mqtt.rxbuff, buff, ret);
        //umqtt_process(&mqtt);
        
        //printf("ret = %d\r\n",ret);
        //Sleep(2000);
		
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
