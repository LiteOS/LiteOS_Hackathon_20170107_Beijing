
#include "typedef.h"

#include "Mem.h"
#include "CODE_LIST.h"

//#define BASE_LENGTH 3000
uint8 BASE[BASE_LENGTH];

void Mem_Clear()
{
	for( uint16 i = 0; i < BASE_LENGTH; ++i ) {
		BASE[i] = 0;
	}
}

uint8 Mem_GetCode_uint8( uint16 Addr )
{
	return CODE_ReadIns( Addr );
}
uint16 Mem_GetCode_uint16( uint16 Addr )
{
	uint16 d = CODE_ReadIns( Addr+1 ); d <<= 8;
	d += CODE_ReadIns( Addr );
	return d;
}
uint32 Mem_GetCode_uint32( uint16 Addr )
{
	uint32 d = CODE_ReadIns( Addr+3 ); d <<= 8;
	d += CODE_ReadIns( Addr+2 ); d <<= 8;
	d += CODE_ReadIns( Addr+1 ); d <<= 8;
	d += CODE_ReadIns( Addr );
	return d;
}

void Mem_Set_uint8( uint16 Addr, uint8 d )
{
	BASE[Addr] = d;
}
void Mem_Set_uint16( uint16 Addr, uint16 d )
{
	BASE[Addr] = d % 256; d /= 256;
	BASE[Addr+1] = d;
}
void Mem_Set_uint32( uint16 Addr, uint32 d )
{
	BASE[Addr] = d % 256; d /= 256;
	BASE[Addr+1] = d % 256; d /= 256;
	BASE[Addr+2] = d % 256; d /= 256;
	BASE[Addr+3] = d;
}
uint8 Mem_Get_uint8( uint16 Addr )
{
	return BASE[Addr];
}
uint16 Mem_Get_uint16( uint16 Addr )
{
	uint16 d = BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return d;
}
uint32 Mem_Get_uint24( uint16 Addr )
{
	uint32 d = BASE[Addr+2]; d <<= 8;
	d += BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return d;
}
uint32 Mem_Get_uint32( uint16 Addr )
{
	uint32 d = BASE[Addr+3]; d <<= 8;
	d += BASE[Addr+2]; d <<= 8;
	d += BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return d;
}

void Mem_Set_int8( uint16 Addr, int8 d )
{
	BASE[Addr] = d;
}
void Mem_Set_int16( uint16 Addr, int16 dd )
{
	uint32 d = (uint32)dd;
	BASE[Addr] = d % 256; d /= 256;
	BASE[Addr+1] = d;
}
void Mem_Set_int32( uint16 Addr, int32 dd )
{
	uint32 d = (uint32)dd;
	BASE[Addr] = d % 256; d /= 256;
	BASE[Addr+1] = d % 256; d /= 256;
	BASE[Addr+2] = d % 256; d /= 256;
	BASE[Addr+3] = d;
}
int8 Mem_Get_int8( uint16 Addr )
{
	return BASE[Addr];
}
int16 Mem_Get_int16( uint16 Addr )
{
	uint16 d = BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return (int16)d;
}
int32 Mem_Get_int32( uint16 Addr )
{
	uint32 d = BASE[Addr+3]; d <<= 8;
	d += BASE[Addr+2]; d <<= 8;
	d += BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return (int32)d;
}



