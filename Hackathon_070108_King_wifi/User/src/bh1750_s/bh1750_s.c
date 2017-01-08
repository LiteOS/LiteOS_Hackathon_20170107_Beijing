#include "bh1750_s.h"
#include "i2c_s.h"
#include "ult.h"
#include "usart2.h"
#include "los_task.h"
float result_lx=0;
u8 BUF[2]={0};
u16 result=0;

//初始化
void Init_BH1750(void)
{
	i2c_Port_Init();
}

void Cmd_Write_BH1750(u8 cmd)
{
    I2C_Start();                  //起始信号
    I2C_Send_Byte(BH1750_Addr+0);   //发送设备地址+写信号
	while(I2C_Wait_Ack());
    I2C_Send_Byte(cmd);    //内部寄存器地址
	while(I2C_Wait_Ack());
   
    //BH1750_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
	delay_ms(5);
}
void Start_BH1750(void)
{
	Cmd_Write_BH1750(BH1750_ON);	 //power on
	Cmd_Write_BH1750(BH1750_RSET);	//clear
	Cmd_Write_BH1750(BH1750_ONE);  //一次H分辨率模式，至少120ms，之后自动断电模式  
}
void Read_BH1750(void)
{   	
    I2C_Start();                          //起始信号
    I2C_Send_Byte(BH1750_Addr+1);         //发送设备地址+读信号
	while(I2C_Wait_Ack());
	BUF[0]=I2C_Read_Byte(1);  //发送ACK
	BUF[1]=I2C_Read_Byte(0);  //发送NACK

    I2C_Stop();                          //停止信号
    delay_ms(5);
}
void Convert_BH1750(void)
{
	
	result=BUF[0];
	result=(result<<8)+BUF[1];  //合成数据，即光照数据
	
	result_lx=(float)result/1.2;

}
/*获取光照值*/
uint16_t get_lux_val(void)
{
    Init_BH1750();
    Start_BH1750();
    LOS_TaskDelay(180);
	//delay_ms(180);  //延时180ms
	Read_BH1750();  //读出数据
	Convert_BH1750();
	return (uint16_t)result_lx;
	
}

