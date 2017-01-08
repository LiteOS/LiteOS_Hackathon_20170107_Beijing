#ifndef COMM_STRUCT_H_
#define COMM_STRUCT_H_
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*空气传感器family*/
typedef struct _air_sensor_info_t {
    int16_t air_t;      /*空气温度*/
    uint16_t air_h;     /*空气湿度*/
    uint16_t air_l;     /*光照*/
    uint16_t air_c;     /*二氧化碳*/
    uint16_t air_wd;    /*风向*/
    uint16_t air_ws;    /*风速*/
    uint16_t air_s1;    /*保留*/
    uint16_t air_s2;    /*保留*/
    uint16_t air_s3;    /*保留*/
    uint16_t air_s4;    /*保留*/
} air_sensor_t;
/*土壤传感器family*/
typedef struct _soil_sensor_info_t{
    int16_t soil_t;     /*土壤温度*/
    uint16_t soil_h5;   /*5cm土壤湿度*/
    uint16_t soil_h10;  /*10cm土壤湿度*/
    uint16_t soil_h15;  /*15cm土壤湿度*/
} soil_sensor_t;

/*水质传感器family*/
typedef struct _water_sensor_info_t{
     uint16_t water_l;   /*水位*/
     uint16_t water_p;   /*PH*/
     uint16_t water_e;   /*电导率*/
     uint16_t water_o;   /*溶解氧*/
     uint16_t water_d;   /*浊度*/
} water_sensor_t;


/* 空，占位      #1     */
/* 0                    */
/* 空气传感器首地址及长度 */
#define AIR_SENSOR_INFO_ADDR            (0x00)
#define AIR_SENSOR_INFO_LENGTH          (sizeof(air_sensor_t) / sizeof(uint16_t))
/* 土壤传感器首地址及长度 */
#define SOIL_SENSOR_INFO_ADDR           (0x0B)
#define SOIL_SENSOR_INFO_LENGTH         (sizeof(soil_sensor_t) / sizeof(uint16_t))
/* 水质传感器首地址及长度 */
#define WATER_SENSOR_INFO_ADDR          (0x0F)
#define WATER_SENSOR_INFO_LENGTH        (sizeof(water_sensor_t) / sizeof(uint16_t))
#ifdef	__cplusplus
}
#endif

#endif // COMM_STRUCT_H_
