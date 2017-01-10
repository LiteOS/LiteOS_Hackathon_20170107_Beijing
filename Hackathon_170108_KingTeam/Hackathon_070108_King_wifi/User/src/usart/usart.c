/*-------------------------------------------------------------*/
/* 串口1收发函数                                                */
/*-------------------------------------------------------------*/
#include "usart.h"
#include "stm32f10x.h"


void USART1_Init(long baud_rate)
{
          USART_InitTypeDef USART_InitStructure;
          GPIO_InitTypeDef 	GPIO_InitStructure;

          RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
          /* Configure USART1 Tx (PA9) as alternate function push-pull */
          GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
          GPIO_Init(GPIOA, &GPIO_InitStructure);

          /* Configure USART1 Rx (PA10) as input floating */
          GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
          GPIO_Init(GPIOA, &GPIO_InitStructure);
          USART_InitStructure.USART_BaudRate = baud_rate;
          USART_InitStructure.USART_WordLength = USART_WordLength_8b;
          USART_InitStructure.USART_StopBits = USART_StopBits_1;
          USART_InitStructure.USART_Parity = USART_Parity_No ;
          USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
          USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
          
          USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //Enable rx enable, 
          USART_Init(USART1, &USART_InitStructure);
          USART_Cmd(USART1, ENABLE);

}

void usart1_init(long baud_rate)
{
	USART1_Init(baud_rate);
}
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
  return (ch);
}

void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

void USART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

void usart1_send_nbyte(char *str, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		USART1_Putc(*str++);
	}
}
void USART1_IRQHandler(void)
{
	u16 str1;  
  	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  	{
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		str1=USART_ReceiveData(USART1);
		USART_SendData(USART1, str1);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
		USART_ITConfig( USART1,USART_IT_RXNE, ENABLE);
  	}
}
