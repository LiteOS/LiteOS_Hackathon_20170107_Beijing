#ifndef __Bsp_Uart2_Zgb_H
#define __Bsp_Uart2_Zgb_H
#include "common.h"

extern uint8_t Zgb_SendBuf[17];

void USART2_Config (void);
void USART2_SendBuf(uint8_t *str);
void USART2_SendByte(u8 dat);

#endif
