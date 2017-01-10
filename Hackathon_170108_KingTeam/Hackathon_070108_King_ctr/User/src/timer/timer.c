//定时器
#include "timer.h"
#include <stdio.h>
#include <ctype.h>
#include "stm32f10x.h"
#include "xassert.h"
#include "adc.h"
#include "sbus.h"
#include "comm_struct.h"
#include "usart3.h"
#include "string.h"
#include "usart.h"

#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)

extern volatile uint8_t rcv_jsonpack_flag ; 
extern uint8_t rx_buf_len;
void Timer_Config(void)         //??????

{  
		TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);  //Timer2使能
		TIM_DeInit(TIM2);                              //复位TIM2定时器     
		TIM_TimeBaseStructure.TIM_Period=2000;          //定时器周期，这个表示定时=100*500us = 50ms；
		TIM_TimeBaseStructure.TIM_Prescaler=36000-1;     //预分频数,这个表示500us
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //TIM2时钟分配，1表示不分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//定时器计数为向上计数模式
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);     
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);           //清除定时器2的溢出标志位
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);      //使能定时器2溢出中断
		//TIM_Cmd(TIM2, ENABLE);                       //定时器2使能,现在time2用于判断一帧数据，那么刚开始是不使能的，等接收到数据之后在使能
}
void Timer3_Config(void)
{
		TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);  //Timer3使能
	    TIM_DeInit(TIM3);                              //复位TIM3定时器  
		TIM_TimeBaseStructure.TIM_Period=200;          //定时器周期，这个表示定时=100 *500us = 50ms；
		TIM_TimeBaseStructure.TIM_Prescaler=36000-1;     //预分频数,这个表示500us
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //TIM3时钟分配，1表示不分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//定时器计数为向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);     
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);           //清除定时器3的溢出标志位
	 	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);      //使能定时器3溢出中断
	    TIM_Cmd(TIM3, ENABLE);  
	
}
/*初始化定时器*/
void timer_init(void)
{
		
		Timer_Config();
}
/*初始化定时器3*/
void timer3_init(void)
{
		
		Timer3_Config();
}
/*定时器2中断操作*/
void TIM2_IRQHandler(void)
{
	
    CLI();
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) //????????????1
	{
			
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); //?????????
        rcv_jsonpack_flag = 1;
        //rx_buf_len = 0;   /*重新开始计数*/
        time2_disable();
		  
	}
    SEI();
}

///*定时器2中断操作*/
//void TIM2_IRQHandler(void)
//{
//	
//    uint8_t i;
//    //static uint8_t temp[10];
//    CLI();
//    
//	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) //????????????1
//	{
//			//定时1s
//		  TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); //?????????
//		  if(rx_buf_len > 0) //这个判断是多余的
//				rcv_jsonpack_flag = 1;
//			for(i = 0 ; i < rx_buf_len; i++){
//					usart2_printf("%c",USART2_RX_BUF[i]);
//			}		
//      usart2_printf("\r\n");			
//      time2_disable();	
//	}
//    SEI();
//}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3,  TIM_IT_Update);  //清除TIMx更新中断标志 
		//your code
			//printf("this is a test\r\n");
			
		}
}
//禁止定时器2
void time2_disable(void)
{
		TIM_Cmd(TIM2, DISABLE);                       //定时器2禁止
	
}
//使能定时器2
void timer2_enable(void)
{
		TIM_Cmd(TIM2, ENABLE);                       //定时器2使能
	
}
