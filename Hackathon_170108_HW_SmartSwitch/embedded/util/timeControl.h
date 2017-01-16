/*
**************************
file info:
	author:fang.junpeng
	start time:2016-05-08
	email:tfzsll@126.com
	usartConfig 
***************************
*/

#include "baseTypeDef.h" 

#ifndef TIMECONTROL_H
#define TIMECONTROL_H

typedef struct{
	u16 section,end;
}Timer_t;

enum TimerEventEnum{
	eTimerEventOnce = 1,eTimerEventRepeat = 3
};

typedef struct{
	Timer_t*thisTimer;
	void(*thisFunc)(void);
	enum TimerEventEnum originState,newState;
}TimerEvent_t;

#define TELIST_LENGTH 10
typedef struct{
	TimerEvent_t* Events[TELIST_LENGTH];
	u8 count,bStopTimerEventsList;//bStopTimerEventsList:0-false,1-true
}TimerEventsList_t;


void setTimerOutFlag(Timer_t* timer,u16 section);

u16 getSystemTime(void);
u16 getMaxTimerCount(void);


void initTimerWithSeciton(Timer_t*timer,u16 section);

void updateTimer(Timer_t* timer);

//timer Event
void initTimerEvent(TimerEvent_t*TE,Timer_t* timer,void(*fun)(void),enum TimerEventEnum state);
void doTimerEvent(TimerEvent_t* TE);
void reActiveTimerEvent(TimerEvent_t* TE);

// for system app
void doTimerEventsList(TimerEventsList_t*TE_list);
void stopTimerEventsList(TimerEventsList_t*TE_list);
void startTimerEventsList(TimerEventsList_t*TE_list);

//this is for system,not for user
void setSystemTime(u16 time);

u8 addToTimerEventsList(TimerEventsList_t*TE_list,TimerEvent_t* TE);


void deleteFromTimerEventsList(TimerEventsList_t*TE_list,TimerEvent_t* TE);


//set timer Hook
void setTimerHook(void);

void reSetTimerHook(void);


#endif


