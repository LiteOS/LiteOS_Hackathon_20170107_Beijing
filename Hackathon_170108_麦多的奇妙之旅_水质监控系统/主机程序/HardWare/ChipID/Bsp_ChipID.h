#ifndef __Bsp_ChipID_H
#define __Bsp_ChipID_H
#include "common.h"

extern long ChipID;             //CPU ID
extern char ChipID_String[25];  //字符串形式的ChipID  最后一个字节存的结束符

void GetChipID (void);
//void Chip2String (uint32_t Num, char* String);

#endif
