//#include "delay.h"
#include "usart3.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "timer.h"
/*?????????????*/
//#ifndef NDEBUG
//??????? 	
uint8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//????,??USART3_MAX_RECV_LEN???.
uint8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//????,??USART3_MAX_SEND_LEN??
uint8_t rx_buf_len = 0;


void usart3_init(uint32 bound)
{  

        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB??
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //??3????
        USART_DeInit(USART3);  //????3
        //USART3_TX   PB10
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
        GPIO_Init(GPIOB, &GPIO_InitStructure); //???PB10
   
        //USART3_RX	  PB11
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
        GPIO_Init(GPIOB, &GPIO_InitStructure);  //???PB11
	
        USART_InitStructure.USART_BaudRate = bound;//????????9600;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
        USART_InitStructure.USART_Parity = USART_Parity_No;//??????
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????
        
		//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //Enable rx enable, 
        USART_Init(USART3, &USART_InitStructure); //?????	3
        USART_Cmd(USART3, ENABLE); 
        /*初始化缓冲区*/
        memset(USART3_RX_BUF, USART3_MAX_RECV_LEN, '\0');
        
}
void usart3_rx_enable(void)
{
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}
void usart3_rx_disable(void)
{
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
}
//??3,printf ??
//???????????USART3_MAX_SEND_LEN??
void usart3_printf(char* fmt,...)  
{  
        u16 i,j; 
        va_list ap; 
        va_start(ap,fmt);
        vsprintf((char*)USART3_TX_BUF,fmt,ap);
        va_end(ap);
        i=strlen((const char*)USART3_TX_BUF);		//?????????
        for(j=0;j<i;j++)							//??????
        {
            while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //????,??????   
            USART_SendData(USART3,USART3_TX_BUF[j]); 
        } 
}
void USART3_IRQHandler(void)
{
	//u16 str1;
	timer2_enable();
  	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  	{
		 USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		 //str1=USART_ReceiveData(USART3);
         USART3_RX_BUF[rx_buf_len] = USART_ReceiveData(USART3);
		 ++rx_buf_len;
		 //返回接收数据
		 //USART_SendData(USART3, str1);
		 //while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
		 USART_ITConfig( USART3,USART_IT_RXNE, ENABLE);
  	}
}
void USART3_Putc(unsigned char c)
{
    USART_SendData(USART3, c);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void usart3_send_nbyte(char *str, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		USART3_Putc(*str++);
	}
}
//#endif
 



















