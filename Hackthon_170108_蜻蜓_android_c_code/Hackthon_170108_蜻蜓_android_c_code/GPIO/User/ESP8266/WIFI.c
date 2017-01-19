#include "WIFI.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_esp8266.h"


volatile uint8_t ucTcpClosedFlag = 0;

char cStr [ 1500 ] = { 0 };
#define event_PM2d5_finish 	0x00000002


/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	
	char cStr [ 100 ] = { 0 };
	char tmp[20];
		
  printf ( "\r\n正在配置 ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();							//使能 macESP8266模块
	
	ESP8266_AT_Test ();									//对WF-ESP8266模块进行AT测试启动
	
	ESP8266_Net_Mode_Choose ( STA );		//选择WF-ESP8266模块的工作模式

  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	//WF-ESP8266模块连接外部WiFi
	
	ESP8266_Enable_MultipleId ( DISABLE );//配置是否多连接
	ESP8266_Inquire_ApIp(tmp,10);
	printf("%s",tmp);
	printf("\r\n");
	printf("------------------------");
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );//WF-ESP8266模块连接外部服务器
	printf("linkSer OK");
	while ( ! ESP8266_UnvarnishSend () );//等待配置WF-ESP8266模块进入透传发送成功
	
	printf ( "\r\n配置 ESP8266 完毕\r\n" );
	
	
	while ( 1 )
	{		
		float *pm2d5 = 0;
		UINT32 uwEvent;
		//printf ( "\r\n1gjfyu\r\n" );
		uwEvent = LOS_EventRead(&WIFI_event, event_PM2d5_finish, LOS_WAITMODE_OR, 1500);//接收WiFi事件同步工作
		printf ( "\r\n配置 123 完毕\r\n" );
		if(uwEvent == event_PM2d5_finish)
		{
			LOS_QueueRead(g_uwQueue, &pm2d5, 4, 0);
			
			sprintf ( cStr,"%.2f\r\n", *pm2d5);
			
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //发送数据
			//ESP8266_SendString ( ENABLE, "500\r\n", 0, Single_ID_0 );
		}
			
		LOS_TaskDelay ( 5000 );
		
		if ( ucTcpClosedFlag )                                             //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //确认失去连接后重连
			{
				printf ( "\r\n正在重连热点和服务器 ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
			
		}
	}
	
		
}


