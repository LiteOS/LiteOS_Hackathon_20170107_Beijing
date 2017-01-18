#include "common.h"

void WatLevl_GPIO_Config (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_WatLevl,ENABLE);	//使能复用功能时钟
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx_WatLevl, GPIO_PinSource_WatLevl);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_WatLevl;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
   GPIO_Init(GPIOx_WatLevl, &GPIO_InitStructure);	
//  GPIO_SetBits(GPIOx_Powdon,GPIO_Pin_PowDon);
}


void Water_Level_Init (void)
{
  WatLevl_GPIO_Config();
}

void CheckWaterLevel (void)
{
  if (GPIO_ReadInputDataBit (GPIOx_WatLevl, GPIO_Pin_WatLevl))
  {
    Flash_FifoBuf[5] &= ~(0x3 << 24);
//    printf("the water level is : 正常\r\n");
  }
  else
  {
    Flash_FifoBuf[5] |= (1 << 24);   
//    printf("the water level is : 偏低\r\n");    
  }
}
