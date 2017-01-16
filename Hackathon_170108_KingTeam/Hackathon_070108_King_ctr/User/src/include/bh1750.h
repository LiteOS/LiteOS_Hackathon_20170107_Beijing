#ifndef	__BH1750_H_
#define __BH1750_H_

#include "stm32f10x.h"
#define BH1750_WRITE_OK     0    //写正常
#define BH1750_READ_OK      0    //读正常
#define BH1750_WRITE_ERROR  -1   // -1
#define BH1750_READ_ERROR   -1   // -1
int8_t bh1750_get_lux_val(float *lux_val);  /*从bh1750中读取光照值*/
void bh1750_init(void);   /*BH1750初始化*/
#endif
//*********************			END OF FILE			*********************
