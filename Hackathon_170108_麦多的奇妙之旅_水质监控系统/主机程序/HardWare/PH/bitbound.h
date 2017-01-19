#ifndef _BITBOUND_H__
#define _BITBOUND_H__
#include <stm32f10x.h>  

#define GPIO_ODR_A  (GPIOA_BASE+0x0c)
#define GPIO_ODR_B  (GPIOB_BASE+0x0c)
#define GPIO_ODR_C  (GPIOC_BASE+0x0c)
#define GPIO_ODR_D  (GPIOD_BASF+0x0c)
#define GPIO_ODR_E  (GPIOE_BASE+0x0c)
#define GPIO_ODR_F  (GPIOF_BASF+0x0c)
#define GPIO_ODR_G  (GPIOG_BASE+0x0c)

#define GPIO_IDR_A  (GPIOA_BASE+0x08)
#define GPIO_IDR_B  (GPIOB_BASE+0x08)
#define GPIO_IDR_C  (GPIOC_BASE+0x08)
#define GPIO_IDR_D  (GPIOD_BASF+0x08)
#define GPIO_IDR_E  (GPIOE_BASE+0x08)
#define GPIO_IDR_F  (GPIOF_BASF+0x08)
#define GPIO_IDR_G  (GPIOG_BASE+0x08)

#define bitbound(addr,num) *(volatile unsigned long *)((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(num<<2)) 

#define paout(n)  bitbound(GPIO_ODR_A ,n)
#define pbout(n)  bitbound(GPIO_ODR_B ,n)
#define pcout(n)	bitbound(GPIO_ODR_C ,n)
#define pdout(n)  bitbound(GPIO_ODR_D ,n)
#define peout(n)  bitbound(GPIO_ODR_E ,n)
#define pfout(n)	bitbound(GPIO_ODR_F ,n)
#define pgout(n)  bitbound(GPIO_ODR_G ,n)

#define pain(n) 	bitbound(GPIO_IDR_A ,n) 
#define pbin(n) 	bitbound(GPIO_IDR_B ,n) 
#define pcin(n) 	bitbound(GPIO_IDR_C ,n) 
#define pdin(n) 	bitbound(GPIO_IDR_D ,n) 
#define pein(n) 	bitbound(GPIO_IDR_E ,n) 
#define pfin(n) 	bitbound(GPIO_IDR_F ,n) 
#define pgin(n) 	bitbound(GPIO_IDR_G ,n) 



#endif
