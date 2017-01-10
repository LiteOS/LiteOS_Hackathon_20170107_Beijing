#include "che.h"		    
//WS2812B RGB-LED IO初始化
void CHE_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //开启AFIO时钟
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //禁止JTAG功能
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB PC端口时钟
	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	 GPIO_ResetBits(GPIOB,GPIO_Pin_3); 			
	 GPIO_ResetBits(GPIOB,GPIO_Pin_12); 
	 GPIO_ResetBits(GPIOB,GPIO_Pin_13); 			
	 GPIO_ResetBits(GPIOB,GPIO_Pin_14); 
	 GPIO_ResetBits(GPIOB,GPIO_Pin_15); 
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB PC端口时钟
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_13|GPIO_Pin_11|GPIO_Pin_12;	 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz

	 GPIO_ResetBits(GPIOB,GPIO_Pin_10); 			
	 GPIO_ResetBits(GPIOB,GPIO_Pin_11); 
	 GPIO_ResetBits(GPIOB,GPIO_Pin_12); 
	 
}

void move_on()
{
	delay_ms(20);
	CHE_LF_M=1;
	CHE_LB_M=1;
	CHE_RF_M=1;
	CHE_RB_M=1;
	
	CHE_LF_B=0;
	CHE_LB_B=0;
	CHE_RF_B=0;
	CHE_RB_B=0;
	delay_ms(20);
}

void move_back()
{
	delay_ms(20);
	CHE_LF_M=0;
	CHE_LB_M=0;
	CHE_RF_M=0;
	CHE_RB_M=0;
	
	CHE_LF_B=1;
	CHE_LB_B=1;
	CHE_RF_B=1;
	CHE_RB_B=1;
	delay_ms(20);
}

void move_left()
{
	delay_ms(20);
	CHE_LF_M=0;
	CHE_LB_M=0;
	CHE_RF_M=1;
	CHE_RB_M=1;
	
	CHE_LF_B=1;
	CHE_LB_B=1;
	CHE_RF_B=0;
	CHE_RB_B=0;
	delay_ms(20);
}

void move_right()
{
	delay_ms(20);
	CHE_LF_M=1;
	CHE_LB_M=1;
	CHE_RF_M=0;
	CHE_RB_M=0;
	
	CHE_LF_B=0;
	CHE_LB_B=0;
	CHE_RF_B=1;
	CHE_RB_B=1;
	delay_ms(20);
}
void stop()
{
	delay_ms(20);
	CHE_LF_M=0;
	CHE_LB_M=0;
	CHE_RF_M=0;
	CHE_RB_M=0;
	
	CHE_LF_B=0;
	CHE_LB_B=0;
	CHE_RF_B=0;
	CHE_RB_B=0;
	delay_ms(20);
}










