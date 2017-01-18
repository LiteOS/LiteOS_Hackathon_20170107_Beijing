#include "common.h"

void Beep (uint8_t Statue)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  if (Statue == ON)
  {
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Beep;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOx_Beep, &GPIO_InitStructure);	
    GPIO_ResetBits(GPIOx_Beep,GPIO_Pin_Beep);
  }
  else
  {
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Beep;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOx_Beep, &GPIO_InitStructure);	
    GPIO_SetBits(GPIOx_Beep,GPIO_Pin_Beep);
  }
}

/*
 * 函数名：Beep_ON_Nop
 * 描述  ：蜂鸣器提示
 * 输入  ：Time：警报时长
 * 返回  ：无
 */
void Beep_ON_Nop (int Time)
{
  Beep(ON);
  UserDelay_KeyScan(Time);
  Beep(OFF);
  UserDelay_KeyScan(Time);
}

/*
 * 函数名：Beep_Init
 * 描述  ：蜂鸣器引脚初始化
 * 输入  ：无
 * 返回  ：无
 */
void Beep_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_Beep, ENABLE); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Beep;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOx_Beep, &GPIO_InitStructure);	
  GPIO_SetBits(GPIOx_Beep,GPIO_Pin_Beep);
  
  Beep(ON);
  Delay_ms(50);
  Beep(OFF);
}
