#include "DS18B20.h"

#define HIGH  1
#define LOW   0

#define DS18B20_IN1_CLK     RCC_APB2Periph_GPIOA
#define DS18B20_IN1_PIN     GPIO_Pin_0                 
#define DS18B20_IN1_PORT		GPIOA 

/*******************************************************************************
* 函数名	: Delay_1us
* 描述	    : us延时函数
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
extern void Delay_1us(uint16_t time)
{
	uint16_t i=0;
	while(time--)
	{
		i=10;
		while(i--);
	}
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_GPIO_Config 
* 描述	    : 18B20引脚配置为输入模式
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
void DS18B20_IN1_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启DS18B20_IN1_PORT的外设时钟*/
	RCC_APB2PeriphClockCmd(DS18B20_IN1_CLK, ENABLE); 

	/*选择要控制的DS18B20_IN1_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = DS18B20_IN1_PIN;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化DS18B20_IN1_PORT*/
  	GPIO_Init(DS18B20_IN1_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(DS18B20_IN1_PORT, DS18B20_IN1_PIN);	 
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Mode_IPU 
* 描述	    : 使DS18B20_IN1-DATA引脚变为输入模式
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
static void DS18B20_IN1_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*选择要控制的DS18B20_IN1_PORT引脚*/	
	GPIO_InitStructure.GPIO_Pin = DS18B20_IN1_PIN;

	/*设置引脚模式为浮空输入模式*/ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	

	/*调用库函数，初始化DS18B20_IN1_PORT*/
	GPIO_Init(DS18B20_IN1_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Mode_Out_PP 
* 描述	    : 使DS18B20_IN1-DATA引脚变为输出模式
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
static void DS18B20_IN1_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*选择要控制的DS18B20_IN1_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = DS18B20_IN1_PIN;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*调用库函数，初始化DS18B20_IN1_PORT*/
  	GPIO_Init(DS18B20_IN1_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Rst 
* 描述	    : 主机给从机发送复位脉冲
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
static void DS18B20_IN1_Rst(void)
{
	/* 主机设置为推挽输出 */
	DS18B20_IN1_Mode_Out_PP();
	
	DS18B20_IN1_DATA_OUT(LOW);
	/* 主机至少产生480us的低电平复位信号 */
	Delay_1us(750);
	
	/* 主机在产生复位信号后，需将总线拉高 */
	DS18B20_IN1_DATA_OUT(HIGH);
	
	/*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
	Delay_1us(15);
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Presence 
* 描述	    : 检测从机给主机返回的存在脉冲
* 输入参数  : 无
* 返回参数  : 0：成功
* 			  1：失败
********************************************************************************/
static uint8_t DS18B20_IN1_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* 主机设置为上拉输入 */
	DS18B20_IN1_Mode_IPU();
	
	/* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
	 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
	 */
	while( DS18B20_IN1_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		Delay_1us(1);
	}	
	/* 经过100us后，存在脉冲都还没有到来*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* 存在脉冲到来，且存在的时间不能超过240us */
	while( !DS18B20_IN1_DATA_IN() && pulse_time<240 )
	{
		pulse_time++;
		Delay_1us(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Read_Bit 
* 描述	    : 从DS18B20_IN1读取一个bit
* 输入参数  : 无
* 返回参数  : uint8_t dat
********************************************************************************/
static uint8_t DS18B20_IN1_Read_Bit(void)
{
	uint8_t dat;
	
	/* 读0和读1的时间至少要大于60us */	
	DS18B20_IN1_Mode_Out_PP();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	DS18B20_IN1_DATA_OUT(LOW);
	Delay_1us(10);
	
	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	DS18B20_IN1_Mode_IPU();
	//Delay_1us(2);
	
	if( DS18B20_IN1_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* 这个延时参数请参考时序图 */
	Delay_1us(45);
	
	return dat;
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Read_Byte 
* 描述	    : 从DS18B20_IN1读一个字节，低位先行
* 输入参数  : 无
* 返回参数  : uint8_t dat
********************************************************************************/
uint8_t DS18B20_IN1_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_IN1_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Write_Byte 
* 描述	    : 写一个字节到DS18B20_IN1，低位先行
* 输入参数  : 无
* 返回参数  : uint8_t dat
********************************************************************************/
void DS18B20_IN1_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_IN1_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* 写0和写1的时间至少要大于60us */
		if (testb)
		{			
			DS18B20_IN1_DATA_OUT(LOW);
			/* 1us < 这个延时 < 15us */
			Delay_1us(8);
			
			DS18B20_IN1_DATA_OUT(HIGH);
			Delay_1us(58);
		}		
		else
		{			
			DS18B20_IN1_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			Delay_1us(70);
			
			DS18B20_IN1_DATA_OUT(HIGH);			
			/* 1us < Trec(恢复时间) < 无穷大*/
			Delay_1us(2);
		}
	}
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Start 
* 描述	    : DS18B20_IN1启动
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************/
void DS18B20_IN1_Start(void)
{
	DS18B20_IN1_Rst();	   
	DS18B20_IN1_Presence();	 
	DS18B20_IN1_Write_Byte(0XCC);		/* 跳过 ROM */
	DS18B20_IN1_Write_Byte(0X44);		/* 开始转换 */
}

/*******************************************************************************
* 函数名	: DS18B20_IN1_Init 
* 描述	    : DS18B20_IN1初始化
* 输入参数  : 无
* 返回参数  : 0：成功
* 			  1：失败
********************************************************************************/
uint8_t DS18B20_IN1_Init(void)
{
	DS18B20_IN1_GPIO_Config();
	DS18B20_IN1_Rst();
	
	return DS18B20_IN1_Presence();
}

/*
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */
u8 DS18B20_IN1_Get_Temp(void)
{
    u8 TL,TH,t1,t;
	
	DS18B20_IN1_Rst();	   
	DS18B20_IN1_Presence();	 
	DS18B20_IN1_Write_Byte(0XCC);				/* 跳过 ROM */
	DS18B20_IN1_Write_Byte(0X44);				/* 开始转换 */
	
	DS18B20_IN1_Rst();
    DS18B20_IN1_Presence();
	DS18B20_IN1_Write_Byte(0XCC);				/* 跳过 ROM */
    DS18B20_IN1_Write_Byte(0XBE);				/* 读温度值 */
	
	TL = DS18B20_IN1_Read_Byte();
	TH = DS18B20_IN1_Read_Byte();
	//取整数的方式
	t1=(TL&0x0f)*0.0625;//取小数部分

	t=((TH<<4)|(TL>>4));//取整数部分
	if(t>0xc0)
	{
	t=~t+1; 

	}
	t=t+t1;


	return(t); 
}

/*************************************END OF FILE******************************/
