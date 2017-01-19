#include "pm2d5.h"

static void delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}


float Get_PM2d5(void)
{
		uint16_t tmp = 0;
		float pm = 0;
		ADC_SoftwareStartConvCmd(macADCx, ENABLE);// 由于没有采用外部触发，所以使用软件触发ADC转换 
		Signal_Low;
		delay(500);
		tmp = ADC_GetConversionValue(macADCx);	
		delay(40);
		Signal_High;
		//Delay(30);
		pm = 0.17*tmp-0.1;
		printf("\r\n The current AD value = %f \r\n", pm); 
		printf("\r\n\r\n");
		return pm;
}


