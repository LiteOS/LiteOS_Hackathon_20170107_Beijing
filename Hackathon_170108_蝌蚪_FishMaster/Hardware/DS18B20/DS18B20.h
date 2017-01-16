#ifndef __DS18B20_H
#define	__DS18B20_H

#include "stm32f10x.h"

#define DS18B20_IN1_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define  DS18B20_IN1_DATA_IN()	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

void DS18B20_IN1_GPIO_Config(void);

uint8_t DS18B20_IN1_Init(void);

u8 DS18B20_IN1_Get_Temp(void);


#endif 







