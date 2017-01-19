#ifndef __Touch2Master_H
#define __Touch2Master_H

#include "common.h"

extern uint8_t Connect_Way; 
extern uint8_t MsgSendStatue;
extern u8 ZGB_Buf[14];
extern u8 CanBuf[8];

void ZGB_ReceMsgProcess (uint8_t Uart2_ReceiveDat[]);
void Zgb_ReqGetNet (void);
void Can_ReceMsgProcess (CanRxMsg* RxMessage);
void Can_ReqGetNet (void);
UINT32 creat_GetNetAgainTask (void);
UINT32 creat_SendAgainTask (void);

#endif
