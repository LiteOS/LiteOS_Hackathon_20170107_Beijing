/*
**************************
file info:
	author:fang.junpeng
	start time:2017-01-07
	debug printf
***************************
*/

#include "uartConfig.h"
#define DebugUart eUart3

void dprintf(u8* str){
    USARTSendByteString(str,DebugUart);
}

