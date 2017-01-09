#include "bh1750.h"
#include "i2c_hard.h"
#include "ult.h"
#include "xassert.h"
extern I2C_INIT_INFO I2C_Info;	///<I2C控制器相关信息
uint8_t I2C_Data[10] ,ret;


/*BH1750初始化*/
void bh1750_init(void)
{
    //根据BH1750FVI传感器设置I2C初始化数据
	I2C_Info.controlMode = 1;//硬件控制
	I2C_Info.speed = 100000;//100K
	I2C_Info.subAddrMode = 2;//无子地址模式
	I2C_Info.subAddrWidth = 0;//子地址为0字节宽度
	I2C_Info.slaveAddr = 0x46;//BH1750FVI传感器I2C地址
	I2C_Info.channel = I2C1_CHANNEL;//使用I2C1, 
	VT_I2C_HardInit(&I2C_Info);
	I2C_Data[0] = 0x01;
	VT_I2C_HardWriteNByte(I2C1,0,I2C_Data,1);//传感器芯片上电命令
}

/**
 * 从bh1750器件中读取光照值
 * 返回值：OK 0，ERROR -1
 * 读到的值存在lux_val中
 *************************/
int8_t bh1750_get_lux_val(float *lux_val)
{
    I2C_Data[0] = 0x11;  /*高分辨模式, 0.5lux*/
    ret = VT_I2C_HardWriteNByte(I2C1,0,I2C_Data,1);//发送测量命令
    if(ret)
    {
        xassert(ret == BH1750_WRITE_OK);
        return BH1750_WRITE_ERROR;     //返回一个达不到的错误值
    }
    
    //delay_ms(120);     //等待大于120ms，必须加这个延时，否则传感器没准备好数据会出现读数据错误
    ret = VT_I2C_HardReadNByte(I2C1,0,I2C_Data,2);//读取光照强度值
    if(ret)
    {
        xassert(ret == BH1750_READ_OK);
        return BH1750_READ_ERROR;   //返回一个达不到的错误值
    }
    *lux_val = ((I2C_Data[0]<<8)+I2C_Data[1])/(1.2*2);
    return BH1750_READ_OK;
    
    
}
