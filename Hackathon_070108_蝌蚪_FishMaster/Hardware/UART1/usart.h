#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

#define USART_REC_LEN  			200  			//定义最大接收字节数 200
#define EN_USART1_RX 			1				//使能（1）/禁止（0）串口1接收

extern	u8		USART_RX_BUF[USART_REC_LEN];
extern	u16 	USART_RX_STA; 
extern	u8  	UsartCount;
extern 	u8 		UART1_CMD; 						//接收命令
extern	u8 		num;

void USART1_Config(u32 bound);
void UART1_Send_Str(unsigned char *s);
void UART1_Send_Array(unsigned char send_array[],unsigned char num);
void USART1_Send_Byte(u8 byte);
void USART1_IRQHandler(void);

#endif


