#include "relay.h"
#include "los_typedef.h"

//stm32 fwlib
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

//relay on-off status
static bool s_bRelayOn = FALSE;
//relay port and pin
#define RELAY_PORT GPIOB
#define RELAY_PIN GPIO_Pin_5
#define RELAY_GPIO_CLK RCC_APB2Periph_GPIOB

void relay_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RELAY_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = RELAY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RELAY_PORT, &GPIO_InitStructure);
    //off the realy
    GPIO_ResetBits(RELAY_PORT,RELAY_PIN);
    //set bool
    s_bRelayOn = FALSE;
}

void relay_on(void){
    //check
    if(TRUE == s_bRelayOn)
        return;
    //on
    GPIO_SetBits(RELAY_PORT,RELAY_PIN);
    //set bool
    s_bRelayOn = TRUE;
    return;
}

void relay_off(void){
    //check
    if(FALSE == s_bRelayOn)
        return;
    //off
    GPIO_ResetBits(RELAY_PORT,RELAY_PIN);
    //set bool
    s_bRelayOn = FALSE;
    return;
}

unsigned char relay_status(void){
    if(TRUE == s_bRelayOn)
        return 1;
    else
        return 0;
}



