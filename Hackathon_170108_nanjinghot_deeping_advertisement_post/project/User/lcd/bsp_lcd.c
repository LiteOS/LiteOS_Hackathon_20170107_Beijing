#include "bsp_lcd.h"



/**
  * @brief  LCD 初始化函数
  * @param  无
  * @retval 无
  */
void LCD_Init ( void )
{
	ILI9341_Init ();
	
	#if macLCD_XPT2046_ENABLE
	
	  XPT2046_Init ();
	
	#endif
	
}


