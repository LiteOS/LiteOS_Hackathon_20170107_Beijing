#ifndef MSG_H
#define MSG_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stm32f10x.h"

#define OK                        (0)
#define ERROR                     (-1)

#define DISP_INFO           0x01
#define DISP_TEST           0x00000002

#define PKG_START						0x0

typedef struct
{
    uint32_t DataAddr;
    uint32_t length;
    uint32_t MsgType;
}MsgStr_t;

typedef struct
{
	uint16_t sStayTime;
	char cProductrID[16];
	unsigned char ucPkgNum;
	unsigned char ucIsEnd;
}Msg_SendInfoBuf;
#endif
