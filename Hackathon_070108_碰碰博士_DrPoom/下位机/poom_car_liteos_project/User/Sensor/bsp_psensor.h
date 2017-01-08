#ifndef __PSENSOR_H
#define __PSENSOR_H

#include "stm32f10x.h"

void PSensor_Init (void);
uint8_t Get_Sensor_Stat (uint8_t id);

#endif
