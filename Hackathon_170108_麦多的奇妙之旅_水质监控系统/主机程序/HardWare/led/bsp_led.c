#include "common.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：LED引脚初始化
 * 输入  ：无
 * 返回  ：无
 */
void LED_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_LED, ENABLE); 
												   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED_Blue;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOx_LED_Blue, &GPIO_InitStructure);	
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED_Green;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOx_LED_Green, &GPIO_InitStructure);	

    LED_Blue(OFF);
    LED_Green(OFF);
}

///*
// * 函数名：LED_Blue_Flash
// * 描述  ：LED蓝灯闪烁
// * 输入  ：time：闪烁次数
// * 返回  ：无
// */
//void LED_Blue_Flash (uint8_t time)
//{
//  int i = 0;
//  for (i = 0; i < time; i++)
//  {
//    LED_Blue(ON);
//    UserDelay_KeyScan(50);
//    LED_Blue(OFF);
//    UserDelay_KeyScan(50);
//  }
//}
