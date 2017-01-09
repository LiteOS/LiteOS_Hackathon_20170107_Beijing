#include "pcf8563.h"
#include "iic.h"
#include "usart3.h"
#include "comm_struct.h"
#include "sbus.h"
unsigned char dec2bcd(unsigned char dec);
unsigned char bcd2dec(unsigned char bcd);
/*pcf8563初始化*/
void pcf8563_init(void)
{
    Iic1_Init();
}


/*测试用*/
void pcf8563_get_sec(void)
{
	unsigned char sec, min, hour, day, month, year;	
	//I2C1_WriteByte(PCF8563_WRITE_ADDR, 0x00, 0x00);
	sec = I2C1_ReadByte(PCF8563_READ_ADDR, SEC_ADDR) & 0x7f;
	min = I2C1_ReadByte(PCF8563_READ_ADDR, MIN_ADDR) & 0x7f;
	hour = I2C1_ReadByte(PCF8563_READ_ADDR, HOUR_ADDR) & 0x3f;
	day = I2C1_ReadByte(PCF8563_READ_ADDR, DAY_ADDR) & 0x3f;
	month = I2C1_ReadByte(PCF8563_READ_ADDR, MON_ADDR) & 0x1f;
	year = I2C1_ReadByte(PCF8563_READ_ADDR, YEAR_ADDR) & 0xff;
	  //sec = bcd2dec(sec);
	  //min = bcd2dec(min);
	  //hour = bcd2dec(hour);
	  //day = bcd2dec(day);
		//month = bcd2dec(month);
		//year = bcd2dec(year);
	
    usart3_printf("20%x-%x-%x,%x:%x:%x\r\n", year, month, day, hour, min, sec);
}
/*获取时间，并将时间串保存到缓冲区中，以备调用*/
void pcf8563_get_time(void)
{
	unsigned char sec, min, hour, day, month, year;	
	dev_time_t dev_time;
	//I2C1_WriteByte(PCF8563_WRITE_ADDR, 0x00, 0x00);
	sec = I2C1_ReadByte(PCF8563_READ_ADDR, SEC_ADDR) & 0x7f;
	min = I2C1_ReadByte(PCF8563_READ_ADDR, MIN_ADDR) & 0x7f;
	hour = I2C1_ReadByte(PCF8563_READ_ADDR, HOUR_ADDR) & 0x3f;
	day = I2C1_ReadByte(PCF8563_READ_ADDR, DAY_ADDR) & 0x3f;
	month = I2C1_ReadByte(PCF8563_READ_ADDR, MON_ADDR) & 0x1f;
	year = I2C1_ReadByte(PCF8563_READ_ADDR, YEAR_ADDR) & 0xff;
	dev_time.sec = sec;
	dev_time.min = min;
	dev_time.hour = hour;
	dev_time.day = day;
	dev_time.month = month;
	dev_time.year = year;
	sbus_set(DEV_TIME_INFO_ADDR, DEV_TIME_INFO_LENGTH, (const uint16_t*)&dev_time);
	
}
void pcf8563_set_time(stime_t *stime)
{
	  unsigned char i;	
	  unsigned char i2c_data[8];
	  i2c_data[0] = stime->second;
	  i2c_data[1] = stime->mint;
	  i2c_data[2] = stime->hour;
	  i2c_data[3] = stime->day;
	  i2c_data[4] = stime->month;
	  i2c_data[5] = stime->year;
	  for(i = 0; i < 6; i++){
				i2c_data[i] = dec2bcd(i2c_data[i]);
		}
	  I2C1_WriteByte(PCF8563_WRITE_ADDR, SEC_ADDR, i2c_data[0]);
	  I2C1_WriteByte(PCF8563_WRITE_ADDR, MIN_ADDR, i2c_data[1]);
	  I2C1_WriteByte(PCF8563_WRITE_ADDR, HOUR_ADDR, i2c_data[2]);
	  I2C1_WriteByte(PCF8563_WRITE_ADDR, DAY_ADDR, i2c_data[3]);
	  I2C1_WriteByte(PCF8563_WRITE_ADDR, MON_ADDR, i2c_data[4]);
	  I2C1_WriteByte(PCF8563_WRITE_ADDR, YEAR_ADDR, i2c_data[5]);
	 
}

//10进制转成BCD码
unsigned char dec2bcd(unsigned char dec)
{
		unsigned char bcd;
	  bcd = ((dec / 10)  *16) + (dec % 10);
	  //bcd = dec;
	  return bcd;
}

unsigned char bcd2dec(unsigned char bcd)
{
		unsigned char dec;
	  dec = 10*(bcd >> 4) + (bcd&0xf0);
	  return dec;
}
