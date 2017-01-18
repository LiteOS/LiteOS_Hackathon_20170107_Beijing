#include "common.h"

uint8_t Connect_Way = USE_CAN;                      //通讯方式标志位 USE_CAN   USE_ZGB


void TestValSet (void)
{
  /*
   *  水温：      上限：30.0    下限：15.0
   *  板温：      上限：30.0    下限：15.0
   *  水流量：    上限：20L/s   下限：0L/s
   *  PH：        上限：8.0     下限：6.0
   */
  
  /* *******水温阈值警报测试用******** */
  Flash_FifoBuf[1] &= ~(0xff << 8);
  Flash_FifoBuf[1] |= ((30 & 0xff) << 8);   //上限设置        
  Flash_FifoBuf[1] &= ~(0xf << 4);
  Flash_FifoBuf[1] |= ((0 & 0xf) << 4);
  
  Flash_FifoBuf[1] &= ~(0xf);
  Flash_FifoBuf[1] |= ((15 >> 4)& 0xf);   //下限设置
  Flash_FifoBuf[2] &= ~(0xff << 24);
  Flash_FifoBuf[2] |= ((15 & 0xf) << 28);
  Flash_FifoBuf[2] |= ((0 & 0xf) << 24);
  /* ********************************* */
  
  /* *******板温阈值警报测试用******** */
  Flash_FifoBuf[2] &= ~(0xff << 16);
  Flash_FifoBuf[2] |= ((30 & 0xff) << 16);         //上限设置
  Flash_FifoBuf[2] &= ~(0xf << 12);
  Flash_FifoBuf[2] |= ((0 & 0xf) << 12);
  
  Flash_FifoBuf[2] &= ~(0xff << 4);
  Flash_FifoBuf[2] |= ((15 & 0xff) << 4);         //下限设置
  Flash_FifoBuf[2] &= ~(0xf << 0);
  Flash_FifoBuf[2] |= ((0 & 0xf) << 0);
  /* ********************************* */
  
  /* *******水流阈值警报测试用******** */
  Flash_FifoBuf[3] &= ~(0xff << 24);
  Flash_FifoBuf[3] |= ((20 & 0xff) << 24);   //上限设置
  Flash_FifoBuf[3] &= ~(0xf << 20);
  Flash_FifoBuf[3] |= ((0 & 0xf) << 20);
  
  Flash_FifoBuf[3] &= ~(0xff << 12);
  Flash_FifoBuf[3] |= ((0 & 0xff) << 12);   //下限
  Flash_FifoBuf[3] &= ~(0xf << 8);
  Flash_FifoBuf[3] |= ((0 & 0xf) << 8);
  /* **************************** */
  
  /* **********PH警报测试用*********** */ 
  Flash_FifoBuf[1] &= ~(0xffff << 16);
  Flash_FifoBuf[1] |= ((8 & 0xf) << 28);    //上限
  Flash_FifoBuf[1] |= ((0 & 0xf) << 24);
  Flash_FifoBuf[1] |= ((6 & 0xf) << 20);    //下限
  Flash_FifoBuf[1] |= ((0 & 0xf) << 16);  
  /* ********************************* */
//  Flash_ReadPageToBUF();
  
  /* ******主从机组网测试用****** */
//  Flash_FifoBuf[0] = 1;
////////  
//  Flash_FifoBuf[7] = 0x67065514;
//  Flash_FifoBuf[8] = 0x01000114;    //模拟已配对
//  Flash_FifoBuf[9] = 0x11600020;
//  
//  Flash_FifoBuf[10] = 0;        
//  Flash_FifoBuf[11] = 0;   //模拟未配对，服务器已回应
//  Flash_FifoBuf[12] = 0;
  /* ************** */
  
  /* 测试所用代码段 */
  //Flash_FifoBuf[0] = 0;  
//  Flash_WriteBufToPage();
//  Flash_WriteOfficDat();
  /* ************** */

//  Flash_ReadPageToBUF();
//  Flash_ReadOfficDat();
//  Flash_WriteBufToPage();
  //printf ("wait for slave\r\n");
}


void hardware_init(void)
{
  Delay_ms(2000);               //显示屏上电需要等待一会 
  GPRS_GPIO_Config();           //A6模块开机，复位键置位  
  Key_Init();
  ReadTouchWay();               //确定通信方式，一开始就确定 
  GPRS_GPIO_Config();           //A6模块开机，复位键置位  
  Beep_Init();                  //蜂鸣器初始化
  LED_GPIO_Config();            //LED初始化，指示灯
  USART_Config();               //串口1初始化，用于屏幕
  USART3_Config();
  Scr_SendCmd("page Home");     //切换到首页
  ProBarGo(15);                 //初始界面进度条走15%
  GetChipID();                  //获取芯片ID
  LED_Green(ON);                //LED灯亮提示TCP正被占用
  GPRS_Rest();                  //A6_GPRS联网初始化，尝试连接服务器，如果失败就不断连接
  LED_Green(OFF);               //TCP使用完成
  DS18B20_Init(DS18B20IN);      //板载的温度传感器初始化
  DS18B20_Init(DS18B20EX);      //水温的温度传感器初始化
  PowDon_Init();                //掉电检测初始化
  WaterSpeedIn_Init();          //水流测速初始化
  Water_Level_Init();
  AD_Init();                    //PH_AD初始化
  TestValSet();                 //测试时在Flash中写入一些值，成品中用于写入阈值
  if (Connect_Way == USE_ZGB)   //CAN普通模式初始化, 波特率500Kbps 
  {
    USART2_Config();
    Beep_ON_Nop(100);           //蜂鸣器一声提示
  }
  else
  {
    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,0); 
    Beep_ON_Nop(100);           //蜂鸣器两声提示
    Beep_ON_Nop(100);
  }
//  Beep_ON_Nop(100);             //蜂鸣器两声，提示底层设备初始化完毕
//  Beep_ON_Nop(100);
  GotoDisDatPage();             //跳转到显示界面
  RTC_Init();
  Beep_ON_Nop(300);             //提示开始正常工作
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
  
	hardware_init();
  
  
  uwRet = creat_GetSelfDatTask();     //采集自身传感器数据
	if(uwRet !=LOS_OK)
		return uwRet;
  
  uwRet = creat_KeyScanTask();        //按键扫描
	if(uwRet !=LOS_OK)
		return uwRet;
  
  uwRet = creat_GetSlaveDatTask();    //采集从机传感器数据
	if(uwRet !=LOS_OK)
		return uwRet;
  
  uwRet = creat_UpDat2ServerTask();   //上传数据到服务器  包括采集数据、报警、重复报警
	if(uwRet !=LOS_OK)
		return uwRet;
  
	return LOS_OK;
}
/*********************************************END OF FILE**********************/
