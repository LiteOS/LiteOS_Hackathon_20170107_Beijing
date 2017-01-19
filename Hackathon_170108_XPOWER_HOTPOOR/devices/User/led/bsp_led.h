#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"

#define key_a4   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) 

#define key_get   1	
void KEY_Init(void);
u8 KEY_Scan(u8 mode);  	
void delay_ms(u16 time);

void  Adc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 times);

#endif