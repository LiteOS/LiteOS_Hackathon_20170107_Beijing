#include "common.h"

/*
 * 函数名：PowDon_GPIO_Config
 * 描述  ：IO初始化
 * 输入  ：无
 * 返回  ：无
 */
void PowDon_GPIO_Config (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_PowDon,ENABLE);	//使能复用功能时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_PowDon;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
  GPIO_Init(GPIOx_Powdon, &GPIO_InitStructure);	
  GPIO_SetBits(GPIOx_Powdon,GPIO_Pin_PowDon);
  
}


/*
 * 函数名：PowDon_Init
 * 描述  ：掉电检测初始化
 * 输入  ：无
 * 返回  ：无
 */
void PowDon_Init (void)
{
  PowDon_GPIO_Config();
}

/*
 * 函数名：CheckPowerDown
 * 描述  ：检测是否掉电
 * 输入  ：无
 * 返回  ：无
 */
void CheckPowerDown (void)
{
  if (GPIO_ReadInputDataBit (GPIOx_Powdon, GPIO_Pin_PowDon))    //没电
  {
    Flash_FifoBuf[5] |= (1 << 20);
  }
  else    //有电
  {
    if (((Flash_FifoBuf[5] >> 20) & 0x01) == 1)
    {
      WarmClearStatue++;
      Flash_FifoBuf[5] &= ~(0x3 << 20);
      WarmUpCount[0][5] = 0;
    }
  }
}
