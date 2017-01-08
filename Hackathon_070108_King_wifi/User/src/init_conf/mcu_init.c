#include "mcu_init.h"
#include "core_cm3.h"

void RCC_Configuration(void)
{
          /* 定义枚举类型变量 HSEStartUpStatus */
          ErrorStatus HSEStartUpStatus;
          
         /* 复位系统时钟设置*/
          RCC_DeInit();
          RCC_HSEConfig(RCC_HSE_ON);
         /* 等待HSE起振并稳定*/
          HSEStartUpStatus = RCC_WaitForHSEStartUp();
          /* 判断HSE起是否振成功，是则进入if()内部 */
          if(HSEStartUpStatus == SUCCESS)
          {
                /* Enable Prefetch Buffer */
                FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
                /* 选择HCLK（AHB）时钟源为SYSCLK 1分频 */
                FLASH_SetLatency(FLASH_Latency_2);
                RCC_HCLKConfig(RCC_SYSCLK_Div1);      /* 选择PCLK2时钟源为 HCLK（AHB） 1分频 */
                RCC_PCLK2Config(RCC_HCLK_Div1);       
                RCC_PCLK1Config(RCC_HCLK_Div2);       /* 选择PCLK1时钟源为 HCLK（AHB） 2分频 */
                RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  /* 选择锁相环（PLL）时钟源为HSE 1分频，倍频数为9，则PLL输出频率为 8MHz * 9 = 72MHz */
                RCC_PLLCmd(ENABLE);
            
                #if defined (STM32F10X_LD_VL) || (STM32F10X_LD_VL) || (STM32F10X_LD_VL)
                RCC_ADCCLKConfig(RCC_PCLK2_Div2);
                #else
                RCC_ADCCLKConfig(RCC_PCLK2_Div4);
                #endif

                /* Wait till PLL is ready */
                while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
                {
                }

                RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);     /* Select PLL as system clock source */

                /* Wait till PLL is used as system clock source */
                while(RCC_GetSYSCLKSource() != 0x08)
                {
                }
          }
            /* TIM2 clock enable */
          RCC_APB1PeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_USART2, ENABLE);

          RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
                                                                                                |RCC_APB2Periph_AFIO  | RCC_APB2Periph_USART1, ENABLE);
         
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
       NVIC_InitTypeDef NVIC_InitStructure;


        #ifdef  VECT_TAB_RAM  
        /* Set the Vector Table base location at 0x20000000 */ 
        NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);

        #else  /* VECT_TAB_FLASH  */
        /* Configure one bit for preemption priority */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

        /* Enable the TIM2 global Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);     /*这句话可不能少了*/
    
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);
	    //USART2
	    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);
				
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);

        /* Set the Vector Table base location at 0x08002000 -> USE AIP*/ 
        //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);  
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);  //only app, no boot included
        
        #endif
}

/*系统重启或者说复位*/
void system_softreset(void)
{
    __set_FAULTMASK(1);  /* 关闭所有中断 */
    NVIC_SystemReset();  /* 复位 */
    
}
