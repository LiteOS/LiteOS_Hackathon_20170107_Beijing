#include "bsp_ov7725.h"
#include "bsp_sccb.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_ov7725.h"

void IEyeImagDisp(void)
{
	uint16_t i, j;
	uint16_t Camera_Data;
	
	/* 设置液晶扫描方向为 左下角->右上角 */
	ILI9341_GramScan( 2 );
	
	for(i = 0; i < 240; i++)
	{
		for(j = 0; j < 320; j++)
		{
			do
			{
	                                  RGB565=0;				
			}while(0);
			ILI9341_Write_Data(Camera_Data);
		}
	}
	
}