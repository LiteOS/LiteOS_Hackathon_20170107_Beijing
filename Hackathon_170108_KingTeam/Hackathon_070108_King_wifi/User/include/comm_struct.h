/*----------------------------------------------------*/
/* 文件：comm_struct.h
   功能：定义各种数据的结构体
   版本：V1.1 又增加了一些结构体定义，包括命令，状态，电池信息
   历史版本：V1.0 下面的结构适用于采集端监测            */
/*----------------------------------------------------*/
#ifndef COMM_STRUCT_H_
#define COMM_STRUCT_H_
#include <stdint.h>
#include <stdio.h>
//水田 paddy 旱田 glebe 水渠 canal 温室 greenhouse 气象站 weather station 植保  plant protection

#ifdef	__cplusplus
extern "C" {
#endif
	
/*控制指令cmd*/
typedef struct _cmd_info_t {
	uint16_t cmd_info;
} cmd_t;
/*状态信息state*/
typedef struct _state_info_t {
	uint16_t sta_info;
} state_t;
/*电池状态*/
typedef struct _battery_info_t{
	uint16_t v_info;
	uint16_t i_info;
} battery_t;
/*设备id*/
typedef struct _devid_info_t {
	uint16_t dev_id;
} devid_t;
/*传感器监测*/
/*水田*/
typedef struct _paddy_info_t {
    uint16_t water_l;   /*水位*/
    int16_t  water_t;   /*水温*/
    int16_t  silt_t;    /*泥温*/
    uint16_t water_ph;  /*PH*/
    uint16_t water_do;  /*溶解氧*/
	uint16_t water_ec;  /*电导率*/
} paddy_t;
/*旱田*/
typedef struct _glebe_info_t {
	int16_t  soil_t;    /*土壤温度*/
    int16_t  soil_5h;   /*5cm土壤湿度*/
    int16_t  soil_10h;  /*10cm土壤湿度*/
	int16_t  soil_15h; 	/*15cm土壤湿度*/
	uint16_t soil_ph;   /*土壤PH*/
	int16_t  air_t;  	/*空气温度*/
	int16_t  air_h;  	/*空气湿度*/
	uint16_t air_l;  	/*光照*/
} glebe_t;
/*水渠*/
typedef struct _cannl_info_t {
    uint16_t water_l;   /*水位*/
    int16_t  water_t;   /*水温*/
    uint16_t water_ph;  /*PH*/
    uint16_t water_do;  /*溶解氧*/
	uint16_t water_ec;  /*电导率*/
	uint16_t water_ntu;  /*浊度*/
} canal_t;
/*温室*/
typedef struct _greenhouse_info_t {
    int16_t soil_t;   	/*土壤温度*/
    int16_t soil_5h;   	/*5cm土壤湿度*/
    int16_t soil_10h;  	/*10cm土壤湿度*/
	int16_t air_t;  	/*空气温度*/
	int16_t air_h;  	/*空气湿度*/
	uint16_t air_co2;  	/*二氧化碳*/
	uint16_t air_l;  	/*光照*/
	uint16_t soil_ph;  	/*ph*/
	uint16_t air_nh;  	/*氨气*/
} greenhouse_t;
/*气象站*/
typedef struct _wea_station_info_t {
	int16_t air_t;  	/*空气温度*/
	int16_t air_h;  	/*空气湿度*/
	uint16_t air_l;  	/*光照*/
	uint16_t air_co2;  	/*二氧化碳*/
	uint16_t soil_ev;  	/*蒸发*/
	uint16_t air_rain;  /*雨量*/
	uint16_t air_s;  	/*风速*/
	uint16_t air_w;  	/*风向*/
	uint16_t air_rs;  	/*雨雪*/
} wea_station_t;
/*植保*/
typedef struct _plant_protect_info_t {

	uint16_t bugs_n;  	/*虫数*/
} plant_protect_t;

/*设备状态*/
/*0：正常运行；1：设备上线；2：设备关机；3：设备故障；4：设备重启*/
typedef struct _dev_status_info_t{
    uint16_t device_status;      /*设备状态*/
} dev_status_t;
/*时间*/
typedef struct _dev_time_info_t{
	uint16_t sec;
	uint16_t min;
	uint16_t hour;
	uint16_t day;
	uint16_t month;
	uint16_t year;
} dev_time_t;

/* 空，占位      #1     */
/* 0                    */
#define BASE_ADDRESS                      	(0x00)
/*命令*/
#define CMD_INFO_ADDR						(BASE_ADDRESS)
#define CMD_INFO_LENGTH						(sizeof(cmd_t) / sizeof(uint16_t))
/*状态*/
#define STATE_INFO_ADDR						(BASE_ADDRESS + CMD_INFO_LENGTH)
#define STATE_INFO_LENGTH				    (sizeof(state_t) / sizeof(uint16_t))
/*电池*/
#define BATTERY_INFO_ADDR					(STATE_INFO_ADDR + STATE_INFO_LENGTH)
#define BATTERY_INFO_LENGTH					(sizeof(battery_t) / sizeof(uint16_t))
	
/*设备ID*/
#define DEVID_INFO_ADDR           			(BATTERY_INFO_ADDR + BATTERY_INFO_LENGTH)
#define DEVID_INFO_LENGTH        			(sizeof(devid_t) / sizeof(uint16_t))

/* 水田监测首地址及长度 */
#define PADDY_INFO_ADDR           			(DEVID_INFO_ADDR + DEVID_INFO_LENGTH)
#define PADDY_INFO_LENGTH        			(sizeof(paddy_t) / sizeof(uint16_t))
/*旱田监测首地址及长度 */
#define GLEBE_INFO_ADDR           			(PADDY_INFO_ADDR + PADDY_INFO_LENGTH)
#define GLEBE_INFO_LENGTH        			(sizeof(glebe_t) / sizeof(uint16_t))
/*水渠监测首地址及长度*/
#define CANAL_INFO_ADDR           			(GLEBE_INFO_ADDR + GLEBE_INFO_LENGTH)
#define CANAL_INFO_LENGTH        			(sizeof(canal_t) / sizeof(uint16_t))
/*温室监测首地址及长度*/
#define GREENHOUSE_INFO_ADDR           		(CANAL_INFO_ADDR + CANAL_INFO_LENGTH)
#define GREENHOUSE_INFO_LENGTH        		(sizeof(greenhouse_t) / sizeof(uint16_t))
/*气象站监测首地址及长度*/
#define WEA_STATION_INFO_ADDR           	(GREENHOUSE_INFO_ADDR + GREENHOUSE_INFO_LENGTH)
#define WEA_STATION_INFO_LENGTH        		(sizeof(wea_station_t) / sizeof(uint16_t))
/*植保监测首地址及长度*/
#define PLANT_PROTECT_INFO_ADDR           	(WEA_STATION_INFO_ADDR + WEA_STATION_INFO_LENGTH)
#define PLANT_PROTECT_INFO_LENGTH        	(sizeof(plant_protect_t) / sizeof(uint16_t))
/*设备运行状态首地址及长度*/
#define DEV_STATUS_INFO_ADDR           		(PLANT_PROTECT_INFO_ADDR + PLANT_PROTECT_INFO_LENGTH)
#define DEV_STATUS_INFO_LENGTH        		(sizeof(dev_status_t) / sizeof(uint16_t))
/*设备运行时间首地址及长度*/
#define DEV_TIME_INFO_ADDR           		(DEV_STATUS_INFO_ADDR + DEV_STATUS_INFO_LENGTH)
#define DEV_TIME_INFO_LENGTH        		(sizeof(dev_time_t) / sizeof(uint16_t))
#ifdef	__cplusplus
}
#endif

#endif // COMM_STRUCT_H_
