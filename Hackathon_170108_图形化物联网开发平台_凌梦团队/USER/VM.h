
#ifndef _VM_h_
#define _VM_h_

#include "typedef.h"

extern uint32 PC;

void VM_Reset( void );
void VM_Run( void );
void VM_AddInterruptEvent( uint8 d );
void VM_ClearSysValue( void );

extern uint32 ReadData;
extern bool WriteNotRead;
extern bool Running;
extern bool EA;


#endif
