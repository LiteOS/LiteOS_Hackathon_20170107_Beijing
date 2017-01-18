#ifndef __Bsp_GPRS_H
#define __Bsp_GPRS_H
#include "common.h"

#define GPIO_Pin_A6_Rst          GPIO_Pin_9
#define GPIO_Pin_A6_PWR          GPIO_Pin_11

#define GPIOx_A6_Rst             GPIOB
#define GPIOx_A6_PWR             GPIOC

#define RCC_APB2Periph_GPIOx_A6_Rst    RCC_APB2Periph_GPIOB
#define RCC_APB2Periph_GPIOx_A6_PWR    RCC_APB2Periph_GPIOC

#define A6_Rst(a)	if (!a)	\
					GPIO_SetBits(GPIOx_A6_Rst,GPIO_Pin_A6_Rst);\
					else		\
					GPIO_ResetBits(GPIOx_A6_Rst,GPIO_Pin_A6_Rst)

#define A6_PWR(a)	if (!a)	\
					GPIO_SetBits(GPIOx_A6_PWR,GPIO_Pin_A6_PWR);\
					else		\
					GPIO_ResetBits(GPIOx_A6_PWR,GPIO_Pin_A6_PWR)

#define ERR_COUNT               20
#define DEVICE_NOFOUND          0x10
#define GPRS_REGISTER_FAILUED   0x11
#define GPRS_NO_SIM             0x12
#define GPRS_NO_NET             0x13

#define OffsetNoSet             0
#define OffSetGet               1
          
extern uint8_t TCPIsUsed;
extern uint8_t WarmUpCount[20 + 1][6];
extern uint16_t WarmClearStatue;
          
void GPRS_GPIO_Config(void);
uint8_t GPRS_Check (void);
void GPRS_Rest (void);
uint8_t GRPS_NetInit (void);
uint8_t UpLoadDat_TCP (void);
uint8_t UpWarmingDat_TCP (void);
uint8_t UpWarmingDatAgain_TCP (void);
  
uint8_t DevBound (void);          
uint8_t ReqSendTcp (uint8_t length);              //请求发送TCP数据
uint8_t AddSlaveDev2Server (uint8_t FlashPos, uint8_t* SlaveChipIDString);    //在服务器上添加从机
uint8_t SendDatTCP2Server_Ready (uint8_t MsgLength);           //发送数据包前的起始包
uint8_t SendDat2Server (uint8_t* MsgSendBuf);     //发送数据包到服务器
uint8_t AddSlaveID2Server (long SlaveID, uint8_t SlaveSerial);
uint8_t UpOneSlaveDat2Server (uint8_t SlaveNum);

void GPRS_ReceMsgProcess (void);
uint8_t UpLostMsg (uint8_t* SlaveID);
uint8_t UpSlaveLost2Server (uint8_t* SlaveChipIDString);
uint8_t SendMsg2Server (uint8_t* SendBuf, char* CheckBuf);
uint8_t UpOffsetDat (uint8_t Index, float Offset);
uint8_t GetOffsetDat (void);
void StrCpy (char* Src, char* Dest, int Length);

#endif
