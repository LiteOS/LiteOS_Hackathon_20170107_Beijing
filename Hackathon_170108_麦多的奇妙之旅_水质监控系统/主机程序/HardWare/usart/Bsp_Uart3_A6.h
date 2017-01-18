#ifndef __Bsp_Uart3_A6_H
#define __Bsp_Uart3_A6_H
#include "common.h"

#define UnFinished    0
#define Finished      1

extern char Uart3_ReceiveDat[500];
extern char DatFromServer[300];
extern int Uart3_BufPoint;
extern uint8_t Uart3_ReceStatue;    //接收结束标志位

void USART3_Config(void);
void USART3_SendBuf(uint8_t *str);
void Uart3_BufClear (void);

#endif
