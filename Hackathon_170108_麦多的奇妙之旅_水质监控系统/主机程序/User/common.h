#ifndef __COMMON_H
#define __COMMON_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
// STM32硬件相关头文件
#include "stm32f10x.h"

#include "Delay.h"

#include "Bsp_GPRS.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "Bsp_Key.h"
#include "Bsp_Beep.h"
#include "Screen.h"
#include "Bsp_ChipID.h"
#include "Bsp_DS18B20.h"
#include "Bsp_PowerDown.h"
#include "Bsp_WaterPwmin_Tim3.h"
#include "Bsp_Water_Level.h"
#include "Bsp_PH.h"
#include "Bsp_Flash.h"
#include "Bsp_RTC.h"
#include "Slave_Control.h"
#include "Bsp_Uart2_Zgb.h"
#include "Bsp_Can.h"
#include "Bsp_Uart3_A6.h"
#include "SelfDatGet.h"
#include "Up2Server.h"

#define MAX_SlaveNum      (20)            //支持最大的从机数量
#define GetSensorTime     (2)             //采集自身传感器时间间隔（秒）
#define TouchSlaveTime    (10)            //主机问询从机数据(秒)
#define SlaveLostTime     (4)             //主机检测从机丢失时间（秒）
#define AlarmTime         (1)            //RTC最小中断时间 (分钟)
#define UpLoadingTime     (15)            //主机向服务器上传数据时间间隔  （UpLoadingTime x AlarmTime 秒）
#define UpWarmingTime     (1)             //上传警告间隔  （UpLoadingTime x AlarmTime 秒）
#define UpWarmingAgainCountTime (3)           //重复上传警告间隔
#define ClearWarmingUpTime (1)
#define SlaveLostCount     6

/* GPRS类 */
#define DefaultValue        0
#define NoResponse          11
#define NoSIMCard           12
#define GRPSGetFailue       13
#define PDPGetFailue        14
#define GetTCPFailue        15
#define ServeNoResponse     16
#define ErrorFromServe      17
#define ServerNoUser        18
#define GetTimeError        19
#define LocalFault          20

/* 系统初试时间设置 */
#define Yaer    2016
#define Moon    10
#define Day     19
#define Hour    20
#define Min     25
#define Sec     0

#define PH_Max              14
#define PH_Min              0
#define WaterTmp_Max        100
#define WaterTmp_Min        0
#define EnvirTemp_Max       100
#define EnvirTemp_Min       0
#define WaterSpeed_Max      14
#define WaterSpeed_Min      0

#define SUCCESS     1
#define FAILUED     0

#define USE_ZGB     1
#define USE_CAN     0

#define ON          1
#define OFF         0

extern uint8_t Connect_Way;
//extern uint8_t TCPIsUsed;

#endif
