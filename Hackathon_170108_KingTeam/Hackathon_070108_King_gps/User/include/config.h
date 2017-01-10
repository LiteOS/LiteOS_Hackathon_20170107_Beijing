#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stm32f10x.h"
#include "Types.h"
#include "network_conf.h"

#pragma pack(1)
//typedef struct _CONFIG_MSG
//{
//  uint8 op[4];//header: FIND;SETT;FACT...
//  uint8 mac[6];
//  uint8 sw_ver[2];
//  uint8 lip[4];
//  uint8 sub[4];
//  uint8 gw[4];
//  uint8 dns[4];	
//  uint8 dhcp;
//  uint8 debug;

//  uint16 fw_len;
//  uint8 state;
//  
//}CONFIG_MSG;

typedef struct _CONFIG_MSG
{
  uint8 op[4];//header: FIND;SETT;FACT...
  uint8 mac[6];
  uint8 sw_ver[2];
  //device name & serial number;
  uint8 device_name[16];
  uint8 device_serial[16];
  
  uint8 lip[4];
  uint8 sub[4];
  uint8 gw[4];
  uint8 dns[4];	
  uint8 dhcp;
  uint8 debug;
  uint16 fw_len;
  uint8 state;

  //password for web/telnet config
  uint8 pwd[16];//md5 digest
  
  //communication
  uint8 mode; //tcp client, tcp server, udp. 
  uint16 lport;
  uint8 rip[4]; //remote ip address
  uint16 rport; //remmote port number
  uint8 dns_flag;
  uint8 domain[32];
  
}CONFIG_MSG;
#pragma pack()

#define CONFIG_MSG_LEN        sizeof(CONFIG_MSG) - 4 // the 4 bytes OP will not save to EEPROM

#define MAX_BUF_SIZE		1460
#define KEEP_ALIVE_TIME	    6	// 10 *5sec

#define __GNUC__

/*---------------------------------------*/
/* dhcp.c                                */
/*---------------------------------------*/
extern CONFIG_MSG  ConfigMsg, RecvMsg;

#define SOCK_DHCP             0
#define SOCK_HTTP             1
#define SOCK_DNS              2
/*---------------------------------------*/
/*定义时，启用调试功能*/
//#define NDEBUG
#endif


