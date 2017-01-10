#ifndef __BSP_LCD_H
#define	__BSP_LCD_H


#include "stm32f10x.h"

#include "bsp_ili9341_lcd.h"
#include "bsp_xpt2046_lcd.h"



/************************************ LCD 函数声明 ***************************************/
#define             macLCD_XPT2046_ENABLE                           0      //是否使用触摸屏



/************************************ LCD 函数声明 ***************************************/
void                     LCD_Init                        ( void );



#endif


