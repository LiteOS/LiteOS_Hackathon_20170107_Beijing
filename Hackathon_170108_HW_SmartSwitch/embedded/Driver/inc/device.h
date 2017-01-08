/*
**************************
file info:
	author:fang.junpeng
	start time:2017-01-01
	device driver
***************************
*/
#include "pm25.h"
#include "timerDriver.h"
#include "uartConfig.h"
#include "bsp_dht11.h"
#include "led.h"
#include "key.h"
#include "esp8266wifi.h"

#ifndef DEVICE_H
#define DEVICE_H

void device_init(void);

#endif

