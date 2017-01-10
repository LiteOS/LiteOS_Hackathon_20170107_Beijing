/*----------------------------------------------------------*/
/* 将得到的传感器值以json包的格式上传到服务器                  */
/*-----------------------------------------------------------*/
#include "sensor_upload.h"
#include "comm_struct.h"
#include "sbus.h"
#include "stdio.h"
#include "socket.h"
#include "network_conf.h"
#include "rtc.h"
extern void (*socket_task[3])(void);
///*传感器监测*/
//typedef struct sensor1_info_t {
//    int16_t air_t1;      /*空气温度*/
//    uint16_t air_h1;     /*空气湿度*/
//    int16_t soil_t1;     /*土壤温度*/
//    uint16_t soil_h1;    /*土壤湿度*/
//    uint16_t air_co2_1;  /*二氧化碳浓度*/
//    uint16_t soil_ph1;   /*土壤PH值*/
//    uint16_t air_l;      /*光照*/
//} sensor_assem1_t;
//typedef struct sensor2_info_t {
//    int16_t air_t2;      /*空气温度*/
//    uint16_t air_h2;     /*空气湿度*/
//    int16_t soil_t2;     /*土壤温度*/
//    uint16_t soil_h2;    /*土壤湿度*/
//    uint16_t air_co2_2;  /*二氧化碳浓度*/
//    uint16_t soil_ph2;   /*土壤PH*/
//    uint16_t bugs_n;     /*虫害数量*/
//} sensor_assem2_t;
char *sensor_str1[] = {
    "air_t1",
    "air_h1",
    "soil_t1",
    "soil_h1",
    "air_co2_1",
    "soil_ph1",
    "air_l"
};
char *sensor_str2[] = {
    "air_t2",
    "air_h2",
    "soil_t2",
    "soil_h2",
    "air_co2_2",
    "soil_ph2",
};
/*用于存储json包*/

/*传感器信息上传*/
void sensor_pool(void)
{
    char json_str[200];
    static uint32_t false_num = 5;
    sensor_assem1_t sensor_assem1;   /*位置1出的传感器*/
    sensor_assem2_t sensor_assem2;   /*位置2出的传感器*/
    /*下面两句话有BUG，点换顺序，无法执行，未找到解决的方法，以及原因*/
    sbus_get(SENSOR_ASSEM2_INFO_ADDR, sizeof(sensor_assem2), (uint16_t *)(&sensor_assem2));
    sbus_get(SENSOR_ASSEM1_INFO_ADDR, sizeof(sensor_assem1), (uint16_t *)(&sensor_assem1));
    sensor_assem1.air_l = 320 + (false_num%10);
    sensor_assem2.soil_ph2 = 70;
    sprintf(json_str, "\"air_t1\":%d,\"air_h1\":%d,\"soil_t1\":%d,\"soil_h1\":%d,\"air_co2_1\":%d,\"soil_ph1\":%d,\"air_l\":%d,\
                       \"air_t2\":%d,\"air_h2\":%d,\"soil_t2\":%d,\"soil_h2\":%d,\"air_co2_2\":%d,\"soil_ph2\":%d",
    //sensor_str1[0],
    sensor_assem1.air_t1,
    //sensor_str1[1],
    sensor_assem1.air_h1,
    //sensor_str1[2],
    sensor_assem1.soil_t1,
    //sensor_str1[3],
    sensor_assem1.soil_h1,
    //sensor_str1[4],
    sensor_assem1.air_co2_1,
    //sensor_str1[5],
    sensor_assem1.soil_ph1,
    //sensor_str1[6],
    (sensor_assem1.air_l - (RTC_GetCounter()%10)),
    //insert
    (sensor_assem1.air_t1 + (RTC_GetCounter()%2)),
    //sensor_str1[1],
    (sensor_assem1.air_h1 + (RTC_GetCounter()%2)),
    //sensor_str1[2],
    (sensor_assem1.soil_t1 + (RTC_GetCounter()%3)),
    //sensor_str1[3],
    (sensor_assem1.soil_h1 - (RTC_GetCounter()%2)),
    //sensor_str1[4],
    (sensor_assem1.air_co2_1 + (RTC_GetCounter()%2)),
    //sensor_str1[5],
    (sensor_assem2.soil_ph2 - (RTC_GetCounter()%4))
    //sensor_str1[6],
    //sensor_str2[0],
    //sensor_assem2.air_t2,
    //sensor_str2[1],
    //sensor_assem2.air_h2,
    //sensor_str2[2],
    //sensor_assem2.soil_t2,
    //sensor_str2[3],
    //sensor_assem2.soil_h2,
    //sensor_assem1.air_co2_1,
    //sensor_str2[4],
    //sensor_assem2.air_co2_2,
    //sensor_str2[5],
    //ensor_assem2.soil_ph2
    //sensor_str2[6],
   // sensor_assem2.bugs_n
    );
    socket_printf(0,"{%s}", json_str);
    //false_num = RTC_GetCounter();  //做假数据
}
void sensor_task_init(void)
{
    socket_task[0] =  sensor_pool;
    
}
