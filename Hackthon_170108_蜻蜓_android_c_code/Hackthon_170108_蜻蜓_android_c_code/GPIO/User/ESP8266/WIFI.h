#ifndef  __WIFI_H
#define	 __WIFI_H



#include "stm32f10x.h"
#include "los_task.h"
#include "bsp_esp8266.h"
#include "los_queue.h"

/********************************** 用户需要设置的参数**********************************/
#define      macUser_ESP8266_ApSsid                       "TechTemple Office"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "f39ac7e2d0"           //要连接的热点的密钥

#define      macUser_ESP8266_TcpServer_IP                 "10.1.17.250"      //要连接的服务器的 IP
#define      macUser_ESP8266_TcpServer_Port               "65535"               //要连接的服务器的端口



/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;
extern UINT32 g_uwQueue;
extern EVENT_CB_S WIFI_event;

/********************************** 测试函数声明 ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );



#endif

