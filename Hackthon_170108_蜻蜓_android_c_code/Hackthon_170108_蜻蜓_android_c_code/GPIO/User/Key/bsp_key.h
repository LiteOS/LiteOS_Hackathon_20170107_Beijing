#ifndef __KEY_H
#define	__KEY_H


#include "stm32f10x.h"

// 使用KEY1，如果要使用KEY2的话，把宏KEY1_PA0注释掉即可
#define   KEYI_PA0

#ifdef    KEYI_PA0
#define               macKEY1_GPIO_CLK                      RCC_APB2Periph_GPIOA
#define               macKEY1_GPIO_PORT    	                GPIOA			   
#define               macKEY1_GPIO_PIN		                  GPIO_Pin_0

#else    //KEY2_PC13
#define               macKEY1_GPIO_CLK                      RCC_APB2Periph_GPIOC
#define               macKEY1_GPIO_PORT    	                GPIOC		   
#define               macKEY1_GPIO_PIN		                  GPIO_Pin_13

#endif

 /*******
 *按键按下标置，按键K1和K2默认是低电平，按键按下的时候是高电平
 KEY_ON 0
 KEY_OFF 1
 ********/
#define KEY_ON	1
#define KEY_OFF	0


void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);


#endif /* __LED_H */

