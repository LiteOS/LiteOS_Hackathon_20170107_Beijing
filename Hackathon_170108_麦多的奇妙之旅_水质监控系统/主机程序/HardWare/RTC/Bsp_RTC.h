#ifndef __Bsp_RTC_H
#define __Bsp_RTC_H

#include "common.h"

//时间结构体
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;					 

extern long int NowTime;
extern _calendar_obj calendar;//时钟结构体 
extern int User_year, User_month, User_day, User_hour, User_min, User_sec;

u8 RTC_Init(void);
u8 RTC_Get(void);         //更新时间   
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间		

#endif
