#include "common.h"

uint8_t GetNet_Statue = Net_NoNet;    //判断是否入网标志位
uint8_t GetMasterNet_Statue = CheckLostTime;

void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);


UINT32 g_uwTskHiID;
UINT32 g_uwTskLoID;

void Delay(__IO uint32_t nCount) //简单延时
{
	for(; nCount != 0; nCount--);
}

void hardware_init(void)
{ 
  u16 Rand = 0;
  
  LED_GPIO_Config();          //LED引脚初始化
  ReadTouchWay();             //读取通讯方式
  Delay_ms(2000);             //上电延时一会
  USART_Config();             //DebugUart初始化
  GetChipID();                //获取芯片ID
  WaterSpeedIn_Init();        //水流测速初始化
  Water_Level_Init();         //水位检测初始化
  DS18B20_Init(DS18B20IN);    //室温初始化
  DS18B20_Init(DS18B20EX);    //水温初始化
  AD_Init();                  //PHAD初始化
  Adc_Init();		  		        //ADC初始化
  FlashInit();                //用于存储需要掉电保存的信息
  
  if (Connect_Way == USE_CAN) //CAN普通模式初始化, 波特率500Kbps 
    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,0); //CAN普通模式初始化, 波特率500Kbps 
  else
    USART2_Config();            //ZGB初始化
  
  while(GetNet_Statue == Net_NoNet)
  {
    Rand = GetRand();
    if (Connect_Way == USE_ZGB)
      Delay_ms(Rand);           //延时是为ZGB考虑的，ZGB通讯速率较慢
    
    if (Connect_Way == USE_CAN)
      Can_ReqGetNet();
    else
      Zgb_ReqGetNet();
    
    if (Connect_Way == USE_ZGB)
      Delay_ms(5000);           //延时是为ZGB考虑的，ZGB通讯速率较慢
    
    switch(GetNet_Statue)
    {
      case Net_Refused:       //如果提示主机拒绝了入网申请
          while(1);           //死循环
      case Net_HasBeenAsked : //如果提示已经发送申请等待服务器确认
      default:                //如果一直未收到主机的信息或是其他错误数据
        break;
    }
  }
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	hardware_init();

	LED_Green(ON);
	LED_Blue(OFF);
  
  uwRet = creat_GetSelfDatTask();     //采集自身传感器数据
	if(uwRet !=LOS_OK)
		return uwRet;

  uwRet = creat_GetNetAgainTask();    //检测是否掉线，定期重连
  if(uwRet !=LOS_OK)
		return uwRet;
  
  uwRet = creat_SendAgainTask();    //检测是否掉线，定期重连
  if(uwRet !=LOS_OK)
		return uwRet;
  
	return LOS_OK;
}
/*********************************************END OF FILE**********************/
