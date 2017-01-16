/*
**************************
file info:
	author:fang.junpeng
	start time:2016-04-07
	email:tfzsll@126.com
	common tools functions for coding
***************************
*/

#include "baseTypeDef.h"
#include "sysTypeDef.h"

#ifndef UTIL_H
#define UTIL_H

//direct copy the byte without any check of array overflow
void byteCopy(u8* dst,u8* src,s16 Length);

u8 byteCompare(u8* dst,u8* src,s16 Length);


//a full clone of src buffer
void BufferSimpleCopy(Buffer_t*dst,Buffer_t*src);

void BufferCopy2U8(u8* dst,Buffer_t*src);


//get length of a string
u16 stringLength(u8* str);

//set the memory all to be byteValue you choose
void memorySet(u8* mem,u8 byteValue,s16 length);

u8 stringEqual(u8* src,u8* dst);

u8 stringEqualIgnoreCase(u8* src,u8* dst);

u8* u8toString(u8 num);

//math function
u8 Abs_U8(u8 data1,u8 data2);

void delay_ms(u16 n);

void delay_s(u16 n);

/*****server buffer operation*****/
//init a buffer
void Buffer_init(Buffer_t*);

//make a buffer clear
void Buffer_clear(Buffer_t*);

//make the index of buffer be zero
void Buffer_reset(Buffer_t*);

//add a u8 data to buffer
void Buffer_add(Buffer_t*,u8);

void Buffer_enable(Buffer_t*);

void Buffer_disable(Buffer_t*);


//check the buffer if has the data
u8 isBufferEabled(Buffer_t*);

//get the number of data in the buffer
u8 Buffer_dataLength(Buffer_t*);

u8 Buffer_MaxLength(void);

u8* getBuffer(Buffer_t*);

#endif

