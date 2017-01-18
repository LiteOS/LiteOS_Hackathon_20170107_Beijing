#include "common.h"

int32_t OfficDatBuf[5] = {0};
uint32_t Flash_FifoBuf[256] = {0};                  //Flash在软件上的BUF

void FlashInit (void)
{
  Flash_ReadOfficDat();
  if(OfficDatBuf[4] != 0xaaaaaaaa)
  {
    OfficDatBuf[0] = 0;
    OfficDatBuf[1] = 0;
    OfficDatBuf[2] = 0;
    OfficDatBuf[3] = 0;
    OfficDatBuf[4] = 0xaaaaaaaa;
    Flash_WriteOfficDat();
  }
}
  


void Flash_ReadOfficDat (void)
{
  int i =0;
  u32 faddr = FLASH_OffSAVE_ADDR;
  
  for (i = 0; i < 5; i++)
  {
    OfficDatBuf[i] = *(vu32*)faddr;
    faddr += 4;
  }
}

void Flash_WriteOfficDat (void)
{  
  int i = 0;
  uint32_t faddr = FLASH_OffSAVE_ADDR;
  
  FLASH_Unlock();				//解锁			
  FLASH_ErasePage(FLASH_OffSAVE_ADDR);
  for (i = 0; i < 256; i++)
  {
    FLASH_ProgramWord(faddr, OfficDatBuf[i]);	
    faddr += 4;
//    IWDG_Feed();
  }
  		
  FLASH_Lock();								//上锁
}

/*
 * 函数名：Flash_Clear
 * 描述  ：主要是要擦除存放从机数量的数据地址 
 * 输入  ：无
 * 返回  ：无
 */
//void Flash_Clear (void)
//{
//  FLASH_ErasePage(FLASH_SAVE_ADDR);
//}

/*
 * 函数名：Flash_Clear
 * 描述  ：一次读64位数据 
 * 输入  ：无
 * 返回  ：无
 */
//void Flash_ReadPageToBUF (void)
//{
//  int i =0;
//  u32 faddr = FLASH_SAVE_ADDR;
//  
//  for (i = 0; i < 256; i++)
//  {
//    Flash_FifoBuf[i] = *(vu32*)faddr;
//    faddr += 4;
//  }
//}


/*
 * 函数名：Flash_Clear
 * 描述  ：将整个BUF写入Flash 
 * 输入  ：无
 * 返回  ：无
 */
//void Flash_WriteBufToPage (void)
//{
//  int i = 0;
//  uint32_t faddr = FLASH_SAVE_ADDR;
//  
//  FLASH_Unlock();				//解锁			
//  FLASH_ErasePage(FLASH_SAVE_ADDR);
//  for (i = 0; i < 256; i++)
//  {
//    FLASH_ProgramWord(faddr, Flash_FifoBuf[i]);	
//    faddr += 4;
//    IWDG_Feed();
//  }
//  		
//  FLASH_Lock();								//上锁
//}

/*
 * 函数名：Flash_Clear
 * 描述  ：主机中查找有无此用户数据 ，有则返回所在位置
 * 输入  ：SlaveID：要查询的ID
 * 返回  ：所在位置
 */
uint8_t Search_SlaveID (uint32_t SlaveID)
{
  int SlaveNum = 0;
  int i = 0;
  
  SlaveNum = Flash_FifoBuf[0];    //第一个32位存的是当前从机数量
  for (i = 0; i < SlaveNum; i++)
  {
    if (SlaveID == (Flash_FifoBuf[7 + i * 3]))
    {
      return (7 + i * 3);
    }
  }
  return 0;
}

/*
 * 函数名：Flash_Clear
 * 描述  ：主机中查找有无此用户数据 ，有则返回所在位置
 * 输入  ：SlaveID：要添加的ID
 * 返回  ：SUCCESS：成功
 *         FLASH_ID_EXIST：已添加过
 */
uint8_t Add_SlaveID (uint32_t SlaveID)
{
  int statue = 0;
  int SlaveNum = 0;
  statue = Search_SlaveID(SlaveID);
  
  if (statue != 0)
  {
    return FLASH_ID_EXIST;
  }
  else
  {
    SlaveNum = Flash_FifoBuf[0];
    Flash_FifoBuf[7 + SlaveNum * 3] = SlaveID;
    Flash_FifoBuf[7 + SlaveNum * 3 + 1] = 0x0000;
    Flash_FifoBuf[7 + SlaveNum * 3 + 2] = 0x0000;
    Flash_FifoBuf[0] = ++SlaveNum;

    return SUCCESS;
  }
}
