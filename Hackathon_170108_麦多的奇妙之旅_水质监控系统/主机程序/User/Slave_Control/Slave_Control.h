#ifndef __Slave_Control_c
#define __Slave_Control_c
#include "common.h"


extern uint8_t Connect_Way;
extern long SlaveNeedAddBuf[20];
extern uint8_t TmpOffGetRec;
extern uint8_t PHOffGetRec_Neutral;
extern uint8_t PHOffGetRec_Acidity;
extern uint8_t PHOffGetRec_Basicity;

void Can_ReceMsgProcess (CanRxMsg* RxMessage);    //从机数据处理
void Test_AllowInNet (void);
void ZGB_ReceMsgProcess (uint8_t Uart2_ReceiveDat[]);
void TouchSlaveProcess_ZGB (void);
void TouchSlaveProcess_CAN (void);
UINT32 creat_GetSlaveDatTask (void);

#endif
