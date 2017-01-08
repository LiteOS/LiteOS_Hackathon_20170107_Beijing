/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOB和GPIOF的外设时钟*/
		RCC_APB2PeriphClockCmd( macLED1_GPIO_CLK|macLED2_GPIO_CLK|macLED3_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(macLED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIOF引脚*/															   
		GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;

		/*调用库函数，初始化GPIOF7*/
		GPIO_Init(macLED2_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIOF引脚*/															   
		GPIO_InitStructure.GPIO_Pin = macLED3_GPIO_PIN;

		/*调用库函数，初始化GPIOF7*/
		GPIO_Init(macLED3_GPIO_PORT, &GPIO_InitStructure);			  

		/* 关闭所有led灯	*/
		GPIO_SetBits(macLED1_GPIO_PORT, macLED1_GPIO_PIN);
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(macLED2_GPIO_PORT, macLED2_GPIO_PIN);	 
    
    /* 关闭所有led灯	*/
		GPIO_SetBits(macLED3_GPIO_PORT, macLED3_GPIO_PIN);	 
}
/*********************************************END OF FILE**********************/
