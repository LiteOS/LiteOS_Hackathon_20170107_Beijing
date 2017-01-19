#include "common.h"

UINT32 g_UpDatTskHandle;

void task_DatUp2Server (void)
{
  UINT32 uwRet = LOS_OK;
  
  uint8_t UpLoadTimeCount = 10;   //10
  uint8_t UpWarmingTimeCount = 0;
  uint8_t UpWarmingAgainCount = 0;
  uint8_t ClearWarmingUpCount = 0;
  
  uint8_t ErrorCount = 0;
  uint8_t statue = 0;
  uint8_t i = 0;
  uint8_t SlaveSerial = 0;
  
  while(1)
  {
    ErrorCount = 0;
    statue = 0;
    i = 0;
    SlaveSerial = 0;
    
    UpLoadTimeCount++;
    UpWarmingTimeCount++;
    UpWarmingAgainCount++;
    ClearWarmingUpCount++;
    
    
    if (UpWarmingAgainCount >= UpWarmingAgainCountTime)
    {
      if(TCPIsUsed == 0)
      {
        TCPIsUsed = 1;
        
        TIM_Cmd(TIM2, DISABLE);	      //关闭任务调度定时器
        LED_Green(ON);                //打开绿色指示灯
        UpWarmingAgainCount = 0;       //重置计数标志位
                
        ErrorCount = 0;               //重置错误计数值
        do
        {
          statue = UpWarmingDatAgain_TCP();    //上传报警
          ErrorCount++;             
          if (ErrorCount > 2) 
          {
            ErrorCount = 0;
            TCPIsUsed = 0;
            GPRS_Rest();                  //如果上报出错，重置GPRS
            statue = DefaultValue;
            TCPIsUsed = 1;
          }
        }while(statue != SUCCESS);
        
        LED_Green(OFF);               //关闭绿色指示灯
        TIM_Cmd(TIM2, ENABLE);
        TCPIsUsed = 0;  
      }
    }
    
    /* 检查一次有无报警以及有无新设备需要上报 */
    if (UpWarmingTimeCount >= UpWarmingTime)      //检查一次有无警报
    {
      if(TCPIsUsed == 0)
      {
        TCPIsUsed = 1;
        
        TIM_Cmd(TIM2, DISABLE);	      //关闭任务调度定时器
        LED_Green(ON);                //打开绿色指示灯
        UpWarmingTimeCount = 0;       //重置计数标志位
        
        if (SlaveNeedAddBuf[0] > 0)       //判断是否有新设备需要注册
        {
          for (i = 1; i <= SlaveNeedAddBuf[0]; i++)
          {
            SlaveSerial = Search_SlaveID(SlaveNeedAddBuf[i]);
          
            ErrorCount = 0;
            do
            {
              statue = AddSlaveID2Server(SlaveNeedAddBuf[i], SlaveSerial);
              ErrorCount++;
              if (ErrorCount >= 2)
              {
                ErrorCount = 0;
                TCPIsUsed = 0;
                GPRS_Rest();
                statue = DefaultValue;
                TCPIsUsed = 1;
              }
             }while(statue != SUCCESS);
          }
          SlaveNeedAddBuf[0] = 0;
        }
        
        ErrorCount = 0;               //重置错误计数值
        do
        {
          statue = UpWarmingDat_TCP();    //上传报警
          ErrorCount++;             
          if (ErrorCount > 2) 
          {
            ErrorCount = 0;
            TCPIsUsed = 0;
            GPRS_Rest();                  //如果上报出错，重置GPRS
            statue = DefaultValue;
            TCPIsUsed = 1;
          }
        }while(statue != SUCCESS);
        
        LED_Green(OFF);               //关闭绿色指示灯
        TIM_Cmd(                TIM2, ENABLE);
        TCPIsUsed = 0;  
      }
    }
    
    ErrorCount = 0;
    if (ClearWarmingUpCount >= ClearWarmingUpTime)
    {
      if (TCPIsUsed == 0)
      {
        TCPIsUsed = 1;
        LED_Green(ON);
        ClearWarmingUpCount = 0;
        TIM_Cmd(TIM2, DISABLE);	
        
        if (WarmClearStatue > 0)
        {
          WarmClearStatue = 0;
          do
          {
            statue = UpLoadDat_TCP();
            ErrorCount++;
            if (ErrorCount > 2)
            {
              ErrorCount = 0;
              TCPIsUsed = 0;
              GPRS_Rest();
              TCPIsUsed = 1;
            }
          }while(statue != SUCCESS);
        }
        
        TIM_Cmd(TIM2, ENABLE);	
        LED_Green(OFF);
//        Flash_WriteBufToPage();
        TCPIsUsed = 0;
      }
    }
    
    ErrorCount = 0;
    if (UpLoadTimeCount >= UpLoadingTime)         //向服务器上传一次数据
    {
      if (TCPIsUsed == 0)
      {
        TCPIsUsed = 1;
        
        LED_Green(ON);
        UpLoadTimeCount = 0;
        TIM_Cmd(TIM2, DISABLE);	
        do
        {
          statue = UpLoadDat_TCP();
          ErrorCount++;
          if (ErrorCount > 2)
          {
            ErrorCount = 0;
            TCPIsUsed = 0;
            GPRS_Rest();
            TCPIsUsed = 1;
          }
        }while(statue != SUCCESS);
        TIM_Cmd(TIM2, ENABLE);	
        LED_Green(OFF);
//        Flash_WriteBufToPage();
        TCPIsUsed = 0;
      }
    }
    
    uwRet = LOS_TaskDelay(1000 * AlarmTime);//系统延时1S
		if(uwRet !=LOS_OK)
      return;
  }
}


UINT32 creat_UpDat2ServerTask (void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 4;                   //任务优先级，0最高
	task_init_param.pcName = "DatUp";                 //name
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_DatUp2Server;//任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;//任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_UpDatTskHandle,&task_init_param);//创建任务
  
  
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}