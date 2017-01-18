#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H
#include "Common.h"

#define GPIO_Pin_DS18B20IN           GPIO_Pin_9
#define GPIO_Pin_DS18B20EX           GPIO_Pin_8

#define GPIOx_DS18B20IN              GPIOC
#define GPIOx_DS18B20EX              GPIOC

#define RCC_APB2Periph_GPIOx_DS18B20IN    RCC_APB2Periph_GPIOC
#define RCC_APB2Periph_GPIOx_DS18B20EX    RCC_APB2Periph_GPIOC
#define DS18B20IN     0       //板载DS18B20
#define DS18B20EX     1       //外部DS18B20

#define HIGH  1
#define LOW   0
                 
uint8_t DS18B20_Init (int InOrEx);    //初始化
float DS18B20_Get_Temp(int InOrEx);   //获取温度
  
#endif
