/*
**************************
file info:
	author:fang.junpeng
	start time:2017-01-01
	pm2.5 driver
***************************
*/
#include "baseTypeDef.h" 

#ifndef PM25_H
#define PM25_H

void PM25_init(void);

u8 getPM25Index(float*);

void PM25HOOK(u8);

#endif

