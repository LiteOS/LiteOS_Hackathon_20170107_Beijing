#include "common.h"

UINT32 g_TestTskHandle;

VOID task_GetSelfDat (void)
{
  UINT32 uwRet = LOS_OK;
  
  /* *************板温相关************** */
  float EnvirTemp = 0.0;  //板温
  uint8_t EnvirTempInt = 0, EnvirTempFloat = 0;   //将温度拆分成整数和小数两部分
  /* *********************************** */
  
  /* *************水温相关************** */
  float WaterTemp = 0.0;  //水温
  uint8_t WaterTempInt = 0, WaterTempFloat = 0;   //将温度拆分成整数和小数两部分
  /* *********************************** */
  
  /* **************PH相关*************** */
  float PH = 0;
  uint16_t PH_Original = 0;
  float PHMax = 0.0, PHMin = 0.0;
  uint8_t PHInt = 0, PHFloat = 0;
  /* *********************************** */
  
  /* ******************水速相关****************** */
  float WaterSpeed = 0.0;       //水速
  int WaterSpeedInt = 0, WaterSpeedFloat = 0;   //水速的整数部分和小数部分
  /* ******************************************** */
    
   while(1)
   {
      /* *****************PH获取代码*************** */
    PH_Original = GetPH();
    
    if (PH_Original< 32750)   //碱性
    {
      PH = 6.86 - (PH_Original * (OfficDatBuf[4] / 1000000.0));
    }
    else                      //酸性
    {
      PH = 6.86 + (PH_Original * (OfficDatBuf[3] / 1000000.0));
    }
    
    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
    PHInt = (int)PH;
    PHFloat = ((int)(PH * 10) % 10);
    
    if ((PH > PHMax) || (PH < PHMin))
    {
      Flash_FifoBuf[5] |= (1 << 16);        //PH警报置位
    }
    else
    {
      Flash_FifoBuf[5] &= ~(0x3 << 16);   //如果PH正常，清除警报和上报服务器标志位
    }
    Flash_FifoBuf[5] &= ~(0xff << 6);
    Flash_FifoBuf[5] |= ((((PHInt & 0xf) << 4) + (PHFloat & 0xf)) << 6);
  /* ****************************************** */
  
    
    /* ******************水温获取**************** */
    WaterTemp = DS18B20_Get_Temp(DS18B20EX);    //获取水温
    WaterTemp += (OfficDatBuf[0] / 10.0);
    
    WaterTempInt = (int)WaterTemp;
    WaterTempFloat = ((int)(WaterTemp * 10) % 10);
    
    Flash_FifoBuf[5] &= ~(0x3f);
    Flash_FifoBuf[5] |= ((WaterTempInt >> 2) & 0x3f);
    Flash_FifoBuf[6] &= ~(0x3f << 26);
    Flash_FifoBuf[6] |= ((WaterTempInt & 0x3) << 30);
    Flash_FifoBuf[6] |= ((WaterTempFloat & 0xf) << 26); 
    
//    printf ("the WaterTemp is:%f\r\n", WaterTemp);
      
    
    /* ******************板温获取*************** */
    EnvirTemp = DS18B20_Get_Temp(DS18B20IN);    //获取板温
    EnvirTempInt = (int)EnvirTemp;
    EnvirTempFloat = ((int)(EnvirTemp * 10) % 10);
    
    Flash_FifoBuf[6] &= ~(0x000000ff << 18);
    Flash_FifoBuf[6] |= ((EnvirTempInt & 0xff) << 18);
    Flash_FifoBuf[6] &= ~(0x0000000f << 14);
    Flash_FifoBuf[6] |= ((EnvirTempFloat & 0x0f) << 14);
  
//    printf ("the EnvirTemp is:%f\r\n", EnvirTemp);
    
    /* ****************** 泵速度获取 *************** */
    WaterSpeed = (TIM3->CNT / 450.0 / (float)GetSensorTime);    //水流速度，升为单位 一升水450个脉冲
    TIM3->CNT = 0;
    WaterSpeedInt = (int)WaterSpeed;                  //将水流速拆分成整数和小数存入Flash
    WaterSpeedFloat = ((int)(WaterSpeed * 10) % 10);
    
    Flash_FifoBuf[6] &= ~(0x00000fff << 2);
    Flash_FifoBuf[6] |= (WaterSpeedInt << 6);
    Flash_FifoBuf[6] |= (WaterSpeedFloat << 2);
      
//    printf ("the water speed is: %f\r\n", WaterSpeed);

    /* ****************** 水位测量 *************** */
    CheckWaterLevel();


    uwRet = LOS_TaskDelay(1000 * GetSensorTime);//系统延时1S
		if(uwRet !=LOS_OK)
      return;
  }
}


UINT32 creat_GetSelfDatTask (void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 4;                   //任务优先级，0最高
	task_init_param.pcName = "SelfDat";                //???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_GetSelfDat;//任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//创建任务
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}



