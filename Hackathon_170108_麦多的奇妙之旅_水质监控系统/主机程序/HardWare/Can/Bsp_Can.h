#ifndef __Bsp_Can_H
#define __Bsp_Can_H
#include "common.h"

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 Can_Send_Msg(u8* msg,u8 len);						//发送数据
u8 Can_Receive_Msg(u8 *buf);							//接收数据

#endif 
