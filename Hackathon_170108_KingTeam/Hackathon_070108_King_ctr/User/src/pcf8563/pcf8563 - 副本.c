#include "pcf8563.h"
#include "i2c_hard.h"
#include "usart3.h"
extern I2C_INIT_INFO I2C_Info;	///<I2C控制器相关信息
uint8_t I2C_Data[16] ,ret;

unsigned char dec2bcd(unsigned char dec);
/*pcf8563初始化*/
void pcf8563_init(void)
{
    //根据PCF8563设置I2C初始化数据
	I2C_Info.controlMode = 1;//硬件控制
	I2C_Info.speed = 50000;//100K
	I2C_Info.subAddrMode = 2;//无子地址模式
	I2C_Info.subAddrWidth = 0;//子地址为0字节宽度
	I2C_Info.slaveAddr = PCF8563_WRITE_ADDR;//I2C地址
	I2C_Info.channel = I2C1_CHANNEL;//使用I2C1, 
	VT_I2C_HardInit(&I2C_Info);
	//I2C_Data[0] = 0x00;
	//VT_I2C_HardWriteNByte(I2C1,0,I2C_Data,1);//时钟芯片上电命令
}

//int8_t bh1750_get_lux_val(float *lux_val)
//{
//    I2C_Data[0] = 0x11;  /*高分辨模式, 0.5lux*/
//    ret = VT_I2C_HardWriteNByte(I2C1,0,I2C_Data,1);//发送测量命令
//    if(ret)
//    {
//        xassert(ret == BH1750_WRITE_OK);
//        return BH1750_WRITE_ERROR;     //返回一个达不到的错误值
//    }
//    
//    delay_ms(120);     //等待大于120ms，必须加这个延时，否则传感器没准备好数据会出现读数据错误
//    ret = VT_I2C_HardReadNByte(I2C1,0,I2C_Data,2);//读取光照强度值
//    if(ret)
//    {
//        xassert(ret == BH1750_READ_OK);
//        return BH1750_READ_ERROR;   //返回一个达不到的错误值
//    }
//    *lux_val = ((I2C_Data[0]<<8)+I2C_Data[1])/(1.2*2);
//    return BH1750_READ_OK;
//    
//    
//}

void pcf8563_get_sec(void)
{
		I2C_Data[0] = 0x00;
	  VT_I2C_HardWriteNByte(I2C1,0,&I2C_Data[0],1);//时钟芯片上电命令
	  ret = VT_I2C_HardReadNByte(I2C1,0,I2C_Data,7);
	  usart3_printf("%x:%x:%x\r\n", I2C_Data[4]&0x1f,I2C_Data[3]&0x7f,I2C_Data[2]&0x7f);
}
void pcf8563_set_time(stime_t *stime)
{
		unsigned char i;	
	  //I2C_Data[0] = 0x00;  //
	  // I2C_Data[1] = 0x00;
	  I2C_Data[2] = 0x00;
		I2C_Data[3] = 0;
		I2C_Data[4] = stime->hour;
		I2C_Data[5] = stime->day;
		I2C_Data[6] = stime->month;
	  I2C_Data[7] = stime->year;
//	  for(i = 2; i <  8; i++){
//				I2C_Data[i] = dec2bcd(I2C_Data[i]);
//		}
	  I2C_Data[0] = 0x20;
	  VT_I2C_HardWriteNByte(I2C1,0,&I2C_Data[0],1);//时钟芯片上电命令
		I2C_Data[0] = 0x00;
	  VT_I2C_HardWriteNByte(I2C1,0,&I2C_Data[0],1);//时钟芯片上电命令
	  VT_I2C_HardWriteNByte(I2C1,2,&I2C_Data[2],1);//时钟芯片停止运行
		
	  //VT_I2C_HardWriteNByte(I2C1,2,&I2C_Data[2],1);
//    VT_I2C_HardWriteNByte(I2C1,3,&I2C_Data[3],1);
//		VT_I2C_HardWriteNByte(I2C1,4,&I2C_Data[4],1);
//		VT_I2C_HardWriteNByte(I2C1,5,&I2C_Data[5],1);
//		VT_I2C_HardWriteNByte(I2C1,7,&I2C_Data[6],1);
//		VT_I2C_HardWriteNByte(I2C1,8,&I2C_Data[7],1);	
//	  I2C_Data[0] = 0x00;
//		VT_I2C_HardWriteNByte(I2C1,0,&I2C_Data[0],1);//时钟芯片开始运行
}

//10进制转成BCD码
unsigned char dec2bcd(unsigned char dec)
{
		unsigned char bcd;
	  bcd = ((dec / 10)  *16) + (dec % 10);
	  return bcd;
}
