
#ifndef _CODE_LIST_h_
#define _CODE_LIST_h_

#include "typedef.h"

void CODE_Start( void );
void CODE_AddIns( uint8 ins );
uint8 CODE_ReadIns( uint16 addr );
void CODE_Save( void );
void CODE_Load( void );

#endif

