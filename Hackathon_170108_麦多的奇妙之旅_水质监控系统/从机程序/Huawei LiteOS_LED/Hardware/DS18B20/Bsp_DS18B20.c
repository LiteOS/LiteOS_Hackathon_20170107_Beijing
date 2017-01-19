#include "Common.h"

  /**
  * @brief  设置DS18B20引脚输出电平
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 无
  */
static void DS18B20_DATA_OUT (int InOrEx, int Statue)
{
  if (InOrEx == DS18B20IN)	 
  {       
    if (Statue)  
    {     
      GPIO_SetBits(GPIOx_DS18B20IN,GPIO_Pin_DS18B20IN);     
    }     
    else	
    {     
      GPIO_ResetBits(GPIOx_DS18B20IN,GPIO_Pin_DS18B20IN);   
    }     
  }   
  else
  {
    if (Statue)  
    {     
      GPIO_SetBits(GPIOx_DS18B20EX,GPIO_Pin_DS18B20EX);     
    }     
    else	
    {     
      GPIO_ResetBits(GPIOx_DS18B20EX,GPIO_Pin_DS18B20EX);   
    } 
  }  
}

  /**
  * @brief  获取DS18B20引脚电平
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 无
  */
static uint8_t DS18B20_DATA_IN (int InOrEx)
{
  if (InOrEx == DS18B20IN)    
  {                           
    return GPIO_ReadInputDataBit(GPIOx_DS18B20IN,GPIO_Pin_DS18B20IN);  
  } 
  else
  {
    return GPIO_ReadInputDataBit(GPIOx_DS18B20EX,GPIO_Pin_DS18B20EX);  
  }
}


  /**
  * @brief  将DS18B20引脚变为输入模式
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 无
  */
  static void DS18B20_Mode_IPU(int InOrEx)
  {
    if (InOrEx == DS18B20IN)
    {
      GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20IN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
      GPIO_Init(GPIOx_DS18B20IN, &GPIO_InitStructure);
    }
    else
    {
      GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20EX;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
      GPIO_Init(GPIOx_DS18B20EX, &GPIO_InitStructure);
    }
  }

 /**
  * @brief  将DS18B20引脚变为输出模式
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 无
  */
static void DS18B20_Mode_Out_PP(int InOrEx)
{
 	GPIO_InitTypeDef GPIO_InitStructure;		
  
  if (InOrEx == DS18B20IN)
  {
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20IN;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_DS18B20IN, &GPIO_InitStructure);
  }
  else
  {
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20EX;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_DS18B20EX, &GPIO_InitStructure);
  }
}


 /**
  * @brief  主机向DS18B20发送复位脉冲
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 无
  */
void DS18B20_Rst(int InOrEx)	   
{                 
  DS18B20_Mode_Out_PP(InOrEx);
  DS18B20_DATA_OUT(InOrEx, LOW); 
  Delay_us(750);   
  DS18B20_DATA_OUT(InOrEx, HIGH); 
  Delay_us(15);   
}


 /**
  * @brief  检测从机给主机返回的存在脉冲
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 无
  */
static uint8_t DS18B20_Presence(int InOrEx)
{
	uint8_t pulse_time = 0;
	
  DS18B20_Mode_IPU(InOrEx);     /* 主机设置为上拉输入 */
  
  /* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
   * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
   */
  while(DS18B20_DATA_IN(InOrEx) && pulse_time<100)
  {
    pulse_time++;
    Delay_us(1);
  }	
 
  if( pulse_time >=100 )        /* 经过100us后，存在脉冲都还没有到来*/
    return 1;
  else
    pulse_time = 0;
  
  while(!DS18B20_DATA_IN(InOrEx) && pulse_time<240)   /* 存在脉冲到来，且存在的时间不能超过240us */
  {
    pulse_time++;
    Delay_us(1);
  }	
  if( pulse_time >=240 )
    return 1;
  else
    return 0;	
}

 /**
  * @brief  向DS18B20发送一个字节数据
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  *         dat   :   要发送的数据
  * @retval 无
  */
void DS18B20_Write_Byte(int InOrEx, uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP(InOrEx);
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		if (testb)
		{			
      DS18B20_DATA_OUT(InOrEx, LOW);
      Delay_us(8); 
      DS18B20_DATA_OUT(InOrEx, HIGH);
      Delay_us(58);
		}		
		else
		{		
      DS18B20_DATA_OUT(InOrEx, LOW);
      Delay_us(70);
      DS18B20_DATA_OUT(InOrEx, HIGH);			
      Delay_us(2);
		}
	}
}

 /**
  * @brief  从DS18B20读一位数据
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 读到的数据
  */
static uint8_t DS18B20_Read_Bit(int InOrEx)
{
	uint8_t dat;
	
	DS18B20_Mode_Out_PP(InOrEx);
	DS18B20_DATA_OUT(InOrEx, LOW);
	Delay_us(10);
	DS18B20_Mode_IPU(InOrEx);
	if( DS18B20_DATA_IN(InOrEx) == SET )
		dat = 1;
	else
		dat = 0;

	Delay_us(45);
	return dat;
}

 /**
  * @brief  从DS18B20读一个字节数据
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 读到的数据
  */
uint8_t DS18B20_Read_Byte(int InOrEx)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit(InOrEx);		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

 /**
  * @brief  获取温度
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 温度值
  */
float DS18B20_Get_Temp(int InOrEx)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst(InOrEx);	   
	DS18B20_Presence(InOrEx);	 
	DS18B20_Write_Byte(InOrEx, 0XCC);				/* 跳过 ROM */
	DS18B20_Write_Byte(InOrEx, 0X44);				/* 开始转换 */
	
	DS18B20_Rst(InOrEx);
  DS18B20_Presence(InOrEx);
	DS18B20_Write_Byte(InOrEx, 0XCC);				/* 跳过 ROM */
  DS18B20_Write_Byte(InOrEx, 0XBE);				/* 读温度值 */
	
	tplsb = DS18B20_Read_Byte(InOrEx);		 
	tpmsb = DS18B20_Read_Byte(InOrEx); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* 负温度 */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}

 /**
  * @brief  初始化DS18B20引脚
  * @param  InOrEx：  DS18B20IN：选择板载DS18B20
  *                   DS18B20EX：选择外部DS18B20
  * @retval 0： 初始化成功
  *         1： 初始化失败
  */
uint8_t DS18B20_Init (int InOrEx)
{
  if (InOrEx == DS18B20IN)    //初始化板载DS18B20
  {
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_DS18B20IN, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20IN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_DS18B20IN, &GPIO_InitStructure);
    
    DS18B20_Rst(DS18B20IN);	
    return DS18B20_Presence(DS18B20IN);
  }
  else                         //初始化外部DS18B20
  {
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_DS18B20EX, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20EX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_DS18B20EX, &GPIO_InitStructure);
    
    DS18B20_Rst(DS18B20EX);	
    return DS18B20_Presence(DS18B20EX);
  }  
}
