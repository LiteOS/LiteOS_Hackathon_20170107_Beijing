#include "Common.h"
#
 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
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

