#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_dht11.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_ili9341_lcd.h"
#include "frame_type.h"

#define SUCCESS				0x55
#define NOT_SUCCESS			0xAA

char send_result = SUCCESS;

void recv_process(unsigned char * recv_data)
{
	char * pCh;
	uint8_t ucId;

	switch (recv_data[0])
	{
	case TYPE_ACK:
		{
			send_result = SUCCESS;
			if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD," ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				ESP8266_SendString ( DISABLE, "wang55", strlen ( "wang55" ), ( ENUM_ID_NO_TypeDef ) ucId );
			}
			break;
		}
	default:
		{
			if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD," ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
				ESP8266_SendString ( DISABLE, "wangAA", strlen ( "wangAA" ), ( ENUM_ID_NO_TypeDef ) ucId );
			}
			break;
		}
	}
}

void send_process(unsigned char type)
{
	send_result = NOT_SUCCESS;
}


/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucId, ucLen;
	uint8_t ucLed1Status = 0, ucLed2Status = 0, ucLed3Status = 0, ucBuzzerStatus = 0;

	char cStr [ 100 ] = { 0 }, cCh;

  char * pCh, * pCh1;

	DHT11_Data_TypeDef DHT11_Data;
	
	LOS_TaskDelay(100);
  //printf ( "\r\n正在配置 ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();

	ESP8266_AT_Test ();

	ESP8266_Net_Mode_Choose ( AP );

  while ( ! ESP8266_CIPAP ( macUser_ESP8266_TcpServer_IP ) );

  while ( ! ESP8266_BuildAP ( macUser_ESP8266_BulitApSsid, macUser_ESP8266_BulitApPwd, macUser_ESP8266_BulitApEcn ) );	
	ESP8266_Enable_MultipleId ( ENABLE );
	
	while ( !	ESP8266_StartOrShutServer ( ENABLE, macUser_ESP8266_TcpServer_Port, macUser_ESP8266_TcpServer_OverTime ) );
	ESP8266_Inquire_ApIp ( cStr, 20 );
	ILI9341_DispString_EN ( 30, 130, "Humidity is:BB", macWHITE, macBLACK );
	//printf ( "\r\n本模块WIFI为%s，密码开放\r\nAP IP 为：%s，开启的端口为：%s\r\n手机网络助手连接该 IP 和端口，最多可连接5个客户端\r\n",
  //         macUser_ESP8266_BulitApSsid, cStr, macUser_ESP8266_TcpServer_Port );
	
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
	
	while ( 1 )
	{		
		if (0)
		{
			send_process(0x01);
		}
		if ( strEsp8266_Fram_Record .InfBit .FramFinishFlag )
		{
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
			
			if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "IEye" ) ) != 0 )
			{
					recv_process(pCh+strlen("IEye"));
			}
			else
			{
					if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD," ) ) != 0 ) 
					{
						ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
						ESP8266_SendString ( DISABLE, "wang", strlen ( "wang" ), ( ENUM_ID_NO_TypeDef ) ucId );
					}
			}
				
			
		  strEsp8266_Fram_Record .InfBit .FramLength = 0;
	    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
			
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断
			
		}
		
	}
	
		
}


