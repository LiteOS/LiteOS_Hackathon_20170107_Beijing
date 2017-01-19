#ifndef __Bsp_Key_H
#define __Bsp_Key_H

#include "common.h"

/* 选择通讯方式的按键 */
#define GPIO_Pin_TouchWay                 GPIO_Pin_1    
#define GPIOx_TouchWay                    GPIOC
#define RCC_APB2Periph_GPIOx_TouchWay     RCC_APB2Periph_GPIOC  

/* 键盘的按键 */
#define GPIO_Pin_KeyLF                    GPIO_Pin_0  
#define GPIOx_KeyLF                       GPIOC
#define RCC_APB2Periph_KeyLF              RCC_APB2Periph_GPIOC

#define GPIO_Pin_KeyUp                    GPIO_Pin_1    
#define GPIOx_KeyUp                       GPIOA
#define RCC_APB2Periph_KeyUp              RCC_APB2Periph_GPIOA

#define GPIO_Pin_KeyRG                    GPIO_Pin_1    
#define GPIOx_KeyRG                       GPIOC
#define RCC_APB2Periph_KeyRG              RCC_APB2Periph_GPIOC

#define GPIO_Pin_KeyOK                    GPIO_Pin_2   
#define GPIOx_KeyOK                       GPIOC
#define RCC_APB2Periph_KeyOK              RCC_APB2Periph_GPIOC
#define EXTI_Line_KeyLF                   EXTI_Line0
#define EXTIx_IRQn_KeyLF                  EXTI0_IRQn

#define GPIO_Pin_KeyDn                    GPIO_Pin_3    
#define GPIOx_KeyDn                       GPIOC
#define RCC_APB2Periph_KeyDn              RCC_APB2Periph_GPIOC
#define EXTI_Line_KeyLF                   EXTI_Line0
#define EXTIx_IRQn_KeyLF                  EXTI0_IRQn

#define KeyDefault    0
#define KeyLF         1
#define KeyUp         2
#define KeyRG         3
#define KeyOK         4
#define KeyDn         5

#define KEY_LF  GPIO_ReadInputDataBit(GPIOx_KeyLF,GPIO_Pin_KeyLF)
#define KEY_UP  GPIO_ReadInputDataBit(GPIOx_KeyUp,GPIO_Pin_KeyUp)
#define KEY_RG  GPIO_ReadInputDataBit(GPIOx_KeyRG,GPIO_Pin_KeyRG)
#define KEY_OK  GPIO_ReadInputDataBit(GPIOx_KeyOK,GPIO_Pin_KeyOK) 
#define KEY_DN  GPIO_ReadInputDataBit(GPIOx_KeyDn,GPIO_Pin_KeyDn)

extern uint8_t KeyStatue;
void Key_Scan (void);
void Key_Scan1 (void);
void Key_Init (void);
void ReadTouchWay (void);
UINT32 creat_KeyScanTask (void);

#endif
