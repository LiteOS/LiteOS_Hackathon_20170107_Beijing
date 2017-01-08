#include "led.h"
#include "los_typedef.h"

//stm32 files
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

static bool bLedOpen = false;
//LED port and pin
#define LED_PORT GPIOB
#define LED_PIN GPIO_Pin_7
#define LED_GPIO_CLK RCC_APB2Periph_GPIOB

void LED_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    //off the realy
    GPIO_ResetBits(LED_PORT,LED_PIN);
    //set bool variable
    bLedOpen = false;
}

void LED_ON(void){
    //check
    if(true == bLedOpen)
        return;
    //off the realy
    GPIO_SetBits(LED_PORT,LED_PIN);
    //set bool variable
    bLedOpen = true;
}

void LED_off(void){
    //check
    if(false == bLedOpen)
        return;
    //off the realy
    GPIO_ResetBits(LED_PORT,LED_PIN);
    //set bool variable
    bLedOpen = false;
}

