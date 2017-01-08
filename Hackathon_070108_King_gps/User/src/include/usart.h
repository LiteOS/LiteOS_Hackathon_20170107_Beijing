#ifndef __USART_H
#define __USART_H
#include <stdio.h> 
#include <string.h>
void USART1_Init(long baud_rate);
int fputc(int ch, FILE *f);
void USART1_Putc(unsigned char c);
void USART1_Puts(char * str);
void USART1_IRQHandler(void);
void usart1_init(long baud_rate);
void usart1_send_nbyte(char *str, int len);
#endif
