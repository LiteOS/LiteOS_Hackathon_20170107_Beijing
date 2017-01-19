#include "common.h"

uint8_t ChipID[4] = {0};

void GetChipID (void)
{
  u32 ChipUniqueID[3];
  ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8);
  ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC);
  ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0);
  
  ChipID[0] = ChipUniqueID[0] & 0xff;
  ChipID[1] = ((ChipUniqueID[0] >> 8) & 0xff);
  ChipID[2] = ((ChipUniqueID[0] >> 16) & 0xff);
  ChipID[3] = ((ChipUniqueID[0] >> 24) & 0xff);
  //printf("######## ChipID: %X-%X-%X  ######## \r\n",ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
}
