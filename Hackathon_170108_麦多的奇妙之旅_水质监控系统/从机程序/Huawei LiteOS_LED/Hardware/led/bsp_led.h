#ifndef __BSP_LED_H
#define __BSP_LED_H

#define ON    0
#define OFF   1

#define GPIO_Pin_LED_Blue           GPIO_Pin_15      //   //GPIO_Pin_9
#define GPIO_Pin_LED_Green          GPIO_Pin_14     //   //GPIO_Pin_11

#define GPIOx_LED_Blue              GPIOB
#define GPIOx_LED_Green             GPIOB       // GPIOC

#define RCC_APB2Periph_GPIOx_LED    RCC_APB2Periph_GPIOB    //    RCC_APB2Periph_GPIOC


#define LED_Blue(a)	if (a)	\
					GPIO_SetBits(GPIOx_LED_Blue,GPIO_Pin_LED_Blue);\
					else		\
					GPIO_ResetBits(GPIOx_LED_Blue,GPIO_Pin_LED_Blue)

#define LED_Green(a)	if (a)	\
					GPIO_SetBits(GPIOx_LED_Green,GPIO_Pin_LED_Green);\
					else		\
					GPIO_ResetBits(GPIOx_LED_Green,GPIO_Pin_LED_Green)
          
          
void LED_GPIO_Config(void);

#endif
