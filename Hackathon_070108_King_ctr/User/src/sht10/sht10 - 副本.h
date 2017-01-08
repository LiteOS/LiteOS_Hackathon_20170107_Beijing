#ifndef _SHT10_H
#define _SHT10_H
#include "stdint.h"
/* Private typedef -----------------------------------------------------------*/
#define SHT10_SDA   GPIO_Pin_7
#define SHT10_SCK   GPIO_Pin_6
#define SHT10_PORT  GPIOB
 /* Private define ------------------------------------------------------------*/
#define SDA_H()         GPIO_SetBits(SHT10_PORT, SHT10_SDA)
#define SDA_L()         GPIO_ResetBits(SHT10_PORT, SHT10_SDA)
#define SCK_H()         GPIO_SetBits(SHT10_PORT, SHT10_SCK)
#define SCK_L()         GPIO_ResetBits(SHT10_PORT, SHT10_SCK)
 //读SDA数据
#define SDA_R()         GPIO_ReadInputDataBit(SHT10_PORT, SHT10_SDA)

 /* Private macro -------------------------------------------------------------*/
#define noACK 0         //无应答
#define ACK   1         //应答

#define STATUS_REG_W    0x06   //000   0011    0
#define STATUS_REG_R    0x07   //000   0011    1
#define MEASURE_TEMP    0x03   //000   0001    1
#define MEASURE_HUMI    0x05   //000   0010    1
#define S_RESET         0x1e   //000   1111    0
enum {TEMP,HUMI};    //枚举温度、湿度类型
typedef union 
{ 
    uint16_t i;
    float f;
} sht10_t;          //定一个sht10存储类型

/* Private function prototypes -----------------------------------------------*/
void SHT10_Config(void);
void SHT10_SDAIn(void);
void SHT10_SDAOut(void);
uint8_t SHT10_WriteByte(uint8_t value);
uint8_t SHT10_ReadByte(uint8_t Ack);
void SHT10_Start(void);
void SHT10_ConReset(void);
uint8_t SHT10_Measure(uint16_t* pValue, uint8_t* pCheckSum, uint8_t mode);
void SHT10_Cal(uint16_t Temp,uint16_t Hum, float* pTempValue,float* pHumValue);
/*用户函数*/
//sht10初始化
void sht10_init(void);
void calc_sth11(float *p_humidity ,float *p_temperature);
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode);
float calc_dewpoint(float h,float t);
#endif
