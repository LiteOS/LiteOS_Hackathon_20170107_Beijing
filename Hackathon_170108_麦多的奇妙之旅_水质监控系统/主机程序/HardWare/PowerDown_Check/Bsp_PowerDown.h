#ifndef __Bsp_PowerDown_H
#define __Bsp_PowerDown_H

#define GPIO_Pin_PowDon           GPIO_Pin_5 
#define GPIOx_Powdon              GPIOC
#define RCC_APB2Periph_GPIOx_PowDon    RCC_APB2Periph_GPIOC   
#define GPIO_PortSourceGPIOx_Powdon    GPIO_PortSourceGPIOC
#define GPIO_PinSource_PowDon          GPIO_PinSource5
#define EXTI_Line_PowDon               EXTI_Line5
#define EXTIx_IRQn_PowDon              EXTI9_5_IRQn

void PowDon_Init (void);
void CheckPowerDown (void);
  
#endif
