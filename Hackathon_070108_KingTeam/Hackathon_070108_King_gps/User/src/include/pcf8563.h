#ifndef __PCF8563_H
#define __PCF8563_H

//PCF8563 读写地址
#define PCF8563_READ_ADDR	  0xA3
#define PCF8563_WRITE_ADDR  0xA2
//RTC 时间寄存器表
#define  SEC_ADDR  		0x02
#define  MIN_ADDR    	0x03
#define  HOUR_ADDR   	0x04
#define  DAY_ADDR     0x05
#define  MON_ADDR   	0x07
#define  YEAR_ADDR    0x08

/**RTC时间结构体**********************/
typedef struct time_info
{
 unsigned char year;
 unsigned char month;
 unsigned char day;
 unsigned char hour;
 unsigned char mint;
 unsigned char second;

}stime_t;

void pcf8563_get_sec(void);
//pcf8563初始化
void pcf8563_init(void);
//pcf8563设定时间
void pcf8563_set_time(stime_t *stime);
/*获取时间*/
void pcf8563_get_time(void);
#endif  //__PCF8563_H
