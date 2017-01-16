#ifndef __GSM_H
#define __GSM_H

#include "string.h"
#include "los_typedef.h"

void GSM_Init(void);
void Connect(void);
void Disconnect(void);
void Send(const char* data);
void SendByte(UINT8 ch);
void Receive(void);

#endif

