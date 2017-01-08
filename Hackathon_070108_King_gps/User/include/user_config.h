#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_
#include "types.h"
#include "stdint.h"
#include "network_conf.h"
/* 产品的版本信息*/
typedef struct device_ver_t_{
    uint8_t tpye_str[6]; /*设备类型*/
    uint8_t ver[2];  /*ver[0]:主版本号，ver[1]:副版本号*/
    uint8_t serial_num[3]; /*设备序列号*/
    uint8_t build_date[3]; /*设备出厂日期*/
    
} device_ver_t;
/*设备上传时间配置*/
typedef struct _upload_delay_time_t{
    
   uint8_t delay_times_h;
   uint8_t delay_times_l;
   // uint16_t delay_times;    /*默认秒*/
}upload_delay_time_t;
/*定义一个设备的基本信息配置，存在eeprom中*/
typedef struct device_config_t_{
    device_ver_t device_ver;                       /* 设备版本 */
    net_config_t net_config;                       /*本地设备网络配置*/
    server_config_t server_config;                 /*服务器配置*/
    upload_delay_time_t upload_delay_time;          /*上传时间配置 */
} device_config_t;

/*定义在EEPROM中的位置*/

/*存在eeprom中的首地址*/
#define BASE_ADDR                       (0x10)
/*设备版本信息首地址及长度*/
#define DEVICE_VER_INFO_ADDR            (BASE_ADDR)
#define DEVICE_VER_INFO_LENGTH          (sizeof(device_ver_t) / sizeof(uint8_t))
/*本地网络配置首地址及长度*/
#define NET_CONFIG_INFO_ADDR            (BASE_ADDR + DEVICE_VER_INFO_LENGTH)
#define NET_CONFIG_INFO_LENGTH          (sizeof(net_config_t) / sizeof(uint8_t))
/*服务端配置首地址及长度*/  
#define SERVER_CONFIG_INFO_ADDR         (NET_CONFIG_INFO_ADDR + NET_CONFIG_INFO_LENGTH)
#define SERVER_CONFIG_INFO_LENGTH       (sizeof(server_config_t) / sizeof(uint8_t))
/*上传时间间隔配置首地址及长度*/
#define UPLOAD_DELAY_TIME_INFO_ADDR     (SERVER_CONFIG_INFO_ADDR + SERVER_CONFIG_INFO_LENGTH)
#define UPLOAD_DELAY_TIME_INFO_LENGTH   (sizeof(upload_delay_time_t) / sizeof(uint8_t))
//用户自定义通用头文件
//#define NDEBUG


#endif


