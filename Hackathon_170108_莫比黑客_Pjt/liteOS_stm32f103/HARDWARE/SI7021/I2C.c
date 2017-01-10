#include "I2C.h"
#include "Delay.h"
#include "stm32f10x_gpio.h"
//===================================================================== 
//函 数 名:	 I2C_GPIO_Config
//功    能:  
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//=====================================================================
  void I2C_GPIO_Config(void)
  {
 GPIO_InitTypeDef    GPIO_InitStructure;  

  GPIO_InitStructure.GPIO_Pin = I2C_SCL;          
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = I2C_SDA;           
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  }
/*
 void I2C_GPIO_Config(void)
{
	 
 	RCC->APB2ENR|=1<<3;//先使能外设PORTB时钟						 
	I2C_PORT->CRL&=0X00FFFFFF;//PB6 7 推挽输出
	I2C_PORT->CRL|=0X33000000;	   
	I2C_PORT->ODR|=0X00C0;     //PB6 7 输出高
    
}
*/
//===================================================================== 
//函 数 名: I2C_Start(void)
//功    能: 产生I2C起始信号 
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//=====================================================================
void I2C_Start(void)
{
  SDA_OUT();     //sda线输出
	I2C_SDA_H;	  	  
	I2C_SCL_H;
	delay_us(8);
 	I2C_SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(8);
	I2C_SCL_L;//钳住I2C总线，准备发送或接收数据 
}
//===================================================================== 
//函 数 名:	void I2C_Stop(void)
//功    能: 产生I2C停止信号 
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//=====================================================================	  
void I2C_Stop(void)
{
  SDA_OUT();//sda线输出
	I2C_SCL_L;
	I2C_SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(8);
	I2C_SCL_H; 
	I2C_SDA_H;//发送I2C总线结束信号
	delay_us(8);			   	
}
 //===================================================================== 
//函 数 名:	u8 I2C_Wait_Ack(void)
//功    能: 等待应答信号到来 
//入口参数: 无
//出口参数: 无
//返 回 值: 1，接收应答失败	 0，接收应答成功
//=====================================================================        
u8 I2C_Wait_Ack(void)
{ 
	u8 ucErrTime=0;	
  SDA_IN();      //SDA设置为输入 
	I2C_SDA_H;delay_us(1);	   
	I2C_SCL_H;delay_us(1);
 		
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL_L;//时钟输出0 	   
	return 0;  
}
//===================================================================== 
//函 数 名:	void I2C_Ack(void)
//功    能: 产生ACK应答 
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//===================================================================== 
void I2C_Ack(void)
{		
	I2C_SCL_L;
  SDA_OUT();
	I2C_SDA_L;
	delay_us(6);
	I2C_SCL_H;
	delay_us(6);
	I2C_SCL_L;
}
 //===================================================================== 
//函 数 名:	void I2C_NAck(void)
//功    能: 不产生ACK应答 
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//=====================================================================		    
void I2C_NAck(void)
{			
	I2C_SCL_L;
  SDA_OUT();
	I2C_SDA_H;
	delay_us(6);
	I2C_SCL_H;
	delay_us(6);
	I2C_SCL_L;
}
//===================================================================== 
//函 数 名:	void I2C_Send_Byte(u8 txd)
//功    能: I2C发送一个字节 
//入口参数: 无
//出口参数: 无
//返 回 值: 
//=====================================================================					 				     		  
void I2C_Send_Byte(u8 SendByte)
{                        
 u8 t;   
	SDA_OUT(); 	    
    I2C_SCL_L;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
      if((SendByte&0x80)>>7)
        I2C_SDA_H;  
      else 
        I2C_SDA_L;  
        SendByte<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		I2C_SCL_H;
		delay_us(2); 
		I2C_SCL_L;	
		delay_us(2);
    }	 
}
//===================================================================== 
//函 数 名:	u8 I2C_Read_Byte(u8 ack)
//功    能:  读1个字节
//入口参数:ack=1时，发送ACK，ack=0，发送nACK 
//出口参数: 无
//返 回 值: 无
//===================================================================== 	      
u8 I2C_Read_Byte(u8 ack)
{
  u8 i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        I2C_SCL_L; 
        delay_us(2);
		    I2C_SCL_H;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        I2C_NAck();//发送nACK
    else
        I2C_Ack(); //发送ACK   
    return receive;
}



