/******************** (C) COPYRIGHT 2012 WildFire Team **************************

**********************************************************************************/
#include "usart2.h"

u8 USART2_RX_BUF[USART_REC_LEN2];			//接收缓冲,最大USART_REC_LEN个字节.
u8 Usart2Count;								//接收计数
u8 receiveOK = 0;

/*******************************************************************************
* 函数名	: USART2_Config()
* 描述	    : USART2配置
* 输入参数  : u32 bound
* 返回参数  : 无
********************************************************************************/
void USART2_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_DeInit(USART2);   										//复位串口2
    USART2->SR&=~(1<<7);    										//清发送成功标志

	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure); 
	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

/*******************************************************************************
* 函数名	: USART2_Send_Byte()
* 描述	    : USART2发送一个字节
* 输入参数  : u8 byte
* 返回参数  : 无
********************************************************************************/
void USART2_Send_Byte(unsigned char byte)
{
        USART_SendData(USART1, byte);
        while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
}

/*******************************************************************************
* 函数名	: UART2_Send_Str()
* 描述	    : UART2发送字符串
* 输入参数  : unsigned char *s
* 返回参数  : 无
********************************************************************************/
void UART2_Send_Str(unsigned char *s)
{
	unsigned char i=0;

	while(s[i]!='\0')
	{
		USART_SendData(USART2,s[i]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
		i++;
	}
}

/*******************************************************************************
* 函数名	: UART2_Send_Array()
* 描述	    : UART2发送数组
* 输入参数  : unsigned char send_array[],unsigned char num
* 返回参数  : 无
********************************************************************************/
void UART2_Send_Array(unsigned char send_array[],unsigned char num) 
{
	unsigned char i=0;

	while(i<num)
	{
		USART_SendData(USART2,send_array[i]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		i++;
	}
}

/*******************************************************************************
* 函数名	: USART2_IRQHandler()
* 描述	    : USART2中断
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
void USART2_IRQHandler(void)
{   
	static char rCnt = 0;  
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{  
		USART2_RX_BUF[rCnt] = USART_ReceiveData(USART2);
		rCnt++;
		if(rCnt>=5)
		{
			rCnt = 0;
			receiveOK = 1;
		}
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
