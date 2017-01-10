
#ifndef __HARD_I2C_DRIVER_H__
#define __HARD_I2C_DRIVER_H__
#include "stm32f10x.h"
//I2C初始化信息结构体
typedef struct
{
    unsigned char controlMode;  ///<I2C控制方式
    unsigned int  speed;        ///<I2C读写速度
    unsigned char subAddrMode;  ///<是否有子地址
    unsigned char subAddrWidth; ///<子地址字节数
    unsigned short slaveAddr;   ///<设备地址
	unsigned char channel;		///<需要初始化的通道
}I2C_INIT_INFO;

#define MASTER_MODE_SELECT_FAILED                          (1)
#define MASTER_TRANSMITTER_MODE_SELECTED_FAILED            (2)    
#define MASTER_RECEIVER_MODE_SELECTED_FAILED               (3)
#define MASTER_BYTE_RECEIVED_FAILED                        (4)
#define MASTER_BYTE_TRANSMITTING_FAILED                    (5)
#define MASTER_BYTE_TRANSMITTED_FAILED                     (6)
#define MASTER_MODE_ADDRESS10_FAILED                       (7)
#define MASTER_GET_I2C_FLAG_BUSY_FAILED					   (8)

#define I2C1_CHANNEL    0   //使用I2C通道1 channel
#define I2C2_CHANNEL    1   //使用I2C通道2
extern uint8_t VT_I2C_HardInit(I2C_INIT_INFO *I2C_Info);
extern uint8_t VT_I2C_HardWriteNByte(I2C_TypeDef *I2Cx, uint32_t subaddr, uint8_t *s, uint32_t num);
extern uint8_t VT_I2C_HardReadNByte(I2C_TypeDef *I2Cx, uint32_t suba,uint8_t *s,uint32_t num);
#endif

