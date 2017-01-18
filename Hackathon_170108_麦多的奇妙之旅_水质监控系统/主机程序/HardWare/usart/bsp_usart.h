#ifndef __USART_H
#define	__USART_H

#include "common.h"

void USART_Config(void);
void USART1_SendByte(u8 dat);
void USART1_SendBuf(uint8_t *str);
void Uart1_Interrupt(void);

#endif /* __USART_H */
