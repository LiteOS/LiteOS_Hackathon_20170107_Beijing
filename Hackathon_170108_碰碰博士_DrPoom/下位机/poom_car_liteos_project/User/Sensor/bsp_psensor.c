#include "bsp_psensor.h"

static uint16_t Sensor_Pins[4] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

void PSensor_Init (void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	for (i = 0; i < 4; i++)
	{
		GPIO_InitStruct.GPIO_Pin = Sensor_Pins[i];
    	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    	GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

uint8_t Get_Sensor_Stat (uint8_t id)
{
	return !(GPIO_ReadInputData(GPIOB) & Sensor_Pins[id]);
}
