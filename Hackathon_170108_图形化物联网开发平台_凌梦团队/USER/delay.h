#ifndef __DELAY_H
#define __DELAY_H

#include "typedef.h"

void Timer_Init( void );
void Timer_SetTimeMs( uint16 t );
bool Timer_isTimeOut( void );

void Timer_delay_ms(uint16 nms);
void Timer_delay_us(uint32 nus);

#endif

