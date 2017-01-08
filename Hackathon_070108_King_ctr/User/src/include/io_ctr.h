/**************************************/
/* 文件：io_ctr.h
   功能：声明函数，控制功能使用头文件
   版本：1.0
***************************************/
#ifndef IO_CTR_H
#define IO_CTR_H
#include "stm32f10x.h"

/*控制端口初始化，默认关闭所有设备*/
void io_ctr_init(void);
/*开启电源*/
void open_dev(void);
void close_dev(void);
#endif

