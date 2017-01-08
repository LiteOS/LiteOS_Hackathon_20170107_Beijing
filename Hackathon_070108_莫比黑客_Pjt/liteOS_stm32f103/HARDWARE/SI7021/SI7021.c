/******************** (C) COPYRIGHT tongxinmao.com ***************************
* 文件名		: si7021.C
* 作者   		: tcm123@126.com
* 创建日期  	: 2016.3.8
* 描述			: 温湿度传感器驱动文件 
* 版本			: V1.0
* 历史记录  	: 暂无
********************************************************************************/
#include "SI7021.h"
#include "I2C.h"
#include "Delay.h"

//===================================================================== 
//函 数 名:	SI7021_Read_Data
//功    能: 从SI7021读取一次数据 
//入口参数:
//出口参数:
//返 回 值:	
//=====================================================================
u8 SI7021_Read_Data(u8 Model,u16 *temp) 
{ 
	u8 RX_Buffer[2];
	I2C_Start(); //起始信号              
	I2C_Send_Byte(SALVE_ADDR);
    if(I2C_Wait_Ack()==1){
		return 0;
	}
	I2C_Send_Byte(Model);
	I2C_Wait_Ack();
  delay_ms(20);
	I2C_Start(); //起始信号 		 	//重新发起始信号
	I2C_Send_Byte(READ_CDM);
	I2C_Wait_Ack();
	RX_Buffer[0]=I2C_Read_Byte(1);
	RX_Buffer[1]=I2C_Read_Byte(0);
	I2C_Stop();
	*temp=((RX_Buffer[0]*256)+RX_Buffer[1]) & ~3;

	return 1;
}
//===================================================================== 
//函 数 名:	SI7021_Read_Data
//功    能: 从SI7021读取温湿度
//入口参数:temp:温度值(范围:0~50°)	 humi:湿度值(范围:20%~90%)
//出口参数:
//返 回 值:	
//=====================================================================
u8 SI7021_Read(u16 *temp,u16 *humi)  
{ 
	u16 Value;
	if(!SI7021_Read_Data(TEMP_NOHOLD_MASTER, &Value ))return 0;
  *temp= ((((long)Value)*1757)>>16) - 469;
  if(!SI7021_Read_Data(HUMI_NOHOLD_MASTER, &Value ))return 0;
  *humi= ((((long)Value)*625)>>15) - 60;
  
  return 1;
}


