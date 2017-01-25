#ifndef __HALL_H
#define	__HALL_H


#include "stm32f10x.h"

//  引脚定义
#define    HALL1_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    HALL1_GPIO_PORT    GPIOE			   
#define    HALL1_GPIO_PIN		  GPIO_Pin_5

//#define    HALL2_GPIO_CLK     RCC_APB2Periph_GPIOC
//#define    HALL2_GPIO_PORT    GPIOE		   
//#define    HALL2_GPIO_PIN		  GPIO_Pin_2


 /** 按键按下标置宏
	*  按键按下为高电平，设置 HALL_ON=1， HALL_OFF=0
	*  若按键按下为低电平，把宏设置成HALL_ON=0 ，HALL_OFF=1 即可
	*/
#define HALL_ON	1
#define HALL_OFF	0

void HALL_GPIO_Config(void);
uint8_t HALL_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);


#endif /* __HALL_H */

