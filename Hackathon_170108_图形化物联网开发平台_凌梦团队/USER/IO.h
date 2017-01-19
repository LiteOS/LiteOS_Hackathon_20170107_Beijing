
#ifndef _IO_h_
#define _IO_h_

#include "typedef.h"

void IO_Init( void );
void IO_Close( void );

void IO_DirWrite( uint8 i, uint8 d );

void IO_PullWrite( uint8 i, uint8 d );

void IO_OutWrite( uint8 i, uint8 d );
uint8 IO_OutRead( uint8 i );

uint8 IO_InRead( uint8 i );

void IO_AnalogOpen( uint8 i, uint8 d );
int32 IO_AnalogRead( uint8 i );


#endif


