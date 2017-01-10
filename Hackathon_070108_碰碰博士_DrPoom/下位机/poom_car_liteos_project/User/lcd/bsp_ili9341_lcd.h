#ifndef      __BSP_ILI9341_LCD_H
#define	     __BSP_ILI9341_LCD_H


#include "stm32f10x.h"



/***************************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM1 连接 TFT，地址范围为0X6000 0000 ~ 0X63FF FFFF
FSMC_A16 接LCD的DC(寄存器/数据选择)脚
寄存器基地址 = 0X60000000
RAM基地址 = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
当选择不同的地址线时，地址要重新计算  
****************************************************************************************/

/******************************* ILI9341 显示屏的 FSMC 参数定义 ***************************/
#define      macFSMC_Addr_ILI9341_CMD         ( ( uint32_t ) 0x60000000 )	     //FSMC_Bank1_NORSRAM1用于LCD命令操作的地址
#define      macFSMC_Addr_ILI9341_DATA        ( ( uint32_t ) 0x60020000 )      //FSMC_Bank1_NORSRAM1用于LCD数据操作的地址      

#define      macFSMC_Bank1_NORSRAMx           FSMC_Bank1_NORSRAM1



/******************************* ILI9341 显示屏8080通讯引脚定义 ***************************/
#define      macILI9341_CS_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      macILI9341_CS_CLK                RCC_APB2Periph_GPIOD    
#define      macILI9341_CS_PORT               GPIOD   
#define      macILI9341_CS_PIN                GPIO_Pin_7

#define      macILI9341_DC_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      macILI9341_DC_CLK                RCC_APB2Periph_GPIOD    
#define      macILI9341_DC_PORT               GPIOD   
#define      macILI9341_DC_PIN                GPIO_Pin_11

#define      macILI9341_RST_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      macILI9341_RST_CLK               RCC_APB2Periph_GPIOE   
#define      macILI9341_RST_PORT              GPIOE
#define      macILI9341_RST_PIN               GPIO_Pin_1

#define      macILI9341_BK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      macILI9341_BK_CLK                RCC_APB2Periph_GPIOD    
#define      macILI9341_BK_PORT               GPIOD
#define      macILI9341_BK_PIN                GPIO_Pin_12



/*************************************** 调试预用 ******************************************/
#define      macDEBUG_DELAY()                



/***************** ILI934 显示屏全屏默认（扫描方向为1时）最大宽度和最大高度 *********************/
#define      macILI9341_Default_Max_Width		  240     //起始点的X坐标
#define      macILI9341_Default_Max_Heigth		320     //起始点的Y坐标




/***************************** ILI934 显示区域的起始坐标和总行列数 ***************************/
#define      macILI9341_DispWindow_X_Star		    0     //起始点的X坐标
#define      macILI9341_DispWindow_Y_Star		    0     //起始点的Y坐标

#define      macILI9341_DispWindow_COLUMN		  240     //总列数
#define      macILI9341_DispWindow_PAGE		    320     //总行数



/***************************** 在 ILI934 显示屏上显示的字符的大小 ***************************/
#define      macWIDTH_EN_CHAR		                 8	      //英文字符宽度 
#define      macHEIGHT_EN_CHAR		              16		    //英文字符高度 


#define      macWIDTH_CH_CHAR		                16	    //中文字符宽度 
#define      macHEIGHT_CH_CHAR		              16		  //中文字符高度 

#define      macGetGBKCode( ucBuffer, usChar )  	  //定义获取中文字符字模数组的函数名，ucBuffer为存放字模数组名，usChar为中文字符（国标码）



/******************************* 定义 ILI934 显示屏常用颜色 ********************************/
#define      macBACKGROUND		                macBLACK   //默认背景颜色

#define      macWHITE		 		                  0xFFFF	   //白色
#define      macBLACK                         0x0000	   //黑色 
#define      macGREY                          0xF7DE	   //灰色 
#define      macBLUE                          0x001F	   //蓝色 
#define      macBLUE2                         0x051F	   //浅蓝色 
#define      macRED                           0xF800	   //红色 
#define      macMAGENTA                       0xF81F	   //红紫色，洋红色 
#define      macGREEN                         0x07E0	   //绿色 
#define      macCYAN                          0x7FFF	   //蓝绿色，青色 
#define      macYELLOW                        0xFFE0	   //黄色 
#define      macBRED                          0xF81F
#define      macGRED                          0xFFE0
#define      macGBLUE                         0x07FF



/******************************* 定义 ILI934 常用命令 ********************************/
#define      macCMD_SetCoordinateX		 		    0x2A	     //设置X坐标
#define      macCMD_SetCoordinateY		 		    0x2B	     //设置Y坐标
#define      macCMD_SetPixel		 		          0x2C	     //填充像素



/********************************** 声明 ILI934 函数 ***************************************/
void                     ILI9341_Init                    ( void );
void                     ILI9341_Rst                     ( void );
void                     ILI9341_BackLed_Control         ( FunctionalState enumState );
__inline void            ILI9341_Write_Cmd               ( uint16_t usCmd );
__inline void            ILI9341_Write_Data              ( uint16_t usData );
__inline uint16_t        ILI9341_Read_Data               ( void );
void                     ILI9341_GramScan                ( uint8_t ucOtion );
void                     ILI9341_OpenWindow              ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     ILI9341_Clear                   ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight, uint16_t usColor );
void                     ILI9341_SetPointPixel           ( uint16_t usX, uint16_t usY, uint16_t usColor );
uint16_t                 ILI9341_GetPointPixel           ( uint16_t usX , uint16_t usY );
void                     ILI9341_DrawLine                ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2, uint16_t usColor );
void                     ILI9341_DrawRectangle           ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint16_t usColor, uint8_t ucFilled );
void                     ILI9341_DrawCircle              ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint16_t usColor, uint8_t ucFilled );
void                     ILI9341_DispChar_EN             ( uint16_t usX, uint16_t usY, const char cChar, uint16_t usColor_Background, uint16_t usColor_Foreground );
void                     ILI9341_DispString_EN           ( uint16_t usX, uint16_t usY, const char * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground );
void                     ILI9341_DispChar_CH             ( uint16_t usX, uint16_t usY, uint16_t usChar, uint16_t usColor_Background, uint16_t usColor_Foreground );
void                     ILI9341_DispString_CH           ( uint16_t usX, uint16_t usY, const uint8_t * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground );
void                     ILI9341_DispString_EN_CH        ( uint16_t usX, uint16_t usY, const uint8_t * pStr, uint16_t usColor_Background, uint16_t usColor_Foreground );



#endif /* __BSP_ILI9341_ILI9341_H */

