#ifndef __DO_H
#define	__DO_H

#include "stm32f10x.h"

#define Relay1ON GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define Relay1OFF GPIO_ResetBits(GPIOB,GPIO_Pin_8)

void DO_Config(void);

#endif
