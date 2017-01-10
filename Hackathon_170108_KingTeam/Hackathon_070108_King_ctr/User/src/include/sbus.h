#ifndef __SBUS_H_
#define __SBUS_H_
#include "stdint.h"
/* 从缓冲区中读取数据 */
void sbus_get(uint16_t addr, uint16_t len, uint16_t *buf);
/* 将数据加载到内存段中 */
void sbus_set(uint16_t addr, uint16_t len, const uint16_t *buf);


#endif // __SBUS_H_
