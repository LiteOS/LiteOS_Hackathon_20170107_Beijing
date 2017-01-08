#include "stm32f10x.h"
#include "adc.h"

vu16  AD_Value[N][M];   //ADC DMA缓存
vu16  adc_buffer[M];    //adc缓冲


/*GPIO ?????
?? ADC ??? 0  1  2  8  9  10  11  12  13  14  15,???????? PA0  PA1 
PA2  PB0  PB1  PC0  PC1  PC2  PC3  PC4  PC5
???? USART1 ?? TX ? PA9,RX ? PA10 */
/*************************
ADC12_IN0    PA0
ADC12_IN1    PA1
ADC12_IN2    PA2(USART2_TX)
ADC1_IN3    PA3(USART2_RX)
ADC12_IN4    PA4(SPI1_SS)
ADC12_IN5    PA5(SPI1_SCK)
ADC12_IN6    PA6(SPI1_MISO)
ADC12_IN7    PA7(SPI1_MOSI)
ADC12_IN8    PB0
ADC12_IN9    PB1
**************************/
#define ADC12_IN10   GPIO_Pin_0
#define ADC12_IN11   GPIO_Pin_1
#define ADC12_IN12   GPIO_Pin_2
#define ADC12_IN7    GPIO_Pin_7
/*ADC IO配置*/
void ADC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    //PC0、PC1、PC2 作为模拟通道输入引脚                         
    GPIO_InitStructure.GPIO_Pin = ADC12_IN10 | ADC12_IN11 | ADC12_IN12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //模拟输入引脚
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //PA7 作为模拟通道输入引脚                         
    GPIO_InitStructure.GPIO_Pin = ADC12_IN7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //模拟输入引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);                        
  
}
/*配置ADCRCC*/
void ADC_RCC_Configuration(void)
 {
     
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO, ENABLE );   //?? ADC1时钟
        /* Configure ADCCLK such as ADCCLK = PCLK2/6 */ 
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC ???????? 14M
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //?? DMA ??
   
 }

/* ADC1配置*/
void ADC1_Configuration(void)
{
      ADC_InitTypeDef  ADC_InitStructure;
      ADC_DeInit(ADC1);  //??? ADC1 ????????????
  
      /* ADC1 configuration ------------------------------------------------------*/
      ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC ????:ADC1 ? ADC2???????
      ADC_InitStructure.ADC_ScanConvMode =ENABLE;  //???????????
      ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //?????????????
      ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //????????
      ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC ?????
      ADC_InitStructure.ADC_NbrOfChannel = M; //????????? ADC ?????
      ADC_Init(ADC1, &ADC_InitStructure); //?? ADC_InitStruct ???????????ADCx ????
      /* ADC1 regular channel11 configuration */ 
      //???? ADC ??????,??????????????
      //ADC1,ADC ?? x,???????? y,????? 239.5 ??
      ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5 );
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5 );
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_7,  4, ADC_SampleTime_239Cycles5 );
      //ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
      //ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 ); 
      //ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_239Cycles5 );
      //ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_239Cycles5 );
      //ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_239Cycles5 );
      //ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, ADC_SampleTime_239Cycles5 );
      //ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 8, ADC_SampleTime_239Cycles5 );
     // ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 9, ADC_SampleTime_239Cycles5 );
     // ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 10, ADC_SampleTime_239Cycles5 );
     // ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC_SampleTime_239Cycles5 );
     // ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 12, ADC_SampleTime_239Cycles5 );
      
      // ?? ADC ? DMA ??(??? DMA ??,?????? DMA ?????)
      ADC_DMACmd(ADC1, ENABLE);  
  
      /* Enable ADC1 */
      ADC_Cmd(ADC1, ENABLE);    //????? ADC1
      /* Enable ADC1 reset calibaration register */   
      ADC_ResetCalibration(ADC1);   //ADC精度
      /* Check the end of ADC1 reset calibration register */
      while(ADC_GetResetCalibrationStatus(ADC1));  //??ADC1??????????,???????
  
       /* Start ADC1 calibaration */
      ADC_StartCalibration(ADC1);   //???? ADC1 ?????
      /* Check the end of ADC1 calibration */
      while(ADC_GetCalibrationStatus(ADC1));    //????ADC1?????,???????
}
/*配置DMA*/
void DMA_Configuration(void)
{
      /* ADC1  DMA1 Channel Config */
      DMA_InitTypeDef DMA_InitStructure;
      DMA_DeInit(DMA1_Channel1);   //? DMA ??? 1 ?????????
      DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;  //DMA??ADC???
      DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;  //DMA ?????
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //????????????
      DMA_InitStructure.DMA_BufferSize = N*M;  //DMA ??? DMA ?????
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //?????????
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //?????????
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //????? 16 ?
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //????? 16 ?
      DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //?????????
      DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA ?? x ?????? 
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA ?? x ????????????
      DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //?? DMA_InitStruct ????????? DMA ???
}
/*ADC初始化*/
void Init_All_Periph(void)
{
  
      ADC_RCC_Configuration();  
      ADC_GPIO_Configuration();
      ADC1_Configuration();
      DMA_Configuration();
   
}

/*adc初始化*/
void adc_init(void)
{
    Init_All_Periph();
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    DMA_Cmd(DMA1_Channel1,ENABLE);//使能DMA通道1
}
/*AD值转化为电压值*/
u16 GetVolt(u16 advalue)   
{
   
      return (u16)(advalue * 330 / 4096);   //??????? 100 ?,????????
}
 
/*滤波*/
void filter(void)
{
	int  sum = 0;
	u8  count;
	u8  i;    
	for(i=0;i<M;i++){
         for ( count=0;count<N;count++)
          {
           sum += AD_Value[count][i];
          }
          adc_buffer[i]=sum/N;
          sum=0;
    }
 }
/*获取adc值*/
uint16_t adc_val(uint8_t ch)
{
	filter();
	return adc_buffer[ch]; 
}
