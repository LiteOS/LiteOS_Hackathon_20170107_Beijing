#include "common.h"

void Key_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
}

void ReadTouchWay (void)    //按键确定通讯方式
{
  uint8_t statue = 0;
  Key_Init();
  
  statue = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
  if (statue)         
    Connect_Way = USE_ZGB;      //默认是ZGB
  else
    Connect_Way = USE_CAN;      //按下是CAN
}
