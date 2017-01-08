/*
**************************
file info:
	author:fang.junpeng
	start time:2016-05-16
	email:tfzsll@126.com
	system typedef 
***************************
*/
#include "baseTypeDef.h"

#ifndef SYSTYPEDEF_H
#define SYSTYPEDEF_H

typedef void (*VOIDHOOK_t)(void);
typedef void (*USARTHOOK_t)(u16);

#define BUFFSIZE 255

typedef struct{
	u8 buffer[BUFFSIZE],enable,cnt,index;
}Buffer_t;


#endif

