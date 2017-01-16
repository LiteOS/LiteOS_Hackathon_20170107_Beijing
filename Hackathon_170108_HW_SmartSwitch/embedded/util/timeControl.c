#include "timeControl.h"
#include "sysTypeDef.h"

#include "stm32f10x.h"

const u16 MAX_TimerCnt=50000;

volatile u16 TimeTickMs = 0;// ms counter

VOIDHOOK_t timerHook =0;

//#define TELIST_LENGTH 10

TimerEventsList_t TimerEventList={{0},0};

void setTimerOutFlag(Timer_t* timer,u16 section){
	timer->section = section;
	updateTimer(timer);
}

u16 getSystemTime(void){
	return TimeTickMs;
}

u16 getMaxTimerCount(void){
	return MAX_TimerCnt;
}

void initTimerWithSeciton(Timer_t*timer,u16 section){
	timer->section = section;
	if(timer->section >(MAX_TimerCnt-getSystemTime()))
		timer->end = timer->section - (MAX_TimerCnt-getSystemTime());
	else
		timer->end = timer->section + getSystemTime();
}

void updateTimer(Timer_t* timer){
	if(timer->section>(MAX_TimerCnt- timer->end))
		timer->end = timer->section-(MAX_TimerCnt - timer->end);
	else
		timer->end+=timer->section;
}

void initTimerEvent(TimerEvent_t*TE,Timer_t* timer,void(*fun)(void),enum TimerEventEnum state){
	if(0 == TE||0==timer||0==fun)
		return;
	TE->thisTimer = timer;
	TE->thisFunc = fun;
	TE->originState = TE->newState = state;
	//init timer
	initTimerWithSeciton(timer,timer->section);
}
void doTimerEvent(TimerEvent_t* TE){
	if(0==TE)
		return;
	if(TE->originState == eTimerEventRepeat){
		if(TE->thisTimer->end == getSystemTime()){//judge if the time is ok
			(TE->thisFunc)();
			updateTimer(TE->thisTimer);
		}
		return;
	}
	else if(TE->originState != TE->newState)
		return;
	if(0 != TE->thisFunc)
	    (TE->thisFunc)();
	TE->newState = TE->originState-1;
	return;
}
void reActiveTimerEvent(TimerEvent_t* TE){
	TE->newState = TE->originState;
}

//for timer event list
void doTimerEventsList(TimerEventsList_t*TE_list){
	u8 i=0,cnt=0;
	if(0 == TE_list)
		return;
	if(TE_list->bStopTimerEventsList !=0)
		return;
	while(i<TELIST_LENGTH && cnt <TE_list->count){
		if(0 != TE_list->Events[i]){
			doTimerEvent(TE_list->Events[i]);
			cnt++;
		}
		i++;//self plus
	}
}

void stopTimerEventList(TimerEventsList_t*TE_list){
	if(0 == TE_list)
		return;
	TE_list->bStopTimerEventsList = 1;//make bStopTimerEventsList true
}

void startTimerEventList(TimerEventsList_t*TE_list){
	if(0 == TE_list)
		return;
	TE_list->bStopTimerEventsList = 0;//make bStopTimerEventsList false
	doTimerEventsList(TE_list);
}


u8 addToTimerEventsList(TimerEventsList_t*TE_list,TimerEvent_t* TE){
	u8 i = TE_list->count,j=0;
	if( TE == 0 || 0== TE_list)
		return 0;
	if(TE_list->count < TELIST_LENGTH){
		while(j<TELIST_LENGTH){
			i=(i+(j++))%TELIST_LENGTH;
			if(TE_list->Events[i] == 0){
				TE_list->Events[i] = TE;
				TE_list->count++;
				return 1;
			}
		}
	}
	else
		return 0;
	return 0;
}

u8 TimerEventCompared(TimerEvent_t*TE1,TimerEvent_t* TE2){
	if(TE1->originState != TE2->originState || TE1->thisFunc != TE2->thisFunc\
		|| TE1->thisTimer != TE2->thisTimer)
		return 1;
	return 0;
}

void deleteFromTimerEventsList(TimerEventsList_t*TE_list,TimerEvent_t* TE){
	u8 i=0;
	if(TE == 0 || 0 == TE_list)
		return;
	while(i<TELIST_LENGTH){
		if(TE_list->Events[i]!=0){
			if(0 == TimerEventCompared(TE_list->Events[i],TE)){
				TE_list->Events[i] = 0;
				TE_list->count--;
			}
		}
	}
}


void setTimerHook(void){}

void reSetTimerHook(void){}


