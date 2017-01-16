
#ifndef __SHT10_H__
#define __SHT10_H__
#include "stm32f10x.h"

enum {TEMP, HUMI};

/* GPIO相关宏定义 */
#define SHT10_AHB2_CLK	RCC_APB2Periph_GPIOC
#define SHT10_DATA_PIN	GPIO_Pin_10
#define SHT10_SCK_PIN	GPIO_Pin_12
#define SHT10_DATA_PORT	GPIOC
#define SHT10_SCK_PORT	GPIOC

#define SHT10_DATA_H()	GPIO_SetBits(SHT10_DATA_PORT, SHT10_DATA_PIN)			 //拉高DATA数据线
#define SHT10_DATA_L()	GPIO_ResetBits(SHT10_DATA_PORT, SHT10_DATA_PIN)			 //拉低DATA数据线
#define SHT10_DATA_R()	GPIO_ReadInputDataBit(SHT10_DATA_PORT, SHT10_DATA_PIN)	 //读DATA数据线

#define SHT10_SCK_H()	GPIO_SetBits(SHT10_SCK_PORT, SHT10_SCK_PIN)				 //拉高SCK时钟线
#define SHT10_SCK_L()	GPIO_ResetBits(SHT10_SCK_PORT, SHT10_SCK_PIN)			 //拉低SCK时钟线

/* 传感器相关宏定义 */
#define	noACK	0
#define ACK		1
#define ERROR_CRC   -1								
                       //addr  command	 r/w
#define STATUS_REG_W	0x06	//000	 0011	  0	  写状态寄存器
#define STATUS_REG_R	0x07	//000	 0011	  1	  读状态寄存器
#define MEASURE_TEMP 	0x03	//000	 0001	  1	  测量温度
#define MEASURE_HUMI 	0x05	//000	 0010	  1	  测量湿度
#define SOFTRESET       0x1E	//000	 1111	  0	  复位


void SHT10_Config(void);
void sht10_conreset(void);
uint8_t SHT10_SoftReset(void);
int8_t sht10_measure(uint16_t *p_value, uint8_t *p_checksum, uint8_t mode);
void sht10_calculate(uint16_t t, uint16_t rh,float *p_temperature, float *p_humidity);
float sht10_calcu_dewpoint(float t, float h);
/*sht10 初始化*/
void sht10_init(void);
#endif

