#ifndef __Bsp_Beep_H
#define __Bsp_Beep_H

#define GPIO_Pin_Beep           GPIO_Pin_12   
#define GPIOx_Beep              GPIOB
#define RCC_APB2Periph_GPIOx_Beep    RCC_APB2Periph_GPIOB    

void Beep_Init(void);
void Beep_ON_Nop (int Time);
          
#endif
