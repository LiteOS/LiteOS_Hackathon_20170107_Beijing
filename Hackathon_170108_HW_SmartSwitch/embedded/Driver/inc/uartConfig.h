/*
**************************
file info:
	author:fang.junpeng
	start time:2016-05-03
	email:tfzsll@126.com
	usartConfig 
***************************
*/

#include "baseTypeDef.h"

#ifndef USARTCONFIG_H
#define USARTCONFIG_H

#define UARTBUFFERSIZE 40
typedef struct{
	u8 enable,cnt;
	u8 buffer[UARTBUFFERSIZE];
}UartBuffer_t;

typedef enum{
	eUart1=0,eUart2,eUart3
}eUartType;

void USART_Config(void);

void USART_newConfig(u32 BaudRate,eUartType uart);

void USARTSendData(u16 SendData,eUartType uartType);
void USARTSendByteArrary(u8* data,u8 length,eUartType uartType);

void USARTSendWordArray(u16*data,u8 length,eUartType uartType);
void USARTSendByteString(u8*str,eUartType uartType);
void USARTSendWordString(u16*str,eUartType uartType);

void USARTSendLine(u8*str,eUartType uartType);



//serial com HOOK
void setUartHOOK(void(*fun)(u16),eUartType uartType);
void resetUartHOOK(eUartType uartType);
void resetAllUartHOOK(void);



#endif


