#include "esp8266wifi.h"
#include "uartConfig.h"
#include "util.h"
#include "stdio.h"

#define Esp8266WiFi_UART eUart1

//wifi status
static WiFiStatus_t sWifStatus = Disconnected;
static u8 sJoinRouterOK = 0;
//

/*************************wifi cmd code****************************/
static u8 CMD_SetWifiSTAMode[]="AT+CWMODE=1";//return OK
static u8 CMD_Reset[]="AT+RST";//return OK
static u8 CMD_JoinRouter[] = "AT+CWJAP=\"whz\",\"1234567890\"";//return OK
static u8 CMD_TcpConnect[] = "AT+CIPSTART=\"TCP\",\"";
static u8 CMD_SetTouChuan[]="AT+CIPMODE=1";
static u8 CMD_StartTransmit[]="AT+CIPSEND";
static u8 CMD_StopTouChuan[]="+++";
static u8 CMD_SmartConfig[] = "AT+CWSMARTSTART=1";
static u8 strConnect2Server[] = "AT+CIPSTART=\"TCP\",\"192.168.1.101\",8989";//return OK


/**********esp8266wifi control*************/
u8 esp8266Config(void)
{
	u8 cnt=0;
	stopTouChuan();
#if 0
	//1.set wifi mode
	esp8266WiFi_WriteLine(CMD_SetWifiSTAMode);
	delay_ms(200);
	//2.restart to be valid
	esp8266WiFi_WriteLine(CMD_Reset);
	delay_ms(1000);
#endif

	//3.connect to router
	esp8266WiFi_WriteLine(CMD_JoinRouter);
	do{
		delay_ms(1000);
		if(0 != isResponseOK())
			break;
		if(cnt == 10)
			esp8266WiFi_WriteLine(CMD_JoinRouter);
		cnt++;
	}while(cnt<20);
	if(cnt == 10)
		return 0;
	cnt = 0;
	dprintfln("join router success");
	//4.connect to server
	esp8266WiFi_WriteLine(strConnect2Server);
	do{
		delay_ms(200);
		if(0 != isResponseOK())
			break;
		cnt++;
	}while(cnt<10);
	if(cnt == 10)
		return 0;
	cnt = 0;
	dprintfln("connect server success");
	//
	esp8266WiFi_WriteLine(CMD_SetTouChuan);
	do{
		delay_ms(200);
		if(0 != isResponseOK())
			break;
		cnt++;
	}while(cnt<10);
	if(cnt == 10)
		return 0;
	cnt = 0;
	dprintfln("set passthrough success");
	//
	esp8266WiFi_WriteLine(CMD_StartTransmit);
	do{
		delay_ms(200);
		if(0 != isResponseOK())
			break;
		cnt++;
	}while(cnt<10);
	if(cnt == 10)
		return 0;
	cnt = 0;
	dprintfln("start transmit");
	sWifStatus = Connected;

	return 1;
}

void setWiFiSTAMode(void){
    esp8266WiFi_WriteLine(CMD_SetWifiSTAMode);
    delay_ms(100);
}


void setTouChuan(void){
    esp8266WiFi_WriteLine(CMD_SetTouChuan);
}

void stopTouChuan(void){
	esp8266WiFi_Write(CMD_StopTouChuan);
	delay_ms(100);
}

void startTransmit(void){
    esp8266WiFi_WriteLine(CMD_StartTransmit);
}

void esp8266_reset(void){
    stopTouChuan();
    setWiFiSTAMode();
    esp8266WiFi_WriteLine(CMD_Reset);
    delay_s(2);
}


WiFiStatus_t getWiFiStatus(void){
	return sWifStatus;
}

void setWifiOK(void){
    sWifStatus = Connected;
}

void setWifiFailed(void){
    sWifStatus = Disconnected;
}

WiFiStatus_t isWifiOK(void){
    return sWifStatus;
}


static u8 bResponseOK=0;
u8 isResponseOK(void){
	if(bResponseOK == 1){
		bResponseOK = 0;
		return 1;
	}
	return 0;
}

void oneStepTouChuan(char*ip,u16 PORT){
    u8 mode = 0;
    const u8 cnt = 10;
    u8 i=0;
    //delay a while for 8266 power on
    Delay_ms(2000);
    if(mode == 1){//join the router
        
    }
    BeginConnect:
    //connect and into TouChuan
    //connect to server
    i = 0;
    do{
        esp8266WiFi_TcpConnect(ip,PORT);
        Delay_ms(200+i*50);
        i++;
    }while(!isResponseOK() || i==cnt);
    if(i == cnt){
        goto BeginConnect;
        dprintfln("connected faile.retry");
    }
    //set TouChuan
    i=0;
    do{
        setTouChuan();
        Delay_ms(100+i*50);
        i++;
    }while(!isResponseOK() || i==cnt);
    if(i == cnt){
        goto BeginConnect;
    }
    //start TouChuan
    i=0;
    do{
        startTransmit();
        Delay_ms(100+i*50);
    }while(!isResponseOK() || i==cnt);
    if(i == cnt){
        goto BeginConnect;
    }
	setWifiOK();//set wifi OK
}


//set sJoinRouterOK state,0--failed,1--success
static void setJoinRouterStatus(u8 state){
    sJoinRouterOK = state;
}

u8 isJoinRouterOK(void){
    u8 result = 0;
    result = sJoinRouterOK;
    setJoinRouterStatus(0);
    return result;
}

void esp8266_SmartConfig(void){
    esp8266WiFi_WriteLine(CMD_SmartConfig);
    //check if success
}


/**********esp8266wifi send and receive***********/
u8 esp8266WiFi_TcpConnect(char* ip,u16 port){
    //result code
    u8 result = 0;
    char sPort[10];
    //check
    if(0 == ip)
        return result;
    //get string of u16's port
    sprintf(sPort,"%d",port);
    //connect
    esp8266WiFi_Write(CMD_TcpConnect);
    esp8266WiFi_Write(ip);
    esp8266WiFi_WriteData("\",",2);
    esp8266WiFi_WriteLine(sPort);
    //check if connect success
    return result;
}


void esp8266WiFi_WriteData(u8* data,u8 length){
	u8 i=0;
    //check
    if(0 == data)
        return;
	while(i<length){
		USARTSendData(data[i++],Esp8266WiFi_UART);
	}
}

void esp8266WiFi_Write(u8* str){
	if(0 == str)
		return;
	while(*str){
		USARTSendData(*str++,Esp8266WiFi_UART);
	}
}


void esp8266WiFi_WriteLine(u8* str){
	u8 newline[]="\r\n";
    //check
    if(0 == str)
        return;
	esp8266WiFi_Write(str);
	esp8266WiFi_Write(newline);
}


/***************HooK operation*****************/
//check if the data end with "OK"
void HookOfEsp8266WiFi(u8 data){
    static u8 mOKState = 0;
    static u8 mGotState = 0;
/***************"OK" flag*******************/
    switch(data){
        case 'O':
            mOKState = 1;break;
        case 'K':
            if(1 == mOKState)
                mOKState = 2;
            else
                mOKState = 0;
            break;
        default:
            mOKState = 0;break;
    }
    if(2 == mOKState)
        bResponseOK = 1;
/****************"GOT" flag******************/
    switch(data){
        case 'G':
            mGotState = 1;break;
        case 'O':
            if(1 == mGotState)
                mGotState = 2;
            else
                mGotState = 0;
            break;
        case 'T':
            if(2 == mGotState)
                mGotState = 3;
            else
                mGotState = 0;
            break;
        default:
            mGotState = 0;break;
    }
    if(3 == mGotState)
        setJoinRouterStatus(1);
}

