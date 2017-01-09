/********************************************/
/*  文件：io_ctr.c
	功能：实现对设备的控制，输出高低电平
	版本：1.0
*********************************************/
#include "io_ctr.h"
#include "stm32f10x.h"

/* ------------------------------*/
/* IO口初始化                     */
/* B2 B3 B4*/
void io_ctr_gpio_configuration()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void io_ctr_init(void)
{
    io_ctr_gpio_configuration();
    GPIO_ResetBits(GPIOC,  GPIO_Pin_6);
    GPIO_ResetBits(GPIOC,  GPIO_Pin_7);
}
/*开启监控设备*/
void open_dev(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
}
/*关闭监控设备*/
void close_dev(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}
