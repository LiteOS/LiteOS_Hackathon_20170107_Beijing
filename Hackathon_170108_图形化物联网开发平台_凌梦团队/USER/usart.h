
#ifndef __USART_H
#define __USART_H

#include "typedef.h"

#include "stm32f4xx.h"

extern bool Serial_OK;
extern bool Serial_Data;


void Serial_Init(u32 BaudRate);
void Serial_write(uint32 tempc);
void Serial_SendString(u8 *tempp);

#endif

