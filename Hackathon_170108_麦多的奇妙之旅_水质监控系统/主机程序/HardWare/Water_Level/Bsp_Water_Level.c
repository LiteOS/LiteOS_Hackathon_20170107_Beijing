#include "common.h"

/*
 * 函数名：WatLevl_GPIO_Config
 * 描述  ：IO初始化
 * 输入  ：无
 * 返回  ：无
 */
void WatLevl_GPIO_Config (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_WatLevl,ENABLE);	//使能复用功能时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_WatLevl;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
  GPIO_Init(GPIOx_WatLevl, &GPIO_InitStructure);	
  GPIO_SetBits(GPIOx_Powdon,GPIO_Pin_PowDon);
}

/*
 * 函数名：Water_Level_Init
 * 描述  ：水位检测初始化
 * 输入  ：无
 * 返回  ：无
 */
void Water_Level_Init (void)
{
  WatLevl_GPIO_Config();
}

/*
 * 函数名：CheckWaterLevel
 * 描述  ：检测水位
 * 输入  ：无
 * 返回  ：无
 */
void CheckWaterLevel (void)
{
  if (GPIO_ReadInputDataBit (GPIOx_WatLevl, GPIO_Pin_WatLevl))    //正常
  {
    if (((Flash_FifoBuf[5] >> 24) & 0x01) == 1)
    {
      WarmClearStatue++;
      Flash_FifoBuf[5] &= ~(0x3 << 24);
    }
  }
  else      //异常
  {
    Flash_FifoBuf[5] |= (1 << 24);   
    WarmUpCount[0][3] = 0;    
  }
}
