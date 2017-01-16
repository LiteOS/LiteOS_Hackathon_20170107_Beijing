#ifndef __SPI2_H
#define __SPI2_H

#include "stm32f10x.h"

#define HIGH	1
#define LOW		0

void SPI2_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
#endif

