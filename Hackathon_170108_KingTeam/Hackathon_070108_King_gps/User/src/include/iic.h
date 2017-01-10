#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm32f10x.h"
unsigned char I2C1_ReadByte(unsigned char  id, unsigned char read_address);
void I2C1_WriteByte(unsigned char id,unsigned char write_address,unsigned char byte);
void Iic1_Init(void);
#endif
