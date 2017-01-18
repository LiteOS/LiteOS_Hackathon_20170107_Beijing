#ifndef __COMMON_H_
#define __COMMON_H_


/* Includes ------------------------------------------------------------------*/
// Huawei LiteOS头文件
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

#include "bsp_led.h"
#include "bsp_usart.h"
#include "Bsp_DS18B20.h"
#include "Bsp_WaterPwmin_Tim3.h"
#include "Bsp_PH.h"
#include "Bsp_Water_Level.h"
#include "Bsp_ADC.h"
#include "Bsp_Flash.h"
#include "Bsp_wdg.h"
#include "SelfDatGet.h"
#include "bsp_usart.h"
#include "bsp_usart2.h"
#include "Bsp_ChipID.h"
#include "Bsp_Key.h"
#include "Bsp_Can.h"

#include "Touch2Master.h"

#define GetSensorTime     (1)             //采集自身传感器时间间隔（秒）
#define GetNetAgainTime   (1)             //多久检测一次是否掉线了
#define GetNetStatue      (1)

#define USE_ZGB     1
#define USE_CAN     0

#define Net_NoNet         2
#define Net_HasBeenAsked  3
#define Net_GetSuccess    4
#define Net_Refused       5

#define SUCCESS           0
#define FAILED            1


extern uint8_t GetNet_Statue;
extern uint8_t GetMasterNet_Statue;

#define CheckLostTime     (5)       //单位：分钟

#endif

