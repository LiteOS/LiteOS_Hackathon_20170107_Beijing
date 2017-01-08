/*
**************************
file info:
	author:fang.junpeng
	start time:2016-01-05
	server action
***************************
*/

#include "baseTypeDef.h" 

#ifndef SERVERACTION_H
#define SERVERACTION_H


/*****server buffer operation*****/
void ServerBuf_init(void);

//get the data buf 
u8* getServerBuf(void);

//get the data buf and disable the buffer
u8* getServerBufDisable(void);

u8 isServerBufferEabled(void);

void ServerBuffer_clear(void);

u8 ServerBuf_dataLength(void);

/*******client-server bussiness*******/
typedef enum{
    CMD_close = 0,CMD_open,CMD_common,CMD_timer,CMD_unknow
}ServerMsg_t;

ServerMsg_t getServerMsg(void);

typedef struct{
    char *pm25,*tempreture,*humidity,*SwitchStatus;
}Sensor_Info_t;

void SensorInfo_addPm25(char*);

void SensorInfo_addTemp(char*);

void SensorInfo_addHumi(char*);

void SensorInfo_addSwStatus(char*);

void setSensorInfoOK(void);

u8 isSersorInfoOK(void);

Sensor_Info_t* getSensorInfo(void);
//when Timer cmd,get the mintues
u8 getTimerMinutes(void);


/*
Msg example:
"hdu123&pm25:500,tempreture:17,humidity:47,switch:open\r\n"
*/
void sendDeviceMsg(Sensor_Info_t*);

/********************HooK operation************************/
void HookOfServerAction(u8 data);

#endif



