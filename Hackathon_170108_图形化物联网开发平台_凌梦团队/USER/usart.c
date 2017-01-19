
#include "usart.h"
#include <stdio.h>

#include "typedef.h"

#include "proto.h"

bool Serial_OK;
uint8 Serial_Data;

//初始化串口2
void Serial_Init(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	//PA2:USART2_TX  ; PA3:USART2_RX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 

	USART_Init(USART2, &USART_InitStructure);

	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_Cmd(USART2, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitTypestucture;
   NVIC_InitTypestucture.NVIC_IRQChannel=USART2_IRQn;
   NVIC_InitTypestucture.NVIC_IRQChannelCmd=ENABLE;
   NVIC_InitTypestucture.NVIC_IRQChannelPreemptionPriority=1;
   NVIC_InitTypestucture.NVIC_IRQChannelSubPriority=1;
   
   NVIC_Init(&NVIC_InitTypestucture);
   
   USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
   
   Serial_OK = false;
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		Serial_OK = true;
        Serial_Data = USART_ReceiveData(USART2);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		
		Deal( Serial_Data );
    }
}

void Serial_write(uint32 tempc)
{
	//while((USART2->SR&0x0040)==0);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART2->DR=tempc;
}
void Serial_SendString(u8 *tempp)
{
	while(*tempp!=0)
	{
		Serial_write(*tempp++);
	}
}

#if 1
#pragma import(__use_no_semihosting)                              
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;         
void _sys_exit(int x) 
{
	x = x; 
}
int fputc(int ch, FILE *f)
{
	
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_SendData(USART2, (u8) ch);
	return ch;
}
#endif
