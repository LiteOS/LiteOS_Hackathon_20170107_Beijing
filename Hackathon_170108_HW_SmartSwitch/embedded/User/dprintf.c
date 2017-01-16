#include "dprintf.h"

#define DebugUart eUart2

void dprintf(u8* str){
    USARTSendByteString(str,DebugUart);
}

void dprintfln(u8* str){
    USARTSendLine(str,DebugUart);
}

void dprintByte(u8 data){
    USARTSendData(data,DebugUart);
}

