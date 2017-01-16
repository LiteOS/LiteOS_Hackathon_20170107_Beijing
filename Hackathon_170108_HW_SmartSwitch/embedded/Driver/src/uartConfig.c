#include "uartConfig.h"
#include "sysTypeDef.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

//uart hook
USARTHOOK_t uart1Hook=0,uart2Hook=0,uart3Hook=0;

//buffer define
UartBuffer_t Uart1Buffer={0,0,{0}},Uart3Buffer={0,0,{0}};

void USART_NVIC_Configuration(void);

void USART_Config(void)
{
    //uart nvic config
    //USART_NVIC_Configuration();
    //uart config
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/***************************uart1 config****************************/

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 使用IO端口配置 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART1 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
	//
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE);// USART1使能
	
	/***************************uart2 config****************************/
	//0.enable usart2 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//1.gpio config for usart2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//2.uart2 config
	USART_Init(USART2, &USART_InitStructure);
	//3.enable uart interrupt and enable uart
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);

	/***************************uart3 config****************************/
	//enable uart3 and gpioB clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    //uart config
    USART_InitStructure.USART_BaudRate = 2400;
	//gpio config
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	
	USART_Cmd(USART3,ENABLE);

}

void USART_newConfig(u32 BaudRate,eUartType uart){
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    //uart common setting
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    switch(uart){
        case eUart1:
            /* 使能 USART1 时钟*/
        	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 
            //gpio config
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
            GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
            USART_Init(USART1, &USART_InitStructure);  //初始化USART1
        	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
        	USART_Cmd(USART1, ENABLE);// USART1使能
            break;
        case eUart2:
            //0.enable usart2 clock
        	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
        	//1.gpio config for usart2
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
          	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        	GPIO_Init(GPIOA, &GPIO_InitStructure);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        	GPIO_Init(GPIOA, &GPIO_InitStructure);
            //2.uart2 config
        	USART_Init(USART2, &USART_InitStructure);
        	//3.enable uart interrupt and enable uart
        	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
        	USART_Cmd(USART2,ENABLE);
            break;
        case eUart3:
           	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
        	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
        	//config
        	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
          	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        	GPIO_Init(GPIOB, &GPIO_InitStructure); 

        	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
        	USART_Init(USART3, &USART_InitStructure);
        	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	
        	USART_Cmd(USART3,ENABLE);
            break;
        default:
            break;
    }
}


//uart nvic config
void USART_NVIC_Configuration(void){
    NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    //if use usart3 change #if
#if 1
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

//send data
void USARTSendData(u16 SendData,eUartType uartType){
	if(uartType == eUart1){
	    USART_SendData(USART1,SendData);
	    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	else if(uartType == eUart2){
		USART_SendData(USART2,SendData);
	    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}else{
		USART_SendData(USART3,SendData);
	    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}

void USARTSendByteArrary(u8* data,u8 length,eUartType uartType){
	u8 i=0;
	if(0==data)
		return;
	while(i<length)
		USARTSendData(data[i],uartType);
}

void USARTSendWordArray(u16*data,u8 length,eUartType uartType){
	u8 i=0;
	if(0==data)
		return;
	while(i<length)
		USARTSendData(data[i],uartType);
}

void USARTSendByteString(u8*str,eUartType uartType){
	if(0==str)
		return;
	while(*str)
		USARTSendData(*str++,uartType);
}
void USARTSendWordString(u16*str,eUartType uartType){
	if(0==str)
		return;
	while(*str)
		USARTSendData(*str++,uartType);
}

void USARTSendLine(u8*str,eUartType uartType){
	u8 newLine[]="\r\n";
	USARTSendByteString(str,uartType);
	USARTSendByteString(newLine,uartType);
}

/*********for set UartHook***********/
void setUartHOOK(void(*fun)(u16),eUartType uartType){
	switch(uartType)
	{
	case eUart1:
		uart1Hook = fun;
		break;
	case eUart2:
		uart2Hook = fun;
		break;
	case eUart3:
		uart3Hook = fun;
		break;
	default:
		break;
	}
}

void resetUartHOOK(eUartType uartType){
	switch(uartType)
	{
	case eUart1:
		uart1Hook = 0;
		break;
	case eUart2:
		uart2Hook = 0;
		break;
	case eUart3:
		uart3Hook = 0;
		break;
	default:
		break;
	}
}

void resetAllUartHOOK(void){
	uart1Hook = uart2Hook = uart3Hook =0;
}


