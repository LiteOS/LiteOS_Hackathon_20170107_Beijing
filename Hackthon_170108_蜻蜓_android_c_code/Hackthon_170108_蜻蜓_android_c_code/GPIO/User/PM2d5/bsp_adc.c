#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;


/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 端口时钟
	macADC_GPIO_APBxClock_FUN ( macADC_GPIO_CLK, ENABLE );
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph,ENABLE)
	// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin = macADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(macADC_PORT, &GPIO_InitStructure);		

	RCC_APB2PeriphClockCmd(macSIGNAL_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = macSIGNAL_POUT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(macSINGAL_PORT, &GPIO_InitStructure);
}
 

/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	

	// 打开ADC时钟
	macADC_APBxClock_FUN ( macADC_CLK, ENABLE );
	
	// ADC 模式配置
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道1个
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// 初始化ADC
	ADC_Init(macADCx, &ADC_InitStructure);
	
	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置 ADC 通道转换顺序为1，第一个转换，采样时间为55.5个时钟周期
	ADC_RegularChannelConfig(macADCx, macADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
	
	// ADC 转换结束产生中断，在中断服务程序中读取转换值
	ADC_ITConfig(macADCx, ADC_IT_EOC, DISABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(macADCx, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(macADCx);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(macADCx));
	
	// ADC开始校准
	ADC_StartCalibration(macADCx);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(macADCx));
	
	
}



/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
	//ADC_NVIC_Config();
}
/*********************************************END OF FILE**********************/
