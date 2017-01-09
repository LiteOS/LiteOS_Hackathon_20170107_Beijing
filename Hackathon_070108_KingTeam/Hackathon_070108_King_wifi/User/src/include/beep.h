#ifndef _BEEP_H
#define _BEEP_H
#include "stm32f10x.h"
#define BEEP_PORT	GPIOA
#define BEEP_OUT	8
void beep_init(void);
void beep_off(void);
void beep_on(void);
#endif
