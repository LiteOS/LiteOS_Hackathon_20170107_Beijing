#include "stm32f10x.h"
#include "usart.h"
#include "stdio.h"

u8 USART_RX_BUF[USART_REC_LEN];     		//接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA=0;       					//接收状态标记
u8 UsartCount;
u8 UART1_CMD; 								//接收命令
u8 num = 0;

/* 加入以下代码,支持printf函数,而不需要选择use MicroLIB */
#if 0
#pragma import(__use_no_semihosting)
struct __FILE 
{ 
	int handle;
}; 

FILE __stdout;       

/*******************************************************************************
* 函数名	: fputc()
* 描述	    : USART1输出字符
* 输入参数  : int ch, FILE *f
* 返回参数  : 无
********************************************************************************/
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif 

/*使用microLib的方法*/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	

    return ch;
}

/*******************************************************************************
* 函数名	: USART1_Config()
* 描述	    : USART1配置
* 输入参数  : u32 bound
* 返回参数  : 无
********************************************************************************/
void USART1_Config(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);   										//复位串口1
    USART1->SR&=~(1<<7);    										//清发送成功标志

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;						//PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);							//初始化PA9

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);							//初始化PA10
	
	USART_InitStructure.USART_BaudRate = bound;						//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure);						//初始化串口
	
#if EN_USART1_RX
    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;		//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);									//根据指定的参数初始化VIC寄存器
	
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					//开启中断
#endif
    USART_Cmd(USART1, ENABLE);                    					//使能串口
}

/*******************************************************************************
* 函数名	: USART1_Send_Byte()
* 描述	    : USART1发送一个字节
* 输入参数  : u8 byte
* 返回参数  : 无
********************************************************************************/
void USART1_Send_Byte(u8 byte)
{
	USART_SendData(USART1, byte);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
}

/*******************************************************************************
* 函数名	: UART1_Send_Str()
* 描述	    : UART1发送字符串
* 输入参数  : unsigned char *s
* 返回参数  : 无
********************************************************************************/
void UART1_Send_Str(unsigned char *s)
{
	unsigned char i=0;

	while(s[i]!='\0')
	{
		USART_SendData(USART1,s[i]);
		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
		i++;
	}
}

/*******************************************************************************
* 函数名	: UART1_Send_Array()
* 描述	    : UART1发送数组
* 输入参数  : unsigned char send_array[],unsigned char num
* 返回参数  : 无
********************************************************************************/
void UART1_Send_Array(unsigned char send_array[],unsigned char num) 
{
	unsigned char i=0;

	while(i<num)
	{
		USART_SendData(USART1,send_array[i]);
		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
		i++;
	}    
}

/*******************************************************************************
* 函数名	: USART1_IRQHandler()
* 描述	    : USART1中断
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
void USART1_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{	
		UART1_CMD = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据			
	}
}
