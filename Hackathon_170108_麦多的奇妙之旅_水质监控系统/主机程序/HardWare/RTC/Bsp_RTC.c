#include "common.h"

long int NowTime = 0;

_calendar_obj calendar;   //时钟结构体 

int User_year = 2016, User_month = 12, User_day = 12, User_hour = 12, User_min = 12, User_sec = 12;

//int User_year, User_month, User_day, User_hour, User_min, User_sec;

void RTC_irqhandle (void)
{
  if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)         //闹钟中断
	{
    RTC_ClearITPendingBit(RTC_IT_ALR);		        //清闹钟中断	 
    RTC_WaitForLastTask();

    USART1_SendBuf ("RTC_Task  ***********************haha*************\r\n");
    
    
    NowTime = RTC_GetCounter();
    RTC_SetAlarm(NowTime + AlarmTime);            //重新设置闹钟
    
    RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);  //清闹钟中断
    RTC_WaitForLastTask();	 
    
  }
  

}

void RTC_nvic(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

VOID RTC_Interrupt(VOID)
{
  UINTPTR uvIntSave;
  uvIntSave = LOS_IntLock();
  RTC_nvic();
  
  LOS_HwiCreate(41, 4,0,RTC_irqhandle,0);//创建中断
  LOS_IntRestore(uvIntSave);
}

/*
 * 函数名：RTC_Init
 * 描述  ：实时时钟配置
 * 输入  ：无
 * 返回  ：0： 成功
 */
u8 RTC_Init(void)
{
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	                      //使能后备寄存器访问  
//	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
//	{	 			
//		BKP_DeInit();	                                    //复位备份区域 	
		RCC_LSEConfig(RCC_LSE_ON);	                      //设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)	    //检查指定的RCC标志位设置与否,等待低速晶振就绪
			{
        temp++;
        UserDelay_KeyScan(10);
			}
      
    if(temp>=250)return 1;                            //初始化时钟失败,晶振有问题	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		        //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	                          //使能RTC时钟  
		RTC_WaitForLastTask();	                          //等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();		                          //等待RTC寄存器同步  
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);		            //使能RTC秒中断
    RTC_ITConfig(RTC_IT_ALR,ENABLE) ;                 //使能闹钟中断
		RTC_WaitForLastTask();	                          //等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();                            // 允许配置	
		RTC_SetPrescaler(32767);                          //设置RTC预分频的值
		RTC_WaitForLastTask();	                          //等待最近一次对RTC寄存器的写操作完成
 
    RTC_ExitConfigMode();                             //退出配置模式  
    NowTime=RTC_GetCounter();
    RTC_WaitForLastTask();	                          //等待最近一次对RTC寄存器的写操作完成
    RTC_SetAlarm(NowTime+AlarmTime);                  //设置闹钟
//    if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)
//    {
      RTC_Set(User_year, User_month,User_day,User_hour,User_min, User_sec);              //设置时间	
//    }
		RTC_ExitConfigMode();                             //退出配置模式  
    NowTime=RTC_GetCounter();
    RTC_WaitForLastTask();	                          //等待最近一次对RTC寄存器的写操作完成
    RTC_SetAlarm(NowTime+AlarmTime);                  //设置闹钟
//		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	        //向指定的后备寄存器中写入用户程序数据
//	}
//	else//系统继续计时
//	{
//		RTC_WaitForSynchro();	                          //等待最近一次对RTC寄存器的写操作完成
//    RTC_ITConfig(RTC_IT_ALR,ENABLE) ;               //使能闹钟中断
//		RTC_WaitForLastTask();	                        //等待最近一次对RTC寄存器的写操作完成
//    NowTime=RTC_GetCounter();
//    RTC_WaitForLastTask();	                          //等待最近一次对RTC寄存器的写操作完成
//    RTC_SetAlarm(NowTime+AlarmTime);                  //设置闹钟
//	}
//	RTC_Get();      

  RTC_Interrupt();  
  
	return 0;

}		 				    

/*
 * 函数名：Is_Leap_Year
 * 描述  ：判断是否是闰年函数
 *          月份   1  2  3  4  5  6  7  8  9  10 11 12
 *          闰年   31 29 31 30 31 30 31 31 30 31 30 31
 *          非闰年 31 28 31 30 31 30 31 31 30 31 30 31
 * 输入  ：year：年份
 * 返回  ：输出:该年份是不是闰年.1,是.0,不是
 */
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   


//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/*
 * 函数名：RTC_Set
 * 描述  ：设置时钟
 *          把输入的时钟转换为秒钟，以1970年1月1日为基准
 * 输入  ：无
 * 返回  ：0,成功;其他:错误代码.
 */
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	                          //把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;          //闰年的秒钟数
		else seccount+=31536000;			                  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	                              //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;              //月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;   //闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;                    //把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;                         //小时秒钟数
    seccount+=(u32)min*60;	                        //分钟秒钟数
	seccount+=sec;                                    //最后的秒钟加上去

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	                    //使能RTC和后备寄存器访问 
	RTC_SetCounter(seccount);                         //设置RTC计数器的值

	RTC_WaitForLastTask();	                          //等待最近一次对RTC寄存器的写操作完成  	
	return 0;	    
}


/*
 * 函数名：RTC_Alarm_Set
 * 描述  ：初始化闹钟		
 * 输入  ：年份
 * 返回  ：0,成功;其他:错误代码.
 */
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	                            //把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;            //闰年的秒钟数
		else seccount+=31536000;			                    //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	                                //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;                //月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;     //闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;                      //把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;                           //小时秒钟数
    seccount+=(u32)min*60;	                          //分钟秒钟数
	seccount+=sec;                                      //最后的秒钟加上去 			    
  /* 设置时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	                                            //使能后备寄存器访问  	
	RTC_SetAlarm(seccount); 
	RTC_WaitForLastTask();	                            //等待最近一次对RTC寄存器的写操作完成  	
	
	return 0;	    
}

/*
 * 函数名：RTC_Get
 * 描述  ：得到当前的时间
 * 输入  ：无
 * 返回  ：0,成功;其他:错误代码.
 */
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;                 //得到天数(秒钟数对应的)
	if(daycnt!=temp)                      //超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	                        //从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))           //是闰年
			{
				if(temp>=366)temp-=366;         //闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	                  //平年 
			temp1++;  
		}   
		calendar.w_year=temp1;              //得到年份
		temp1=0;
		while(temp>=28)                     //超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)   //当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;                       //闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];   //平年
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	        //得到月份
		calendar.w_date=temp+1;  	        //得到日期 
    
    if ((calendar.w_month == 13) && (calendar.w_date == 1))
    {
      calendar.w_month = 12;
      calendar.w_date = 31;
    }
	}
	temp=timecount%86400;     		      //得到秒钟数   	   
	calendar.hour=temp/3600;     	      //小时
	calendar.min=(temp%3600)/60; 	      //分钟	
	calendar.sec=(temp%3600)%60; 	      //秒钟
	return 0;
}	 

/*
 * 函数名：RTC_IRQHandler
 * 描述  ：RTC时钟中断
 * 输入  ：无
 * 返回  ：无
 */
//void RTC_IRQHandler(void)
//{		 
////  static int UpLoadTimeCount = 10;   //10
////  static int UpWarmingTimeCount = 0;
////  static int UpWarmingAgainCount = 0;
////  static int ClearWarmingUpCount = 0;
////  
////  uint8_t ErrorCount = 0;
////  uint8_t statue = 0;
////  uint8_t i = 0;
////  uint8_t SlaveSerial = 0;
//  
//	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)         //闹钟中断
//	{
////    UpLoadTimeCount++;
////    UpWarmingTimeCount++;
////    UpWarmingAgainCount++;
////    ClearWarmingUpCount++;
////    
////		RTC_ClearITPendingBit(RTC_IT_ALR);		        //清闹钟中断	 
////    RTC_WaitForLastTask();

////    /* 警报重复上传 */
////    if (UpWarmingAgainCount >= UpWarmingAgainCountTime)
////    {
////      if(TCPIsUsed == 0)
////      {
////        TCPIsUsed = 1;
////        
////        TIM_Cmd(TIM2, DISABLE);	      //关闭任务调度定时器
////        LED_Green(ON);                //打开绿色指示灯
////        UpWarmingAgainCount = 0;       //重置计数标志位
////                
////        ErrorCount = 0;               //重置错误计数值
////        do
////        {
////          statue = UpWarmingDatAgain_TCP();    //上传报警
////          ErrorCount++;             
////          if (ErrorCount > 2) 
////          {
////            ErrorCount = 0;
////            TCPIsUsed = 0;
////            GPRS_Rest();                  //如果上报出错，重置GPRS
////            statue = DefaultValue;
////            TCPIsUsed = 1;
////          }
////        }while(statue != SUCCESS);
////        
////        LED_Green(OFF);               //关闭绿色指示灯
////        TIM_Cmd(TIM2, ENABLE);
////        TCPIsUsed = 0;  
////      }
////    }
////    
////    /* 检查一次有无报警以及有无新设备需要上报 */
////    if (UpWarmingTimeCount >= UpWarmingTime)      //检查一次有无警报
////    {
////      if(TCPIsUsed == 0)
////      {
////        TCPIsUsed = 1;
////        
////        TIM_Cmd(TIM2, DISABLE);	      //关闭任务调度定时器
////        LED_Green(ON);                //打开绿色指示灯
////        UpWarmingTimeCount = 0;       //重置计数标志位
////        
////        if (SlaveNeedAddBuf[0] > 0)       //判断是否有新设备需要注册
////        {
////          for (i = 1; i <= SlaveNeedAddBuf[0]; i++)
////          {
////            SlaveSerial = Search_SlaveID(SlaveNeedAddBuf[i]);
////          
////            ErrorCount = 0;
////            do
////            {
////              statue = AddSlaveID2Server(SlaveNeedAddBuf[i], SlaveSerial);
////              ErrorCount++;
////              if (ErrorCount >= 2)
////              {
////                ErrorCount = 0;
////                TCPIsUsed = 0;
////                GPRS_Rest();
////                statue = DefaultValue;
////                TCPIsUsed = 1;
////              }
////             }while(statue != SUCCESS);
////          }
////          SlaveNeedAddBuf[0] = 0;
////        }
////        
////        ErrorCount = 0;               //重置错误计数值
////        do
////        {
////          statue = UpWarmingDat_TCP();    //上传报警
////          ErrorCount++;             
////          if (ErrorCount > 2) 
////          {
////            ErrorCount = 0;
////            TCPIsUsed = 0;
////            GPRS_Rest();                  //如果上报出错，重置GPRS
////            statue = DefaultValue;
////            TCPIsUsed = 1;
////          }
////        }while(statue != SUCCESS);
////        
////        LED_Green(OFF);               //关闭绿色指示灯
////        TIM_Cmd(                TIM2, ENABLE);
////        TCPIsUsed = 0;  
////      }
////    }
////    
////    ErrorCount = 0;
////    if (ClearWarmingUpCount >= ClearWarmingUpTime)
////    {
////      if (TCPIsUsed == 0)
////      {
////        TCPIsUsed = 1;
////        LED_Green(ON);
////        ClearWarmingUpCount = 0;
////        TIM_Cmd(TIM2, DISABLE);	
////        
////        if (WarmClearStatue > 0)
////        {
////          WarmClearStatue = 0;
////          do
////          {
////            statue = UpLoadDat_TCP();
////            ErrorCount++;
////            if (ErrorCount > 2)
////            {
////              ErrorCount = 0;
////              TCPIsUsed = 0;
////              GPRS_Rest();
////              TCPIsUsed = 1;
////            }
////          }while(statue != SUCCESS);
////        }
////        
////        TIM_Cmd(TIM2, ENABLE);	
////        LED_Green(OFF);
//////        Flash_WriteBufToPage();
////        TCPIsUsed = 0;
////      }
////    }
////    
////    ErrorCount = 0;
////    if (UpLoadTimeCount >= UpLoadingTime)         //向服务器上传一次数据
////    {
////      if (TCPIsUsed == 0)
////      {
////        TCPIsUsed = 1;
////        
////        LED_Green(ON);
////        UpLoadTimeCount = 0;
////        TIM_Cmd(TIM2, DISABLE);	
////        do
////        {
////          statue = UpLoadDat_TCP();
////          ErrorCount++;
////          if (ErrorCount > 2)
////          {
////            ErrorCount = 0;
////            TCPIsUsed = 0;
////            GPRS_Rest();
////            TCPIsUsed = 1;
////          }
////        }while(statue != SUCCESS);
////        TIM_Cmd(TIM2, ENABLE);	
////        LED_Green(OFF);
//////        Flash_WriteBufToPage();
////        TCPIsUsed = 0;
////      }
////    }
////    
////    RTC_Get();                                    //更新时间      
////	  NowTime = RTC_GetCounter();
////    RTC_SetAlarm(NowTime + AlarmTime);            //重新设置闹钟
////    
////    RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);  //清闹钟中断
////    RTC_WaitForLastTask();	 
//  } 				  								   						 	   	 
//}
