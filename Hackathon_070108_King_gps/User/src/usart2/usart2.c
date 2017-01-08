//#include "delay.h"
#include "usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "stm32f10x.h"
#include "types.h"
#include "timer.h"
/*调试用串口输出输出驱动函数*/
//#ifndef NDEBUG
//串口接收缓存区 	
//串口接收缓存区 	
//uint8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
uint8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节
//uint8_t rx_buf_len = 0;
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void usart2_init(uint32 bound)
{  
        //A2 TX2 A3 RX2
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	// GPIOB时钟
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE); //串口2时钟使能
        USART_DeInit(USART2);  //复位串口2
        //USART2_TX   PA2
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
        GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
        //USART2_RX	  PA3
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3
	
        USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
        USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //Enable rx enable, 
        USART_Init(USART2, &USART_InitStructure);
        USART_Cmd(USART2, ENABLE);

        //使能接收中断
        //USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	
        //设置中断优先级
       

}

//串口2,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void usart2_printf(char* fmt,...)  
{  
        u16 i,j; 
        va_list ap; 
        va_start(ap,fmt);
        vsprintf((char*)USART2_TX_BUF,fmt,ap);
        va_end(ap);
        i=strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度
        for(j=0;j<i;j++)							//循环发送数据
        {
            while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
            USART_SendData(USART2,USART2_TX_BUF[j]); 
        } 
}

void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void USART2_Puts(uint8_t * str)
{
    while(*str)
    {
        USART_SendData(USART2, *str++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}

void USART2_IRQHandler(void)
{
	u16 str1;   
    //uint8_t i = 0;
  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{
     USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
     USART_SendData(USART2, str1);
     while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
     USART_ITConfig( USART2,USART_IT_RXNE, ENABLE);
  	}
}
//#endif
 



















