#ifndef __USART3_H
#define __USART3_H	 
#include "types.h"  
#include "stm32f10x.h"
//#include "config.h"
//#ifndef NDEBUG
#define USART3_MAX_RECV_LEN		600					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		600					//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

extern uint8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern uint8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节


void usart3_init(uint32 bound);				//串口2初始化 
void usart3_printf(char* fmt,...);
void USART3_Puts(uint8_t * str);
void USART3_Putc(unsigned char c);
//#endif
#endif













