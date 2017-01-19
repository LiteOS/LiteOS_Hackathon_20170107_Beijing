#ifndef __Bsp_Water_Level_H
#define __Bsp_Water_Level_H

#define GPIO_Pin_WatLevl           GPIO_Pin_0 
#define GPIOx_WatLevl             GPIOA
#define RCC_APB2Periph_GPIOx_WatLevl    RCC_APB2Periph_GPIOA   
#define GPIO_PortSourceGPIOx_WatLevl    GPIO_PortSourceGPIOA
#define GPIO_PinSource_WatLevl          GPIO_PinSource0
#define EXTI_Line_WatLevl               EXTI_Line0
#define EXTIx_IRQn_WatLevl              EXTI0_IRQn

void Water_Level_Init (void);
void CheckWaterLevel (void);

#endif
