
#include "stm32f4xx.h"
#include "typedef.h"
#include "usart.h"

uint16_t GPIO_LIST[20];
GPIO_TypeDef *(GPION_LIST[20]);

GPIO_InitTypeDef GPIO_InitStructure;

void IO_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	
	GPIO_LIST[0] = GPIO_Pin_3;
	GPIO_LIST[1] = GPIO_Pin_2;
	GPIO_LIST[2] = GPIO_Pin_10;
	GPIO_LIST[3] = GPIO_Pin_3;
	GPIO_LIST[4] = GPIO_Pin_5;
	GPIO_LIST[5] = GPIO_Pin_4;
	GPIO_LIST[6] = GPIO_Pin_10;
	GPIO_LIST[7] = GPIO_Pin_8;
	
	GPIO_LIST[8] = GPIO_Pin_9;
	GPIO_LIST[9] = GPIO_Pin_7;
	GPIO_LIST[10] = GPIO_Pin_6;
	GPIO_LIST[11] = GPIO_Pin_7;
	GPIO_LIST[12] = GPIO_Pin_6;
	GPIO_LIST[13] = GPIO_Pin_5;
	
	GPIO_LIST[14] = GPIO_Pin_0;
	GPIO_LIST[15] = GPIO_Pin_1;
	GPIO_LIST[16] = GPIO_Pin_4;
	GPIO_LIST[17] = GPIO_Pin_0;
	GPIO_LIST[18] = GPIO_Pin_1;
	GPIO_LIST[19] = GPIO_Pin_0;
	
	GPION_LIST[0] = GPIOA;
	GPION_LIST[1] = GPIOA;
	GPION_LIST[2] = GPIOA;
	GPION_LIST[3] = GPIOB;
	GPION_LIST[4] = GPIOB;
	GPION_LIST[5] = GPIOB;
	GPION_LIST[6] = GPIOB;
	GPION_LIST[7] = GPIOA;
	
	GPION_LIST[8] = GPIOA;
	GPION_LIST[9] = GPIOC;
	GPION_LIST[10] = GPIOB;
	GPION_LIST[11] = GPIOA;
	GPION_LIST[12] = GPIOA;
	GPION_LIST[13] = GPIOA;
	
	GPION_LIST[14] = GPIOA;
	GPION_LIST[15] = GPIOA;
	GPION_LIST[16] = GPIOA;
	GPION_LIST[17] = GPIOB;
	GPION_LIST[18] = GPIOC;
	GPION_LIST[19] = GPIOC;
	
	for( uint8 i = 0; i < 20; ++i ) {
		GPIO_InitStructure.GPIO_Pin = GPIO_LIST[i];
		GPIO_Init(GPION_LIST[i],&GPIO_InitStructure);
	}
}

void IO_Close()
{
	
}
void IO_DirWrite( uint8 i, uint8 d )
{
	if( d == 0 ) {
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	}
	else {
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	}
	GPIO_InitStructure.GPIO_Pin = GPIO_LIST[i];
	GPIO_Init(GPION_LIST[i],&GPIO_InitStructure);
}
void IO_PullWrite( uint8 i, uint8 d )
{
	if( d == 0 ) {
		
	}
	else {
		
	}
}

void IO_OutWrite( uint8 i, uint8 d )
{
	if( d == 0 ) {
		GPIO_ResetBits(GPION_LIST[i],GPIO_LIST[i]);
	}
	else {
		GPIO_SetBits(GPION_LIST[i],GPIO_LIST[i]);
	}
}
uint8 IO_OutRead( uint8 i )
{
	return GPIO_ReadOutputDataBit(GPION_LIST[i],GPIO_LIST[i]);
}
uint8 IO_InRead( uint8 i )
{
	return GPIO_ReadInputDataBit(GPION_LIST[i],GPIO_LIST[i]);
}
void IO_AnalogOpen( uint8 i, uint8 d )
{
	//m_aio[i] = mraa_aio_init( i );
}
int32 IO_AnalogRead( uint8 i )
{
	int32 d = 0;//mraa_aio_read( m_aio[i] );
	return d;
}




