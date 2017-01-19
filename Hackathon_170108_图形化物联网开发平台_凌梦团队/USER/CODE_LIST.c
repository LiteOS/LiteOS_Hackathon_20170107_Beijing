
#include "typedef.h"


uint16 InsDisc_CurrentIndex;
#define MaxNumber 30000
uint8 ByteCodeList[MaxNumber];

void CODE_Start()
{
	InsDisc_CurrentIndex = 0;
}
void CODE_AddIns( uint8 ins )
{
	ByteCodeList[InsDisc_CurrentIndex] = ins;
	++InsDisc_CurrentIndex;
}
uint8 CODE_ReadIns( uint16 addr )
{
	return ByteCodeList[addr];
}
void CODE_Save()
{
	
}
void CODE_Load()
{
	
}


