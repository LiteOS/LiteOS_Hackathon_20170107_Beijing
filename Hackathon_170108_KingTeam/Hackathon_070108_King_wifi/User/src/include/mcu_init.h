#ifndef _MCU_INT
#define _MCU_INT

#include "stm32f10x.h"
#define WIZ_SCS			      GPIO_Pin_12	// out
#define WIZ_SCLK			    GPIO_Pin_13	// out
#define WIZ_MISO			    GPIO_Pin_14	// in
#define WIZ_MOSI			    GPIO_Pin_15	// out

#define WIZ_RESET		        GPIO_Pin_9	// out
#define WIZ_INT			        GPIO_Pin_6	// in
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void Timer_Configuration(void);
void DMA_Configuration(unsigned long ADCConvertedValue);
void ADC_Configuration(void);
/*系统重启或者说复位*/
void system_softreset(void);
/* Private functions ---------------------------------------------------------*/
#endif

