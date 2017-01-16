/*
**************************
file info:
	author:fang.junpeng
	start time:2017-01-01
	relay driver
***************************
*/
#ifndef RELAY_H
#define RELAY_H

void relay_init(void);

void relay_on(void);

void relay_off(void);

/*
reutrn:1---ON,0---OFF
*/
unsigned char relay_status(void);

#endif
