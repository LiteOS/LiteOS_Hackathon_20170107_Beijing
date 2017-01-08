#include "serverAction.h"
#include "util.h"
#include "sysTypeDef.h"
#include "esp8266wifi.h"

//server buffer
static Buffer_t sServerBuf = {{0},0};

//client id
static u8 deviceID[]="hdu123";

//timer minutes
static u8 sTimerMinutes = 0;

//sensor info
static Sensor_Info_t sSensorInfo;
static u8 sSensorInfoOK = 0;

//client-server interaction command
static u8 OpenCmd[]="open";static const u8 OpenCmdLength = 4;
static u8 CloseCmd[]="close";static const u8 CloseCmdLength = 5;
static u8 TimerCmd[]="timer";static const u8 TimerCmdLength = 5;
static u8 ComResponse[]="%%%";static const u8 ComResLength = 3;


/*****server buffer operation*****/
void ServerBuf_init(void){
    Buffer_init(&sServerBuf);
}

u8* getServerBuf(void){
    return getBuffer(&sServerBuf);
}

u8 isServerBufferEabled(void){
    return isBufferEabled(&sServerBuf);
}

void ServerBuffer_clear(void){
    Buffer_clear(&sServerBuf);
}

u8 ServerBuf_dataLength(void){
    return Buffer_dataLength(&sServerBuf);
}

/**********sensor info************/
void SensorInfo_addPm25(char*pm25){
    sSensorInfo.pm25 = pm25;
}

void SensorInfo_addTemp(char*temp){
    sSensorInfo.tempreture = temp;
}

void SensorInfo_addHumi(char*humi){
    sSensorInfo.humidity = humi;
}

void SensorInfo_addSwStatus(char*Status){
    sSensorInfo.SwitchStatus = Status;
}

u8 isSersorInfoOK(void){
    return sSensorInfoOK;
}

void setSensorInfoOK(void){
    sSensorInfoOK = 1;
}

Sensor_Info_t* getSensorInfo(void){
    if(0 == isSersorInfoOK())
        return 0;
    //disable the sensorInfo
    sSensorInfoOK = 0;
    return &sSensorInfo;
}

//when Timer cmd,get the mintues
u8 getTimerMinutes(void){
    u8 result;
    result = sTimerMinutes;
    sTimerMinutes = 0;
    return result;
}

//it shoul be call by getServerMsg,when "timer" cmd,set sTimerMinutes
static void getMinutesFromMsg(void){
    u8* buf = getServerBuf();
    if(TimerCmdLength+1 == ServerBuf_dataLength()){
        sTimerMinutes = buf[TimerCmdLength]-'0';
    }
    else if(TimerCmdLength+2 == ServerBuf_dataLength()){
        sTimerMinutes = (buf[TimerCmdLength]-'0')*10;
        sTimerMinutes += (buf[TimerCmdLength+1]-'0');
    }
}

/*******client-server bussiness*******/
ServerMsg_t getServerMsg(void){
    ServerMsg_t MsgType = CMD_unknow;
    switch(ServerBuf_dataLength()){
        case OpenCmdLength:
            if(byteCompare(getServerBuf(),OpenCmd,OpenCmdLength))
                MsgType = CMD_open;     
            ServerBuffer_clear();
            break;
        case CloseCmdLength:
            if(byteCompare(getServerBuf(),CloseCmd,CloseCmdLength))
                MsgType = CMD_close;
            else if(byteCompare(getServerBuf(),TimerCmd,TimerCmdLength)){
                MsgType = CMD_timer;//set msg timer
                //get Minutes
                getMinutesFromMsg();
            }
            ServerBuffer_clear();
            break;
        case ComResLength:
            if(byteCompare(getServerBuf(),ComResponse,ComResLength))
                MsgType = CMD_close;
            break;
        default:
            break; 
    }
    return MsgType;
}



static void sendSensorInfo(Sensor_Info_t*sensorInfo){
    //check
    if(0 == sensorInfo)
        return;
    //send
    esp8266WiFi_Write("pm=");
    esp8266WiFi_Write(sensorInfo->pm25);
    esp8266WiFi_Write("&humidity=");
    esp8266WiFi_Write(sensorInfo->humidity);
    esp8266WiFi_Write("&tempreture=");
    esp8266WiFi_Write(sensorInfo->tempreture);
    esp8266WiFi_Write("&status=");
    esp8266WiFi_WriteLine(sensorInfo->SwitchStatus);
}

static char* sDeviceId = "123";
static void sendDeviceID(){
    esp8266WiFi_Write("deviceID=");
    esp8266WiFi_Write(sDeviceId);
}

void sendDeviceMsg(Sensor_Info_t*sensorInfo){
    //check
    if(0 == sensorInfo)
        return;
    sendDeviceID();
    esp8266WiFi_Write("&");
    sendSensorInfo(sensorInfo);
}


/*
the server data is like "@xxxxx$"
we should store the content "xxxxx" between '@' and '$'
*/
void HookOfServerAction(u8 data){
    static u8 mState = 0;
    switch(data){
        case '@':
            mState = 1;break;
        case '$':
            mState = 2;
            Buffer_enable(&sServerBuf);
            break;
        default:
            //save the data into buffer
            if(1 == mState && '@' != data){
                Buffer_add(&sServerBuf,data);
            }
    }
}



