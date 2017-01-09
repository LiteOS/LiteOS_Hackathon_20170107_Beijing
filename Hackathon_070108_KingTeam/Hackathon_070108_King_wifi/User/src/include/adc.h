#ifndef __ADC_H_
#define __ADC_H_
#include "stm32f10x.h"
#define N   50
#define M   4
/*adc初始化*/
void adc_init(void);
/*求平均值函数*/
void filter(void);
/*获取 ADC 的值，将二进制换算为十进制*/
u16 GetVolt(u16 advalue);
/*获取adc值*/
uint16_t adc_val(uint8_t ch);

#endif
