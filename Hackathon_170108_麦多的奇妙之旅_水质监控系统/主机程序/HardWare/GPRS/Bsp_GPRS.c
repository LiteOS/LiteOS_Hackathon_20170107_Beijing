#include "common.h"

uint8_t TCPIsUsed = 0;            //TCP是否正在被使用标志位
uint8_t GetNet = 0;               //是否连接到网络标志位
//uint16_t WarmClearStatue = 0;     //是否所有警报都已报警

/*
 * 函数名：GetNetTime
 * 描述  ：获取网络时间
 * 输入  ：无
 * 返回  ：无
 */
void GetNetTime (void)
{
  char* BufPoint = 0;
  int year = 2000, month = 0, day = 0, hour = 0, minute = 0, second = 0;;
  
  UserDelay_KeyScan(100);
  BufPoint = strstr(Uart3_ReceiveDat, "UTC(NIST)");
  BufPoint -= 31;
  
//  USART3_SendBuf((uint8_t*)BufPoint);
  
  year += (((BufPoint[0] - 0x30) * 10) + (BufPoint[1] - 0x30));
  month = (((BufPoint[3] - 0x30) * 10) + (BufPoint[4] - 0x30));
  day = (((BufPoint[6] - 0x30) * 10) + (BufPoint[7] - 0x30));
  hour = (((BufPoint[9] - 0x30) * 10) + (BufPoint[10] - 0x30));
  minute = (((BufPoint[12] - 0x30) * 10) + (BufPoint[13] - 0x30));
  second = (((BufPoint[15] - 0x30) * 10) + (BufPoint[16] - 0x30));

  hour += 8;
  if (hour > 23)
  {
    hour -= 24;
    day++;
    if (((month == 2) || (month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day == 31))
    {
      day = 0;
      month++;
      if (month == 13)
      {
        year++;
        month = 1;
      }
    }
    
    if (((month == 1) || (month == 3) || (month == 5) || (month == 7) 
          || (month == 8) || (month == 10) || (month == 12)) && (day == 32))
    {
      day = 0;
      month++;
      if (month == 13)
      {
        year++;
        month = 1;
      }
    }
  }
  
  if (month == 13)
    month = 1;
  calendar.w_year = year;
  calendar.w_month = month;
  calendar.w_date = day;
  calendar.hour = hour;
  calendar.min = minute;
  calendar.sec = second;
  
  
}

//uint8_t UpTmpoffset2Server (uint8_t* SlaveChipIDString)
//{
//  uint8_t ErrorCount = 0;
//  uint8_t TimeDelayCount = 0;
//  uint8_t MsgSendBuf[300];
//  sprintf((char*)MsgSendBuf, "{\"type\":\"lost\",\"lost\":\"%s\"}\r\n", SlaveChipIDString);
//  
//  do
//  {
//    UserDelay_KeyScan(1000);
//    Uart3_BufClear();  
//    USART3_SendBuf(MsgSendBuf);  
//    TimeDelayCount = 0;
//    while(TimeDelayCount < 40)
//    {
//      UserDelay_KeyScan(100);
//      TimeDelayCount++;
//      if (strstr(Uart3_ReceiveDat, "true") != NULL)
//        return SUCCESS;  
//    } 
//    ErrorCount++;
//    if (ErrorCount > 3)
//    {
//      return ErrorFromServe;
//    }
//  }while(strstr(Uart3_ReceiveDat, "true") == NULL);
//  return SUCCESS;  
//}

/*
 * 函数名：GetOffDatProcess
 * 描述  ：对服务器端获得的PH和温度补偿数据进行处理
 * 输入  ：无
 * 返回  ：无
 */
float GetOffDatProcess (uint8_t Index)
{
  char* position;
  char* TempPosition;
  uint8_t length = 0;
  float Num = 0.0;
  position = (strstr(Uart3_ReceiveDat, "value") + 7);
    
  TempPosition = (strstr(position, "."));
  length = TempPosition - position;
  
  if (*position == '-')                   //如果是个负数
  {
    position++;     //指针往后移一位，把符号滤过
    length--;       //整数长度减1，之前包含了符号
  }
  
  switch (Index)
  {
    case PH_AN_Offset:
    case PH_BN_Offset:
      switch(length)        //判断整数长度
      {
        case 1:
          Num = position[0] - 0x30;
          break;
        case 2:
          Num = (position[0] - 0x30) * 10 + (position[1] - 0x30);
          break;
        case 3:
          Num = (position[0] - 0x30) * 100 + (position[1] - 0x30) * 10 + (position[2] - 0x30);
          break;
        case 4:
          Num = (position[0] - 0x30) * 1000 + (position[1] - 0x30) * 100 + (position[2] - 0x30) * 10 + (position[3] - 0x30);
          break;
        case 5:
          Num = (position[0] - 0x30) * 10000 + (position[1] - 0x30) * 1000 + (position[2] - 0x30) * 100 + (position[3] - 0x30) * 10 + (position[4] - 0x30);
          break;
      }
      /* 计算出最终结果 */
      Num += (float)((position[length + 1] - 0x30) / 10.0 + (position[length + 2] - 0x30) / 100.0 
            + (position[length + 3] - 0x30) / 1000.0 + (position[length + 4] - 0x30) / 10000.0
            + (position[length + 5] - 0x30) / 100000.0 + (position[length + 6] - 0x30) / 1000000.0 
            + (position[length + 7] - 0x30) / 10000000.0 + (position[length + 8] - 0x30) / 100000000.0);
          break;
    case Tmp_Offset:
      switch (length)
      {
        case 1:
          Num = (float)((position[0] - 0x30) + (float)((position[2] - 0x30) / 10.0));
          break;
        case 2:
          Num = (float)(((position[0] - 0x30) * 10) + (position[1] - 0x30) + ((position[3] - 0x30) / 10.0));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
  if (*(position - 1) == '-')
              Num = -Num;
  
  return Num;
}


/*
 * 函数名：GetOffsetDat
 * 描述  ：从服务器端获取PH斜率及温度补偿数据
 * 输入  ：无
 * 返回  ： SUCCESS: 成功
 *          else:    失败
 */
uint8_t GetOffsetDat (void)
{
  float num = 0.0;
  uint8_t statue, i;
  uint8_t SendBuf[50];
  uint8_t MsgLength = 0;  
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;

  for (i = 1; i <= 3; i++)
  {
    sprintf ((char *)SendBuf, "{\"type\":\"number\",\"number\":0,\"value\":%d}\r\n", i);
    MsgLength = strlen((char *)SendBuf);
    
    statue = ReqSendTcp(MsgLength);
    if (statue != SUCCESS)
      return statue;
      
    do
    {
      USART3_SendBuf(SendBuf); 
      Delay_ms(100);
      Uart3_BufClear();  
      while(TimeDelayCount < 40)
      {
        Delay_ms(100);
        TimeDelayCount++;
        if (strstr(Uart3_ReceiveDat, ".") != NULL)
          break;
      }
      ErrorCount++;
      if (ErrorCount > 3)
      {
        return ErrorFromServe;
      }
    }while(strstr(Uart3_ReceiveDat, ".") == NULL);
    num = GetOffDatProcess(i);
    
    switch (i)
    {
      case PH_AN_Offset:
        OfficDatBuf[3] = num * 1000000;
        break;
      case PH_BN_Offset:
        OfficDatBuf[4] = num * 1000000;
        break;
      case Tmp_Offset:
        OfficDatBuf[0] = (int)(num * 10);
        break;
      default:
        break;
    }
  
  } 
  
  return SUCCESS;
}
  

/*
 * 函数名：UpOffsetDat
 * 描述  ：向服务器端上传PH斜率及温度补偿数据
 * 输入  ：Index: 
 *            PH_AN_Offset：PH酸性斜率
 *            PH_BN_Offset：PH碱性斜率
 *            Tmp_Offset：温度补偿值
 * 返回  ： SUCCESS: 成功
 *          else:    失败
 */
uint8_t UpOffsetDat (uint8_t Index, float Offset)
{
  uint8_t statue;
  uint8_t SendBuf[50];
  uint8_t MsgLength = 0;  
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  
  sprintf ((char *)SendBuf, "{\"type\":\"number\",\"number\":%d,\"value\":%f}\r\n",Index, Offset);
  MsgLength = strlen((char *)SendBuf);
  
  /* 向A6申请发送TCP消息 */
  statue = ReqSendTcp(MsgLength);
  if (statue != SUCCESS)
    return statue;
  
  do
  {
    Uart3_BufClear();  
    USART3_SendBuf(SendBuf); 
    while(TimeDelayCount < 40)
    {
      Delay_ms(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, "true") != NULL)
        return SUCCESS; 
    }
    ErrorCount++;
    if (ErrorCount > 3)
    {
      return ErrorFromServe;
    }
  }while(strstr(Uart3_ReceiveDat, "true") == NULL);


  return SUCCESS;
}

/*
 * 函数名：UpLostMsg
 * 描述  ：上报从机丢失信息
 * 输入  ：SlaveID：丢失的从机ID
 * 返回  ： SUCCESS: 成功
 *          else:    失败
 */
uint8_t UpLostMsg (uint8_t* SlaveID)
{
  uint8_t statue;
  uint8_t ReqSendBuf[100];
  uint8_t SalveIDStrBuf[9];
  int MsgLength = 0;  
  
  
  sprintf ((char *)ReqSendBuf, "{\"type\":\"lost\",\"lost\":\"%s\"}\r\n", SlaveID);
  MsgLength = strlen((char *)ReqSendBuf);
  
  /* 向A6申请发送TCP消息 */
  statue = ReqSendTcp(MsgLength);
  if (statue != SUCCESS)
    return statue;

  /* 发送添加从机包 */
  statue = UpSlaveLost2Server(SlaveID);
  if (statue != SUCCESS)
    return statue;
  
  return SUCCESS;
}

/*
 * 函数名：Bsp_Init
 * 描述  ：设备绑定
 * 输入  ：无
 * 返回  ：无
 */
uint8_t DevBound (void)
{
  uint8_t ReqSendBuf[100];
  uint8_t statue = 0;
  int MsgLength = 0;
  uint8_t ErrorCount;
  uint16_t Delay_TimeCount;
  
  while(TCPIsUsed);
  if (TCPIsUsed == 0)
  {
    TCPIsUsed = 1;
    TIM_Cmd(TIM2, DISABLE);		
    
    Beep_ON_Nop(100);                                             //提示开始发送绑定信息
    sprintf ((char *)ReqSendBuf, "{\"type\":\"add2user\",\"add2user\":\"%s\"}\r\n", ChipID_String);
    MsgLength = strlen((char *)ReqSendBuf);                       //计算长度
    
    ErrorCount = 0;    
    do
    {
      statue = ReqSendTcp(MsgLength);
      if (statue != SUCCESS)
      {
        TCPIsUsed = 0;
        return statue;
      }
      Uart3_BufClear();  
      UserDelay_KeyScan(1500);
      USART3_SendBuf(ReqSendBuf);  
      Delay_TimeCount = 0;
      while(Delay_TimeCount < 200)
      {
        UserDelay_KeyScan(100);
        Delay_TimeCount++;
        if (strstr(Uart3_ReceiveDat, "\"type\":\"add2user\",\"add2user\":true") != NULL)
          break;
        if (strstr(Uart3_ReceiveDat, "\"type\":\"add2user\",\"add2user\":false") != NULL)
        {                            
          Beep_ON_Nop(500);   //两声提示音提示绑定失败
          Beep_ON_Nop(500);
          TCPIsUsed = 0;
          return FAILUED;     
        }
      }     
      
      ErrorCount++;
      if (ErrorCount > 2)
      {
        Beep_ON_Nop(500);   //两声提示音提示绑定失败
        Beep_ON_Nop(500);
        TCPIsUsed = 0;
        return FAILUED;      
      }
    }while(strstr(Uart3_ReceiveDat, "\"type\":\"add2user\",\"add2user\":true") == NULL);
    
    Beep_ON_Nop(1000);      //一声长提示音提示绑定成功
    TIM_Cmd(TIM2, ENABLE);
    TCPIsUsed = 0;
    return SUCCESS;
  }
  TCPIsUsed = 0;
  return SUCCESS;
}

/*
 * 函数名：SendWarmingPack
 * 描述  ：发送报警信息
 * 输入  ：sensor：   传感器    
 *            ph、sensortemp、cputemp、speed、water、power
 *         warntype： 警告类型
 *            -1：太低    0：正常   1：太高
 *         did：      设备ID
 * 返回  ：SUCCESS:  成功
 *         其他：    错误
 */
uint8_t SendWarmingPack (uint8_t* sensor, int8_t warntype, uint8_t* did)
{
  uint8_t MsgSendBuf[300];
  uint8_t ReqSendBuf[30];
  uint8_t MsgLengthBuf[15];
  int MsgLength = 0;
  int ErrorCount = 0;
  int ErrorCount1 = 0;
  int Delay_TimeCount = 0;
  uint8_t statue;
  
  do
  { 
    sprintf ((char *)ReqSendBuf, "{\"type\":\"warn\",\"warn\":\"%s\"}\r\n", sensor);    //请求发送警告信息
    MsgLength = strlen((char *)ReqSendBuf);                         //计算长度
      
    ErrorCount = 0;
    ErrorCount1 = 0;
    /* 发送TCP申请和警报前包 */
    
    do
    {
      statue = ReqSendTcp(MsgLength);
      if (statue != SUCCESS)
        return statue;
      
      Uart3_BufClear();                   //清空串口缓存区
      UserDelay_KeyScan(1500);                     //实际发现延时一会稳定性能上去
      USART3_SendBuf(ReqSendBuf);         //发送数据
      Delay_TimeCount = 0;
      while(Delay_TimeCount < 30)
      {
        UserDelay_KeyScan(500);
        Delay_TimeCount++;
        if (strstr(Uart3_ReceiveDat, "true") != NULL)   //等待服务器反馈成功
          break;
      }      
      ErrorCount++;
      if (ErrorCount > 3)                //服务器无响应超时退出
      {
        return ServeNoResponse;      
      }
    }while(strstr(Uart3_ReceiveDat, "true") == NULL);
    
    sprintf ((char*)MsgSendBuf, "{\"type\":\"%d\",\"value\":\"%s\"}\r\n",warntype, did); 
    MsgLength = strlen((char*)MsgSendBuf);
    
    statue = ReqSendTcp(MsgLength);
    if (statue != SUCCESS)
      return statue;
    
    UserDelay_KeyScan(500);
    Uart3_BufClear();
    USART3_SendBuf(MsgSendBuf);
    Delay_TimeCount = 0;

    while(Delay_TimeCount < 50)
    {
      UserDelay_KeyScan(100);
      Delay_TimeCount++;
      if (strstr(Uart3_ReceiveDat, "{\"type\":\"warn\",\"warn\":true}") != NULL || (strstr(Uart3_ReceiveDat, "no user") != NULL))
        break;
      if (strstr(Uart3_ReceiveDat, "{\"type\":\"warn\",\"warn\":false}") != NULL)
        return FAILUED;
    }
    ErrorCount1++;
    if (ErrorCount1 > 3)
      return ServeNoResponse; 
  }while((strstr(Uart3_ReceiveDat, "{\"type\":\"warn\",\"warn\":true}") == NULL) && (strstr(Uart3_ReceiveDat, "no user") == NULL));
  return SUCCESS;
}

//float WaterMaxTest = 0.0;
//float WaterMinTest = 0.0;
//float WaterTempTest = 0.0;

/*
 * 函数名：UpWarmingDat_TCP
 * 描述  ：上传报警信息
 * 输入  ：无
 * 返回  ：SUCCESS: 成功
 *          其他：失败
 */
uint8_t EnvirTempWarmCount = 0;
uint8_t WaterTempWarmCount = 0;
uint8_t WaterSpeedWarmCount = 0;
uint8_t WaterLevelWarmCount = 0;
uint8_t PHWarmCount = 0;
uint8_t PowerWarmCount = 0;

//uint8_t WarmUpCount[MAX_SlaveNum + 1][6];

uint8_t UpWarmingDatAgain_TCP (void)
{
  uint8_t i = 0, k = 0, statue = 0;
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
  float PH, WaterTemp, EnvirTemp, WaterSpeed;
  uint8_t SalveIDStrBuf[9];
  
  /* 主机报警检测 */
  for (i = 0; i < 6; i++)
  {
    if ((WarmUpCount[0][i] >= 1) && (WarmUpCount[0][i] < 3))   //如果报警过，且报警次数小于三次
    {
      switch (i)
      {
        case 0:
          EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
          EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
          EnvirTemp = ((Flash_FifoBuf[6] >> 18) & 0xff) + (((Flash_FifoBuf[6] >> 14) & 0xf) / 10.0);
        
          if (EnvirTemp > EnvirTempMax)
            statue = SendWarmingPack((uint8_t*)"cputemp", 1, (uint8_t*)ChipID_String);
          else if (EnvirTemp < EnvirTempMin)
            statue = SendWarmingPack((uint8_t*)"cputemp", -1, (uint8_t*)ChipID_String);
          else 
            statue = SUCCESS;
          
          WarmUpCount[0][0]++;
          
          if (statue == SUCCESS)
          {
            Flash_FifoBuf[5] |= (1 << 19);  
          }
          else
            return FAILUED;
          break;
        case 1:
          WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
          WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
          WaterTemp = ((Flash_FifoBuf[5] & 0x3f) << 2) + ((Flash_FifoBuf[6] >> 30)& 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0); 
          
          
        
          if (WaterTemp > WaterTempMax)
            statue = SendWarmingPack((uint8_t*)"sensortemp", 1, (uint8_t*)ChipID_String);
          else if (WaterTemp < WaterTempMin)
            statue = SendWarmingPack((uint8_t*)"sensortemp", -1, (uint8_t*)ChipID_String);
          else 
            statue = SUCCESS;
          
          WarmUpCount[0][1]++;
          
          if (statue == SUCCESS)
          {
            Flash_FifoBuf[5] |= (1 << 15);   
          }
          else
            return FAILUED;
          break;
        case 2:
          WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
          WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
          WaterSpeed = ((Flash_FifoBuf[6] >> 6) & 0xff) + (((Flash_FifoBuf[6] >> 2) & 0xf) / 10.0);
          
          
        
          if (WaterSpeed > WaterSpeedMax)
            statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)ChipID_String);
          else if (WaterSpeed < WaterSpeedMin)
            statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)ChipID_String);
          else 
            statue = SUCCESS;
          
          WarmUpCount[0][2]++;
          
          if (statue == SUCCESS)
          {
            Flash_FifoBuf[5] |= (1 << 23); 
          }
          else
            return FAILUED;
          break;
        case 3:
          statue = SendWarmingPack((uint8_t*)"water", -1, (uint8_t*)ChipID_String);
          WarmUpCount[0][3]++;
        
          if (statue == SUCCESS)
          {
            
            Flash_FifoBuf[5] |= (1 << 25);                //已经向服务器发送消息
          }
          else
          {
            return FAILUED;
          }

          break;
        case 4:
          PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
          PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
          PH = (((Flash_FifoBuf[5] >> 10) & 0xf) + ((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);
          
          
        
          if (PH > PHMax)
            statue = SendWarmingPack((uint8_t*)"ph", 1, (uint8_t*)ChipID_String);
          else if (PH < PHMin)
            statue = SendWarmingPack((uint8_t*)"ph", -1, (uint8_t*)ChipID_String);
          else 
      //      statue = SendWarmingPack((uint8_t*)"ph", 0, (uint8_t*)ChipID_String);
            statue = SUCCESS;
          
          WarmUpCount[0][4]++;
          
          if (statue == SUCCESS)
          {
            
            Flash_FifoBuf[5] |= (1 << 17);  
          }
          else
            return FAILUED;
          break;
        case 5:
          statue = SendWarmingPack((uint8_t*)"power", -1, (uint8_t*)ChipID_String);
          WarmUpCount[0][5]++;
        
          if (statue == SUCCESS)
          {
            
            Flash_FifoBuf[5] |= (1 << 21);                //已经向服务器发送消息
          }
          else
          {
            return FAILUED;
          }
          break;
      }
    }
  }
  for (k = 1; k <= Flash_FifoBuf[0]; k++)
  {
    Chip2String(Flash_FifoBuf[7 + (k - 1) * 3], (char*)SalveIDStrBuf);
    for (i = 0; i < 6; i++)
    {
      if ((WarmUpCount[k][i] >= 1) && (WarmUpCount[k][i] < 3))   //如果报警过，且报警次数小于三次
      {
        switch (i)
        {
          case 0:
            {
              EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
              EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
              EnvirTemp = ((Flash_FifoBuf[7 + (k - 1) * 3 + 2] >> 24) & 0xff) + (((Flash_FifoBuf[7 + (k - 1) * 3 + 2] >> 20) & 0xf) / 10.0);
              
              WarmUpCount[k][0]++;
            
              if (EnvirTemp > EnvirTempMax)
                statue = SendWarmingPack((uint8_t*)"cputemp", 1, (uint8_t*)SalveIDStrBuf);
              else if (EnvirTemp < EnvirTempMin)
                statue = SendWarmingPack((uint8_t*)"cputemp", -1, (uint8_t*)SalveIDStrBuf);
              else 
                statue = SUCCESS;
              
              if (statue == SUCCESS)
              {
                
                Flash_FifoBuf[7 + (k - 1) * 3 + 1] |= (1 << 21);   
              }
              else
              {
                return FAILUED;
              }
            }
            break;
          case 1:
              {
                WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
                WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
                WaterTemp = (((Flash_FifoBuf[7 + (k - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (k - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
                
                WarmUpCount[k][1]++;
              
                if (WaterTemp > WaterTempMax)
                  statue = SendWarmingPack((uint8_t*)"sensortemp", 1, (uint8_t*)SalveIDStrBuf);
                else if (WaterTemp < WaterTempMin)
                  statue = SendWarmingPack((uint8_t*)"sensortemp", -1, (uint8_t*)SalveIDStrBuf);
                else 
                  statue = SUCCESS;
                
                if (statue == SUCCESS)
                {
                  
                  Flash_FifoBuf[7 + (k - 1) * 3 + 2] |= (1 << 4);  
                }
                else
                {
                  return FAILUED;
                }
              }
            break;
          case 2:
            {
              WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
              WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
              WaterSpeed = ((Flash_FifoBuf[7 + (k - 1) * 3 + 2] >> 12) & 0xff) + (((Flash_FifoBuf[7 + (k - 1) * 3 + 2] >> 8) & 0xf) / 10.0);
              
              WarmUpCount[k][2]++;
            
              if (WaterSpeed > WaterSpeedMax)
                statue = SendWarmingPack((uint8_t*)"speed", 1, (uint8_t*)SalveIDStrBuf);
              else if (WaterSpeed < WaterSpeedMin)
                statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)SalveIDStrBuf);
              else 
                statue = SUCCESS;
              
              if (statue == SUCCESS)
              {
                
                Flash_FifoBuf[7 + (k - 1) * 3 + 1] |= (1 << 23);   
              }
              else
              {
                return FAILUED;
              }
            }
            break;
          case 3:
            {
              statue = SendWarmingPack((uint8_t*)"water", -1, (uint8_t*)SalveIDStrBuf);
              WarmUpCount[k][3]++;
            
              if (statue == SUCCESS)
              {
                
                Flash_FifoBuf[7 + (k - 1) * 3 + 1] |= (1 << 25);                //已经向服务器发送消息
              }
              else
              {
                return FAILUED;
              }
            }
            break;
          case 4:
            {
              PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
              PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
              PH = (((Flash_FifoBuf[7 + (k - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (k - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
                    
              WarmUpCount[k][4]++;
            
              if (PH > PHMax)
                statue = SendWarmingPack((uint8_t*)"ph", 1, (uint8_t*)SalveIDStrBuf);
              else if (PH < PHMin)
                statue = SendWarmingPack((uint8_t*)"ph", -1, (uint8_t*)SalveIDStrBuf);
              else 
                statue = SUCCESS;
              
              if (statue == SUCCESS)
              {
                
                Flash_FifoBuf[7 + (k - 1) * 3 + 2] |= (1 << 6);  
              }
              else
              {
                return FAILUED;
              }
            }
            break;
          case 5:
            break;
          default:
            break;
        }
      }
    }
  }
  return SUCCESS;
}

/*
 * 函数名：UpWarmingDat_TCP
 * 描述  ：上传报警信息
 * 输入  ：无
 * 返回  ： SUCCESS: 成功
 *          else:    失败
 */
uint8_t UpWarmingDat_TCP (void)
{
  uint8_t i, statue = 0;
  uint8_t WarmCount = 0;
  int ErrorCount = 0;
  int Delay_TimeCount = 0;
  uint8_t MsgLengthBuf[15];
  uint8_t SalveIDStrBuf[9];
  int MsgLength = 0;
  
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
  float PH, WaterTemp, EnvirTemp, WaterSpeed;
  /* 主机采集值检测是否有超阈值的 */
  
  /* 水位报警 */
  if (((Flash_FifoBuf[5] >> 24) & 0x03) == 0x01)    //水位  如果存在警报且未上报服务器
  {
    WarmCount++;
    statue = SendWarmingPack((uint8_t*)"water", -1, (uint8_t*)ChipID_String);
    if (statue == SUCCESS)
    {
      WarmUpCount[0][3] = 1;
      Flash_FifoBuf[5] |= (1 << 25);                //已经向服务器发送消息
    }
    else
    {
      return FAILUED;
    }
  }
  
  /* 掉电报警 */
  if (((Flash_FifoBuf[5] >> 20) & 0x03) == 0x01)
  {
    WarmCount++;
    statue = SendWarmingPack((uint8_t*)"power", -1, (uint8_t*)ChipID_String);
    if (statue == SUCCESS)
    {
      WarmUpCount[0][5] = 1;
      Flash_FifoBuf[5] |= (1 << 21);                //已经向服务器发送消息
    }
    else
    {
      return FAILUED;
    }
  }
  
  /* 流速报警 */
  if (((Flash_FifoBuf[5] >> 22) & 0x03) == 0x01)
  {
    WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
    WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
    WaterSpeed = ((Flash_FifoBuf[6] >> 6) & 0xff) + (((Flash_FifoBuf[6] >> 2) & 0xf) / 10.0);
    
    if (WaterSpeed > WaterSpeedMax)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)ChipID_String);
    }
    else if (WaterSpeed < WaterSpeedMin)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)ChipID_String);
    }
    else 
//      statue = SendWarmingPack((uint8_t*)"speed", 1, (uint8_t*)ChipID_String);
      statue = SUCCESS;
    
    if (statue == SUCCESS)
    {
      WarmUpCount[0][2] = 1;
      Flash_FifoBuf[5] |= (1 << 23); 
    }
    else
    {
      return FAILUED;
    }
  }
  

  
  /* 板温报警 */
  if (((Flash_FifoBuf[5] >> 18) & 0x03) == 0x01)
  {
    EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
    EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
    EnvirTemp = ((Flash_FifoBuf[6] >> 18) & 0xff) + (((Flash_FifoBuf[6] >> 14) & 0xf) / 10.0);
    
    if (EnvirTemp > EnvirTempMax)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"cputemp", 1, (uint8_t*)ChipID_String);
    }
    else if (EnvirTemp < EnvirTempMin)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"cputemp", -1, (uint8_t*)ChipID_String);
    }
    else 
//      statue = SendWarmingPack((uint8_t*)"cputemp", 0, (uint8_t*)ChipID_String);
      statue = SUCCESS;
    
    if (statue == SUCCESS)
    {
      WarmUpCount[0][0] = 1;
      Flash_FifoBuf[5] |= (1 << 19);  
    }
    else
    {
      return FAILUED;
    }
  }
  

  
  /* PH报警 */
  if (((Flash_FifoBuf[5] >> 16) & 0x03) == 0x01)
  {
    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
    PH = (((Flash_FifoBuf[5] >> 10) & 0xf) + ((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);
    
    if (PH > PHMax)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"ph", 1, (uint8_t*)ChipID_String);
    }
    else if (PH < PHMin)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"ph", -1, (uint8_t*)ChipID_String);
    }
    else 
//      statue = SendWarmingPack((uint8_t*)"ph", 0, (uint8_t*)ChipID_String);
      statue = SUCCESS;
    
    if (statue == SUCCESS)
    {
      WarmUpCount[0][4] = 1;
      Flash_FifoBuf[5] |= (1 << 17);  
    }
    else
    {
      return FAILUED;
    }
  }
  
  /* 水温报警 */
  if (((Flash_FifoBuf[5] >> 14) & 0x03) == 0x01)
  {
    WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
//    WaterTempMin =    (Flash_FifoBuf[1] & 0xf  << 4) + ((Flash_FifoBuf[2] >> 28) & 0xf) +  (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0);
    WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
    WaterTemp = ((Flash_FifoBuf[5] & 0x3f) << 2) + ((Flash_FifoBuf[6] >> 30)& 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0); 

    
//    WaterMaxTest = WaterTempMax;
//    WaterMinTest = WaterTempMin;
//    WaterTempTest = WaterTemp;
    
    if (WaterTemp > WaterTempMax)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"sensortemp", 1, (uint8_t*)ChipID_String);
    }
    else if (WaterTemp < WaterTempMin)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"sensortemp", -1, (uint8_t*)ChipID_String);
    }
    else 
//      statue = SendWarmingPack((uint8_t*)"sensortemp", 0, (uint8_t*)ChipID_String);
      statue = SUCCESS;
    
    if (statue == SUCCESS)
    {
      WarmUpCount[0][1] = 1;
      Flash_FifoBuf[5] |= (1 << 15);   
    }
    else
    {
      return FAILUED;
    }
  }
  

  /* *************从机检测有无报警***************** */
  for (i = 1; i <= Flash_FifoBuf[0]; i++)
  {
    Chip2String(Flash_FifoBuf[7 + (i - 1) * 3], (char*)SalveIDStrBuf);
    
    /* 水位检测 */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 24) & 0x03) == 0x01)    //水位  如果存在警报且未上报服务器
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"water", -1, (uint8_t*)SalveIDStrBuf);
      if (statue == SUCCESS)
      {
        WarmUpCount[i][3] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 25);                //已经向服务器发送消息
      }
      else
      {
        return FAILUED;
      }
    }
    
    /* 流速检测 */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 22) & 0x03) == 0x01)
    {
      WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
      WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
      WaterSpeed = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 12) & 0xff) + (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 8) & 0xf) / 10.0);
      
      if (WaterSpeed > WaterSpeedMax)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"speed", 1, (uint8_t*)SalveIDStrBuf);
      }
      else if (WaterSpeed < WaterSpeedMin)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)SalveIDStrBuf);
      }
      else 
//        statue = SendWarmingPack((uint8_t*)"speed", 0, (uint8_t*)SalveIDStrBuf);
        statue = SUCCESS;
      
      if (statue == SUCCESS)
      {
        WarmUpCount[i][2] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 23);   
      }
      else
      {
        return FAILUED;
      }
    }
    
    /* 板温检测 */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 20) & 0x03) == 0x01)
    {
      EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
      EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
      EnvirTemp = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 24) & 0xff) + (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 20) & 0xf) / 10.0);
      
      if (EnvirTemp > EnvirTempMax)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"cputemp", 1, (uint8_t*)SalveIDStrBuf);
      }
      else if (EnvirTemp < EnvirTempMin)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"cputemp", -1, (uint8_t*)SalveIDStrBuf);
      }
      else 
//        statue = SendWarmingPack((uint8_t*)"cputemp", 0, (uint8_t*)SalveIDStrBuf);
        statue = SUCCESS;
      
      if (statue == SUCCESS)
      {
        WarmUpCount[i][0] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 21);   
      }
      else
      {
        return FAILUED;
      }
    }
    
    /* PH检测 */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 5) & 0x03) == 0x01)
    {
      PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
      PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
      PH = (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
            
      if (PH > PHMax)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"ph", 1, (uint8_t*)SalveIDStrBuf);
      }
      else if (PH < PHMin)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"ph", -1, (uint8_t*)SalveIDStrBuf);
      }
      else 
//        statue = SendWarmingPack((uint8_t*)"ph", 0, (uint8_t*)SalveIDStrBuf);
        statue = SUCCESS;
      if (statue == SUCCESS)
      {
        WarmUpCount[i][4] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 2] |= (1 << 6);  
      }
      else
      {
        return FAILUED;
      }
    }
    
    /* 水温检测 */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 3) & 0x03) == 0x01)
    {
      WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
//      WaterTempMin = (Flash_FifoBuf[1] & 0xf << 4) + ((Flash_FifoBuf[2] >> 28) & 0xf) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0);
      WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
      WaterTemp = (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
      
      if (WaterTemp > WaterTempMax)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"sensortemp", 1, (uint8_t*)SalveIDStrBuf);
      }
      else if (WaterTemp < WaterTempMin)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"sensortemp", -1, (uint8_t*)SalveIDStrBuf);
      }
      else 
//        statue = SendWarmingPack((uint8_t*)"sensortemp", 0, (uint8_t*)SalveIDStrBuf);
        statue = SUCCESS;
      
      if (statue == SUCCESS)
      {
        WarmUpCount[i][1] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 2] |= (1 << 4);  
      }
      else
      {
        return FAILUED;
      }
    }
  }
  
  if (WarmCount > 0)
  {
    do
    {
      statue = UpLoadDat_TCP();
      ErrorCount++;
      if (ErrorCount > 10)
      {
        ErrorCount = 0;
        TCPIsUsed = 0;
        GPRS_Rest();
        TCPIsUsed = 1;
      }
    }while(statue != SUCCESS);
    
  }
  
  return SUCCESS;
}


/*
 * 函数名：Bool2String
 * 描述  ：bool转换为string
 * 输入  ：无
 * 返回  ：无
 */
void Bool2String (int num, uint8_t* string)
{
  if (num)
  {
    strcpy((char*)string, "true");
  }
  else
  {
    strcpy((char*)string, "false");
  }
}

/*
 * 函数名：StrCpy
 * 描述  ：复制字符串
 * 输入  ：Src：    源
 *         Dest：   目标
 *         Length： 要复制的长度
 * 返回  ：无
 */
void StrCpy (char* Src, char* Dest, int Length)
{
  int i = 0;
  for (i = 0; i < Length; i++)
  {
    *Dest = *Src;
    Dest++;
    Src++;
  }
  *Dest = '\0';
}

/*
 * 函数名：StringFloat
 * 描述  ：字符串转换为float
 * 输入  ：num：    要转换的float
 *         String： 转换成功的string
 * 返回  ：无
 */
void StringFloat (float* num, char String[])
{
  char length = 0;
  length = strlen(String);
  
  switch (length)
  {
    case 3:
      String[0] -= 0x30;
      String[2] -= 0x30;
      *num = String[0] + (String[2] / 10.0);
      break;
    case 4:
      String[0] -= 0x30;
      String[1] -= 0x30;
      String[3] -= 0x30;
      *num = String[0] * 10 + String[1] + (String[3] / 10.0);
      break;
    default:
      break;
  }
  
}

/*
 * 函数名：BufClear
 * 描述  ：清空A6串口接收Buf
 * 输入  ：无
 * 返回  ：无
 */
void BufClear (void)
{
  int i =0;
  for (i = 0; i < 299; i++)
  {
    DatFromServer[i] = 0x00;
  }
  
}

/*
 * 函数名：GPRS_ReceMsgProcess
 * 描述  ：A6串口接收处理函数，截取阈值
 * 输入  ：无
 * 返回  ：无
 */
void GPRS_ReceMsgProcess (void)
{
  uint8_t i = 0;
  char* TempPosition = 0;
  char* position = 0;
  char Length = 0;
  
  char DevID_String[25];            //ID
  char PHMin_String[5];             //PH
  char WaterTempMin_String[5];      //水温最小值
  char WaterTempMax_String[5];      //水温最小值
  char EnvirTempMin_String[5];      //板温最小值
  char EnvirTempMax_String[5];      //板温最小值
  char WaterSpeedMin_String[5];     //最小水速
  char WaterSpeedMax_String[5];     //最小水速
  char PHMax_String[5];
  
  float DevID_Float;
  float WaterSpeedMax_float;   
  float PHMin_Float;
  float WaterTempMin_Float;
  float EnvirTempMin_Flaot;
  float WaterSpeedMin_Float;
  float PHMax_Float;
  float WaterTempMax_Float;
  float EnvirTempMax_Float;
  float WaterSpeedMax_Float;
  
  int TempInt;
  int TempFloat;
  
 
  if (strstr(DatFromServer, "minph") != NULL)         //如果是服务器向主机发送的与阈值有关的包
  {
    position = (strstr(DatFromServer, "did") + 6);
    StrCpy(position, DevID_String, 24);               //获取到ID
    if (strstr(DevID_String, ChipID_String) != NULL)
    {
      /* PH下限 */
      position = (strstr(DatFromServer, "minph") + 7);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, PHMin_String, Length);         //获取PH
      StringFloat(&PHMin_Float, PHMin_String);        //转换
      TempInt = (int)PHMin_Float;
      TempFloat = ((int)(PHMin_Float * 10) % 10);
 
      if ((PHMin_Float >= PH_Min) && (PHMin_Float <= PH_Max))
      {
        Flash_FifoBuf[1] &= ~(0xff << 16);
        Flash_FifoBuf[1] |= ((TempInt & 0xf) << 20);    //下限
        Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 16);  
      }     
      
      /* 水温下限 */
      position = (strstr(DatFromServer, "minsensortemp") + 15);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, WaterTempMin_String, Length);            //获取水温
      StringFloat(&WaterTempMin_Float, WaterTempMin_String);    //转换
      TempInt = (int)WaterTempMin_Float;
      TempFloat = ((int)(WaterTempMin_Float * 10) % 10);
      
      if ((WaterTempMin_Float >= WaterTmp_Min) && (WaterTempMin_Float <= WaterTmp_Max))
      {
        Flash_FifoBuf[1] &= ~(0xf);
        Flash_FifoBuf[1] |= ((TempInt >> 4)& 0xf);                //下限设置
        Flash_FifoBuf[2] &= ~(0xff << 24);
        Flash_FifoBuf[2] |= ((TempInt & 0xf) << 28);
        Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 24);
      }
      /* ************************************* */
      
      /* 板温下限 */    
      position = (strstr(DatFromServer, "mincputemp") + 12);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, EnvirTempMin_String, Length);            //获取板温
      StringFloat(&EnvirTempMin_Flaot, EnvirTempMin_String);    //转换
      TempInt = (int)EnvirTempMin_Flaot;
      TempFloat = ((int)(EnvirTempMin_Flaot * 10) % 10);
      
      if ((EnvirTempMin_Flaot >= EnvirTemp_Min) && (EnvirTempMin_Flaot <= EnvirTemp_Max))
      {
        Flash_FifoBuf[2] &= ~(0xff << 4);
        Flash_FifoBuf[2] |= ((TempInt & 0xff) << 4);              //下限设置
        Flash_FifoBuf[2] &= ~(0xf << 0);
        Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 0);
      }
      /* ************************************* */
      
      /* 水速下限 */
      position = (strstr(DatFromServer, "minspeed") + 10);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, WaterSpeedMin_String, Length);         
      StringFloat(&WaterSpeedMin_Float, WaterSpeedMin_String); 
      TempInt = (int)WaterSpeedMin_Float;
      TempFloat = ((int)(WaterSpeedMin_Float * 10) % 10);
      
      if ((WaterSpeedMin_Float >= WaterSpeed_Min) && (WaterSpeedMin_Float <= WaterSpeed_Max))
      {
        Flash_FifoBuf[3] &= ~(0xff << 12);
        Flash_FifoBuf[3] |= ((TempInt & 0xff) << 12);             //下限
        Flash_FifoBuf[3] &= ~(0xf << 8);
        Flash_FifoBuf[3] |= ((TempFloat & 0xf) << 8);
      }      
      /* ************************************* */
      
      /* PH上限 */
      position = (strstr(DatFromServer, "maxph") + 7);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, PHMax_String, Length);                  
      StringFloat(&PHMax_Float, PHMax_String);                 
      TempInt = (int)PHMax_Float;
      TempFloat = ((int)(PHMax_Float * 10) % 10);
      
      if ((PHMax_Float >= PH_Min) && (PHMax_Float <= PH_Max))
      {
        Flash_FifoBuf[1] &= ~(0xff << 24);
        Flash_FifoBuf[1] |= ((TempInt & 0xf) << 28);              //上限
        Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 24);
      }
      /* ************************************* */

      /* 水温上限 */
      position = (strstr(DatFromServer, "maxsensortemp") + 15);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, WaterTempMax_String, Length);           
      StringFloat(&WaterTempMax_Float, WaterTempMax_String);    
      TempInt = (int)WaterTempMax_Float;
      TempFloat = ((int)(WaterTempMax_Float * 10) % 10);
      
      if ((WaterTempMax_Float >= WaterTmp_Min) && (WaterTempMax_Float <= WaterTmp_Max))
      {
        Flash_FifoBuf[1] &= ~(0xff << 8);
        Flash_FifoBuf[1] |= ((TempInt & 0xff) << 8);              //上限设置        
        Flash_FifoBuf[1] &= ~(0xf << 4);
        Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 4);
      }
      /* ************************************* */

      /* 板温上限 */
      position = (strstr(DatFromServer, "maxcputemp") + 12);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, EnvirTempMax_String, Length);           
      StringFloat(&EnvirTempMax_Float, EnvirTempMax_String);    //转换
      TempInt = (int)EnvirTempMax_Float;
      TempFloat = ((int)(EnvirTempMax_Float * 10) % 10);
      
      if ((EnvirTempMax_Float >= EnvirTemp_Min) && (EnvirTempMax_Float <= EnvirTemp_Max))
      {
        Flash_FifoBuf[2] &= ~(0xff << 16);
        Flash_FifoBuf[2] |= ((TempInt & 0xff) << 16);             //上限设置
        Flash_FifoBuf[2] &= ~(0xf << 12);
        Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 12);
      }      
      /* ************************************* */
      
      /* 水速上限 */
      position = (strstr(DatFromServer, "maxspeed") + 10);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, WaterSpeedMax_String, Length);          
      StringFloat(&WaterSpeedMax_Float, WaterSpeedMax_String); 
      TempInt = (int)WaterSpeedMax_Float;
      TempFloat = ((int)(WaterSpeedMax_Float * 10) % 10);
      
      if ((WaterSpeedMax_Float >= WaterSpeed_Min) && (WaterSpeedMax_Float <= WaterSpeed_Max))
      {
        Flash_FifoBuf[3] &= ~(0xff << 24);
        Flash_FifoBuf[3] |= ((TempInt & 0xff) << 24);             //上限设置
        Flash_FifoBuf[3] &= ~(0xf << 20);
        Flash_FifoBuf[3] |= ((TempFloat & 0xf) << 20);
      }   
      
      /* 主机报警标志位清空 */
      Flash_FifoBuf[5] &= ~(0x3 << 23);   //流速
      Flash_FifoBuf[5] &= ~(0x3 << 19);   //板温
      Flash_FifoBuf[5] &= ~(0x3 << 17);   //PH
      Flash_FifoBuf[5] &= ~(0x3 << 15);   //水温
      WarmUpCount[0][0] = 0;
      WarmUpCount[0][1] = 0;
      WarmUpCount[0][2] = 0;
      WarmUpCount[0][3] = 0;
      WarmUpCount[0][4] = 0;
      
      /* 从机报警标志位清空 */
      for (i = 1; i <= Flash_FifoBuf[0]; i++)
      {
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] &= ~(0x3 << 23);	    //流速
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] &= ~(0x3 << 21);	    //板温
        Flash_FifoBuf[7 + (i - 1) * 3 + 2] &= ~(0x3 << 6);  	  //PH
        Flash_FifoBuf[7 + (i - 1) * 3 + 2] &= ~(0x3); 	        //水温
        WarmUpCount[i][0] = 0;
        WarmUpCount[i][1] = 0;
        WarmUpCount[i][2] = 0;
        WarmUpCount[i][3] = 0;
        WarmUpCount[i][4] = 0;
        WarmUpCount[i][5] = 0;
      }
      
      //GoLimPage();
    }
    BufClear();
  }
  
}


/*
 * 函数名：GPRS_Init
 * 描述  ：TCP连接服务器初始化
 * 输入  ：无
 * 返回  ：无
 */
void GPRS_Rest (void)
{
  uint8_t statue = DefaultValue;
  
  /* 初始化TCP前需要确保当前没有任务在使用TCP */
  if (TCPIsUsed == 0)
  {
    TCPIsUsed = 1;
    do
    {
      statue = GRPS_NetInit();    //GPRS初始化
      switch(statue)
      {
        
        case NoResponse:          //GRPS无响应
          InitDis(" GRPS无响应 ");
          break;
        case SUCCESS:             //初始化成功
          GetNet = 1;
          break;
        case NoSIMCard:           //未检测到SD卡
          InitDis(" 无SIM卡 ");
          while(1);
        case GRPSGetFailue:       //GPRS附着失败
          InitDis(" GPRS附着失败");
          break;
        case PDPGetFailue:        //PDP激活失败
          InitDis(" PDP激活失败，正在重试 ");
          break;
        case ServeNoResponse:     //连接服务器时没有反馈
          InitDis(" 服务器无响应，正在重试 ");
          break;    
        case GetTCPFailue:
          InitDis(" 无法与服务器连接，正在重试 ");
          break;
        default:
          InitDis(" 出现未知错误，正在重试 ");
          break;
      }

    }while(statue != SUCCESS);    //如果不成功死循环不断尝试
    TCPIsUsed = 0;                //成功后清空TCP被占用标志位
  }
}

/*
 * 函数名：SendMsg2Server
 * 描述  ：向服务器发送数据，此函数被其他函数调用，一般不直接使用
 * 输入  ： SendBuf:发送的包
 *          CheckBuf：判断是否有回包的关键字数组
 * 返回  ： SUCCESS: 成功
 *          else:    失败
 */
uint8_t SendMsg2Server (uint8_t* SendBuf, char* CheckBuf)
{
  uint8_t ErrorCount1 = 0;    //错误计数1
  uint8_t ErrorCount2 = 0;    //错误计数2
  uint8_t statue = 0;
  
  Uart3_BufClear();  
  
  do                   
  {
    USART3_SendBuf(SendBuf);    
    ErrorCount1 = 0; 
    ErrorCount2 = 0;
    while(ErrorCount1 < 100)   //在10s内查看是否返回成功状态
    {
      UserDelay_KeyScan(100);
      ErrorCount1++;
      if (strstr(Uart3_ReceiveDat, CheckBuf) != NULL)
        break;
      if (strstr(Uart3_ReceiveDat, "ERROR") != NULL)        //如果发送数据失败
        return LocalFault;
    }
    ErrorCount2++;
    if (ErrorCount2 > 2)      //总共等待30s
      return NoResponse;
   }while(strstr(Uart3_ReceiveDat, CheckBuf) == NULL);
  
  UserDelay_KeyScan(200);
   
  return SUCCESS;             //执行这条指令说明前面一切顺利
}

/*
 * 函数名：GRPS_NetInit
 * 描述  ：GPRS网络初始化
 * 输入  ：无
 * 返回  ：无
 */
uint8_t GRPS_NetInit (void)
{
  uint8_t i, statue = 0;
  int ErrorCount = 0;         //错误计数
  int Delay_TimeCount = 0;    
  uint8_t MsgSendBuf[300];    //发送的包
  uint8_t MsgLengthBuf[15];   //包长度包
  uint8_t SalveIDStrBuf[9];   //从机设备ID包
  int MsgLength = 0;          //包长度
  uint8_t getNetTimeStatue = SUCCESS;
  /* 复位 */
  A6_Rst(OFF);
  UserDelay_KeyScan(200);
  A6_Rst(ON);
  
  UserDelay_KeyScan(10000);      //延时10s  暂时先将延时放在这里，还要移到调用它的地方去   延时是为了GPRS充分启动完毕
  
  statue = SendMsg2Server((uint8_t *)"AT\r\n", "OK");
  if (statue != SUCCESS)
    return NoResponse;

  /* 检测有无SIM卡 */
  InitDis(" 正在检测SIM卡 ");
  statue = SendMsg2Server((uint8_t *)"AT+CCID\r\n", "SIM Card ID");
  if (statue != SUCCESS)
    return NoSIMCard;
  InitDis(" 检测到SIM卡 ");
  
  /* GPRS附着 */
  InitDis(" 正在附着GPRS ");
  UserDelay_KeyScan(3000);
  statue = SendMsg2Server((uint8_t *)"AT+CGATT=1\r\n", "OK");
  if (statue != SUCCESS)
    return GRPSGetFailue;
  InitDis(" GPRS附着成功 ");
  
  /* PDP激活 */
  InitDis(" 正在PDP激活 ");
  UserDelay_KeyScan(5000);
  statue = SendMsg2Server((uint8_t *)"AT+CGACT=1,1\r\n", "OK");
  if (statue != SUCCESS)
    return PDPGetFailue;
  InitDis(" PDP激活成功 ");
  
  /* 检测当前是否已连接TCP */
  
  UserDelay_KeyScan(2000);
  statue = SendMsg2Server((uint8_t *)"AT+CIPSTATUS\r\n", "+IPSTATUS");
  if (statue != SUCCESS)
    return statue;
  
//  InitDis(" 正在获取网络时间 ");
//  ErrorCount = 0;
//  Uart3_BufClear();
//  /* 连接TCP */
//  USART3_SendBuf("at+cipstart=\"TCP\",\"132.163.4.102\",13\r\n");  
//  while((strstr(Uart3_ReceiveDat, "UTC(NIST)") == NULL) && getNetTimeStatue == SUCCESS)
//  {
//    UserDelay_KeyScan(20);
//    ErrorCount++;
//    if (ErrorCount > 1000)
//      getNetTimeStatue = FAILUED;
//  }
//  if (getNetTimeStatue == SUCCESS)
//    GetNetTime();
//  else
//  {
//    calendar.w_year = 2016;
//    calendar.w_month = 6;
//    calendar.w_date = 15;
//    calendar.hour = 12;
//    calendar.min = 30;
//    calendar.sec = 30;
//    InitDis(" 网络时间获取失败，请开机后手动设置");
//  }
//    
//  Delay_ms(1000);
  
//  UserDelay_KeyScan(100);  
//  statue = SendMsg2Server("AT+CIPCLOSE\r\n", "OK");
//  if (statue != SUCCESS)
//    return statue;
  
  InitDis(" 正在连接TCP ");
  ErrorCount = 0;
  Uart3_BufClear();
  /* 连接TCP */
  USART3_SendBuf("at+cipstart=\"TCP\",\"115.28.72.3\",4040\r\n");  
  while(strstr(Uart3_ReceiveDat, "CIPRCV") == NULL)
  {
    UserDelay_KeyScan(20);
    ErrorCount++;
    if (ErrorCount > 1000)
      return GetTCPFailue;
  }
  InitDis(" TCP连接成功 ");
  
  InitDis(" 正在挂接设备 ");
  sprintf((char*)MsgSendBuf, "{\"type\":\"did\",\"did\":\"%s\"}\r\n", ChipID_String);
  MsgLength = strlen((char*)MsgSendBuf);     
  sprintf((char*)MsgLengthBuf, "AT+CIPSEND=%d\r\n", MsgLength);     //将长度融合进语句
  /* 设定第一条语句的发送数据包长度 */
  
  statue = SendMsg2Server((uint8_t *)MsgLengthBuf, ">");
  if (statue != SUCCESS)
    return statue;
  
  /* 访问服务器，发送连接信息 */
  statue = SendMsg2Server((uint8_t *)MsgSendBuf, ChipID_String);
  if (statue != SUCCESS)
    return statue;
  
  
  /* 如果未配对以及未向服务器申请 */
  if ((((Flash_FifoBuf[5] >> 31) & 0x01) == 0) && (((Flash_FifoBuf[5] >> 30) & 0x01) == 0))
  {
      /* 设定下一条语句长度 */
    sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", ChipID_String);   //先合并数据帧语句
    MsgLength = strlen((char*)MsgSendBuf);                                                //就算数据帧语句长度
    sprintf((char*)MsgLengthBuf, "AT+CIPSEND=%d\r\n", MsgLength);     //将长度融合进语句
    
    statue = SendMsg2Server((uint8_t *)MsgLengthBuf, ">");
    if (statue != SUCCESS)
      return statue;
    
    /* 在服务器中添加本主机设备 */
    /* 如果数据库中已经还没有，则添加，如果有，就不添加 */
    ErrorCount = 0;
    do
    {
      UserDelay_KeyScan(3000);
      Uart3_BufClear();  
      USART3_SendBuf(MsgSendBuf);  
      Delay_TimeCount = 0;
      while(Delay_TimeCount < 10)
      {
        UserDelay_KeyScan(100);
        Delay_TimeCount++;
        if ((strstr(Uart3_ReceiveDat, ChipID_String) != NULL) || (strstr(Uart3_ReceiveDat, "repeat") != NULL))
        {
          Flash_FifoBuf[5] |= (1 << 30);    //已申请
          Flash_FifoBuf[5] |= (1 << 31);    //已配对          
          break;
        }
      }
      ErrorCount++;
      if (ErrorCount > 10)
        return ServeNoResponse;
    }while((strstr(Uart3_ReceiveDat, ChipID_String) == NULL) && (strstr(Uart3_ReceiveDat, "repeat") == NULL));
  }
  
  /* 在服务器中添加从机设备 */
  for (i = 1; i <= Flash_FifoBuf[0]; i++)
  {
    if((((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 31) & 0x01) == 0) &&           //设备未配对且未向服务器发送申请
         (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 30) & 0x01) == 0))
    {
      Chip2String(Flash_FifoBuf[7 + (i - 1) * 3], (char*)SalveIDStrBuf);
      sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SalveIDStrBuf);   //先合并数据帧语句
      MsgLength = strlen((char*)MsgSendBuf);    
      sprintf((char*)MsgLengthBuf, "AT+CIPSEND=%d\r\n", MsgLength);     //将长度融合进语句
      
      do
      {
        UserDelay_KeyScan(1000);
        Uart3_BufClear();  
        USART3_SendBuf(MsgLengthBuf);  
        Delay_TimeCount = 0;
        while(Delay_TimeCount < 30)
        {
          UserDelay_KeyScan(100);
          Delay_TimeCount++;
          if (strstr(Uart3_ReceiveDat, ">") != NULL)
            break;
        }
      }while(strstr(Uart3_ReceiveDat, ">") == NULL);
      
      ErrorCount = 0;
      do
      {
        UserDelay_KeyScan(100);
        Uart3_BufClear();  
        USART3_SendBuf(MsgSendBuf);  
        Delay_TimeCount = 0;
        while(Delay_TimeCount < 10)
        {
          UserDelay_KeyScan(100);
          Delay_TimeCount++;
          if ((strstr(Uart3_ReceiveDat, (char*)SalveIDStrBuf) || strstr(Uart3_ReceiveDat, "repeat")) != NULL)
          {
            Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 30);    //已申请
            Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 31);    //已配对          
            break;
          }
        }
        ErrorCount++;
        if (ErrorCount > 10)
          return ErrorFromServe;
      }while((strstr(Uart3_ReceiveDat, (char*)SalveIDStrBuf) || strstr(Uart3_ReceiveDat, "repeat")) == NULL);
      
      Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 30);     //已经向服务器发送申请
    }
  }
  
  statue = GetOffsetDat();
  if (statue != SUCCESS)
      return statue;
  InitDis(" 补偿接收完成 ");
  
  return SUCCESS;  
}

/*
 * 函数名：AddSlaveID2Server
 * 描述  ：向服务器添加一个从机
 * 输入  ： SlaveID:从机ID
 *          SlaveSerial：从机在本地的序号
 * 返回  ： SUCCESS: 成功
 *          else:    失败
 */
uint8_t AddSlaveID2Server (long SlaveID, uint8_t SlaveSerial)
{
  uint8_t statue = 0;
  uint8_t SalveIDStrBuf[9];
  uint8_t MsgLength;
  uint8_t MsgSendBuf[100];
  uint16_t Delay_TimeCount = 0, ErrorCount = 0; 
  
  Chip2String(SlaveID, (char*)SalveIDStrBuf);
  sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SalveIDStrBuf);   //先合并数据帧语句
  MsgLength = strlen((char*)MsgSendBuf);   
  
  statue = ReqSendTcp(MsgLength);
  if (statue != SUCCESS)
      return statue;
  
  statue = SendMsg2Server((uint8_t *)MsgSendBuf, (char*)SalveIDStrBuf);
  
  if (statue != SUCCESS)
    return ErrorFromServe;
  
//  ErrorCount = 0;
//  do
//  {
//    UserDelay_KeyScan100);
//    Uart3_BufClear();  
//    USART3_SendBuf(MsgSendBuf);  
//    Delay_TimeCount = 0;
//    while(Delay_TimeCount < 10)
//    {
//      UserDelay_KeyScan100);
//      Delay_TimeCount++;
//      if (strstr(Uart3_ReceiveDat, (char*)SalveIDStrBuf) != NULL)
//        break;
//    }
//    ErrorCount++;
//    if (ErrorCount > 10)
//      return ErrorFromServe;
//  }while(strstr(Uart3_ReceiveDat, (char*)SalveIDStrBuf) == NULL);
  
  Flash_FifoBuf[SlaveSerial + 1] |= (1 << 30);    //已申请
  Flash_FifoBuf[SlaveSerial + 1] |= (1 << 31);    //已配对        
  
  return SUCCESS;
}

/*
 * 函数名：UpSlaveLost2Server
 * 描述  ：向服务器上报一个从机丢失
 * 输入  ： SlaveChipIDString:从机ID 字符串形式
 * 返回  ： SUCCESS: 成功
 *          else:    失败
 */
uint8_t UpSlaveLost2Server (uint8_t* SlaveChipIDString)
{
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  uint8_t MsgSendBuf[300];
  sprintf((char*)MsgSendBuf, "{\"type\":\"lost\",\"lost\":\"%s\"}\r\n", SlaveChipIDString);
  
  do
  {
    UserDelay_KeyScan(1000);
    Uart3_BufClear();  
    USART3_SendBuf(MsgSendBuf);  
    TimeDelayCount = 0;
    while(TimeDelayCount < 40)
    {
      UserDelay_KeyScan(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, "true") != NULL)
        return SUCCESS;  
    } 
    ErrorCount++;
    if (ErrorCount > 3)
    {
      return ErrorFromServe;
    }
  }while(strstr(Uart3_ReceiveDat, "true") == NULL);
  return SUCCESS;  
}

/*
 * 函数名：AddSlaveDev2Server
 * 描述  ：向服务器添加从机
 * 输入  ：FlashPos：从机在Flash中的申请标识位
 *         SlaveChipIDString： 从机ID
 * 返回  ：SUCCESS： 成功   其他：失败
 */
uint8_t AddSlaveDev2Server (uint8_t FlashPos, uint8_t* SlaveChipIDString)
{
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  uint8_t MsgSendBuf[300];
  sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SlaveChipIDString);
  
  do
  {
    UserDelay_KeyScan(1000);
    Uart3_BufClear();  
    USART3_SendBuf(MsgSendBuf);  
    TimeDelayCount = 0;
    while(TimeDelayCount < 10)
    {
      UserDelay_KeyScan(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, (char*)SlaveChipIDString) != NULL)
      {
        Flash_FifoBuf[FlashPos] |= (1 << 30);    //已申请
        Flash_FifoBuf[FlashPos] |= (1 << 31);    //已配对
        break;
      }
    } 
    ErrorCount++;
    if (ErrorCount > 10)
    {
      return ErrorFromServe;
    }
  }while(strstr(Uart3_ReceiveDat, (char*)SlaveChipIDString) == NULL);
  return SUCCESS;  
}

/*
 * 函数名：ReqSendTcp
 * 描述  ：向A6请求发送TCP协议
 * 输入  ：length：要发送的协议的长度
 * 返回  ：SUCCESS： 成功   其他：失败
 */
uint8_t ReqSendTcp (uint8_t length)
{
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  uint8_t MsgLengthBuf[20];
  sprintf((char*)MsgLengthBuf, "AT+CIPSEND=%d\r\n", length);     //将长度融合进语句
  
  ErrorCount = 0;
  do
  {
    UserDelay_KeyScan(1000);
    Uart3_BufClear();  
    USART3_SendBuf(MsgLengthBuf);  
    TimeDelayCount = 0;
    while(TimeDelayCount < 30)
    {
      UserDelay_KeyScan(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, ">") != NULL)
        break;
    }      
    ErrorCount++;
    if (ErrorCount > 10)
    {
      return GRPSGetFailue;
    }
  }while(strstr(Uart3_ReceiveDat, ">") == NULL);
  return SUCCESS;
}

/*
 * 函数名：SendDatTCP2Server_Ready
 * 描述  ：发送数据包前的数据标识包
 * 输入  ：无
 * 返回  ：SUCCESS： 成功   其他：失败
 */
uint8_t SendDatTCP2Server_Ready (uint8_t MsgLength)
{
  uint8_t statue = 0;
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  
  do
  {
    Uart3_BufClear();  
    statue = ReqSendTcp(MsgLength);   //请求发送TCP
    if (statue != SUCCESS)
      return statue;
    UserDelay_KeyScan(500);
    USART3_SendBuf((uint8_t *)"{\"type\":\"data\",\"data\":\"message\"}\r\n");  
    TimeDelayCount = 0;
    while(TimeDelayCount < 30)
    {
      UserDelay_KeyScan(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, "true") != NULL)
        break;
    }
    ErrorCount++;
    if (ErrorCount > 10)
    {
      return ServeNoResponse;
    }
  }while(strstr(Uart3_ReceiveDat, "true") == NULL);
  return SUCCESS;
}

/*
 * 函数名：SendDat2Server
 * 描述  ：发送采集到的数据到服务器
 * 输入  ：MsgSendBuf：要发送的包
 * 返回  ：SUCCESS： 成功   其他：失败
 */
uint8_t SendDat2Server (uint8_t* MsgSendBuf)
{
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  uint8_t MsgLength, MsgLength1;
  uint8_t statue = 0;
  
  MsgLength = strlen((char*)MsgSendBuf);     //就算数据帧语句长度
  
  do
  {
    statue = ReqSendTcp(MsgLength);
    if (statue != SUCCESS)
      return statue;
    UserDelay_KeyScan(500);
    Uart3_BufClear();  
    USART3_SendBuf(MsgSendBuf);   //发送采集到的数据信息
    TimeDelayCount = 0;
    ErrorCount = 0;
    while(TimeDelayCount < 30)
    {
      UserDelay_KeyScan(200);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, "{\"type\":\"data\",\"data\":true}") != NULL)    //检测到返回正确
        break;
      if (strstr(Uart3_ReceiveDat, "\"format\":false") != NULL)         //检测到格式错误
      {    
        /* 设定将发送的语句长度 */
        MsgLength1 = strlen("{\"type\":\"data\",\"data\":\"message\"}\r\n");   //计算数据帧语句长度
        
        statue = SendDatTCP2Server_Ready(MsgLength1); //发送数据起始包
        if (statue != SUCCESS)
          return statue;        
        break;
      }
    }    
    ErrorCount++;
    if (ErrorCount > 3)
    {
      return ServeNoResponse;
    }
  }while(strstr(Uart3_ReceiveDat, "{\"type\":\"data\",\"data\":true}") == NULL);
  return SUCCESS;
}

/*
 * 函数名：UpOneSlaveDat2Server
 * 描述  ：向服务器上报一个从机信息
 * 输入  ： SlaveNum:从机在本地的序号
 * 返回  ： SUCCESS: 成功
 *          else:    失败
 */
uint8_t UpOneSlaveDat2Server (uint8_t SlaveNum)
{
  uint8_t statue = DefaultValue;
  int MsgLength = 0;
    
  uint8_t WaterLevel = 0;   //水位
  uint8_t PowerDown = 0;    //掉电
  float WaterSpeed = 0.0;   //流速
  float EnvirTemp = 0.0;    //板温
  float ph = 0.0;           //ph
  float WaterTemp = 0.0;    //水温
  
  uint8_t WaterLevelString[5]; 
  uint8_t SlaveChipIDString[15];
  uint8_t MsgSendBuf[300];
  uint8_t PowerDownString[5];  
  
  PowerDown = ((Flash_FifoBuf[5] >> 20) & 0x01);  
  WaterLevel = ((Flash_FifoBuf[SlaveNum + 2] >> 7) & 0x01);
  Bool2String(WaterLevel, WaterLevelString);
  WaterSpeed = ((Flash_FifoBuf[SlaveNum + 2] >> 12) & 0xff) + (((Flash_FifoBuf[SlaveNum + 2] >> 8) & 0xf) / 10.0);
  EnvirTemp = ((Flash_FifoBuf[SlaveNum + 2] >> 24) & 0xff) + (((Flash_FifoBuf[SlaveNum + 2] >> 20) & 0xf) / 10.0);
  ph = (((Flash_FifoBuf[SlaveNum + 1] >> 16) & 0xf) + ((Flash_FifoBuf[SlaveNum + 1] >> 12) & 0xf) / 10.0);
  WaterTemp = (((Flash_FifoBuf[SlaveNum + 1] >> 4) & 0xff) + ((Flash_FifoBuf[SlaveNum + 1] >> 0) & 0xf) / 10.0);
  
  Chip2String(Flash_FifoBuf[SlaveNum], (char*)SlaveChipIDString);
  Bool2String(!PowerDown, PowerDownString);   
  
//  if((((Flash_FifoBuf[SlaveNum + 1] >> 31) & 0x01) == 0) &&           //设备未配对且未向服务器发送申请
//         (((Flash_FifoBuf[SlaveNum + 1] >> 30) & 0x01) == 0))
//  {
//    sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SlaveChipIDString);   //先合并数据帧语句
//    MsgLength = strlen((char*)MsgSendBuf);    
//    
//    /* 向A6申请发送TCP消息 */
//    statue = ReqSendTcp(MsgLength);
//    if (statue != SUCCESS)
//      return statue;

//    /* 发送添加从机包 */
//    statue = AddSlaveDev2Server(SlaveNum + 1, SlaveChipIDString);
//    if (statue != SUCCESS)
//      return statue;
//  }
  MsgLength = strlen("{\"type\":\"data\",\"data\":\"message\"}\r\n");
//  /* 要先通知服务器接下来要发送采集的数据 */
//  statue = ReqSendTcp(MsgLength);
//  if (statue != SUCCESS)
//    return statue;
  
  /* 发送数据包前包 */
  statue = SendDatTCP2Server_Ready(MsgLength);
  if (statue != SUCCESS)
    return statue;
  
  sprintf ((char*)MsgSendBuf, "{\"did\":\"%s\",\"ph\":%.1f,\"cputemp\":%.1f,\"sensortemp\":%.1f,\"speed\":%.1f,\"water\":%s,\"power\":%s}\r\n",
              SlaveChipIDString, ph, EnvirTemp, WaterTemp, WaterSpeed, WaterLevelString, PowerDownString);
  
  statue = SendDat2Server(MsgSendBuf);   //发送从机数据
  if (statue != SUCCESS)
  return statue;
  
  return SUCCESS;
}

/*
 * 函数名：UpLoadDat_TCP
 * 描述  ：向服务器更新数据
 * 输入  ：无
 * 返回  ：无
 */
uint8_t UpLoadDat_TCP (void)
{
  /* 这里添加每半小时向服务器发送一次数据的代码 */
  uint8_t statue = DefaultValue;
  
  int i = 0, MsgLength = 0, ErrorCount = 0, ErrorCount1 = 0;
  int Delay_TimeCount = 0;
  uint8_t WaterLevel = 0;   //水位
  uint8_t PowerDown = 0;    //掉电
  float WaterSpeed = 0.0;   //流速
  float EnvirTemp = 0.0;    //板温
  float ph = 0.0;           //ph
  float WaterTemp = 0.0;    //水温
  
  /* 上面各项参数的字符串形式 */
  uint8_t WaterLevelString[5]; 
  uint8_t PowerDownString[5];  
  uint8_t SlaveChipIDString[15];
  uint8_t MsgSendBuf[300];
 
//  for (i = 1; i <= Flash_FifoBuf[0]; i++)
//  {
//    if((((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 31) & 0x01) == 0) &&           //设备未配对且未向服务器发送申请
//         (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 30) & 0x01) == 0))
//    {
//      Chip2String(Flash_FifoBuf[7 + (i - 1) * 3], (char*)SlaveChipIDString);
//      sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SlaveChipIDString);   //先合并数据帧语句
//      MsgLength = strlen((char*)MsgSendBuf);    
//      
//      /* 向A6申请发送TCP消息 */
//      statue = ReqSendTcp(MsgLength);
//      if (statue != SUCCESS)
//        return statue;

//      /* 发送添加从机包 */
//      statue = AddSlaveDev2Server(7 + (i - 1) * 3 + 1, SlaveChipIDString);
//      if (statue != SUCCESS)
//        return statue;
//    }
//  }

  /* 主机采集的数据发送 */
  WaterLevel = ((Flash_FifoBuf[5] >> 24) & 0x01);                                                                                 /* 水位 */
  Bool2String(!WaterLevel, WaterLevelString);
  PowerDown = ((Flash_FifoBuf[5] >> 20) & 0x01);                                                                                  /* 掉电 */
  Bool2String(!PowerDown, PowerDownString);     
  WaterSpeed = ((Flash_FifoBuf[6] >> 6) & 0xff) + (((Flash_FifoBuf[6] >> 2) & 0xf) / 10.0);                                       /* 流速 */
  EnvirTemp = ((Flash_FifoBuf[6] >> 18) & 0xff) + (((Flash_FifoBuf[6] >> 14) & 0xf) / 10.0);                                      /* 板稳 */
  ph = (((Flash_FifoBuf[5] >> 10) & 0xf) + ((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);                                               /* ph */
  WaterTemp = ((Flash_FifoBuf[5] & 0x1f) << 2) + ((Flash_FifoBuf[6] >> 30)& 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0);     /* 水温 */
  
  /* 设定将发送的语句长度 */
  MsgLength = strlen("{\"type\":\"data\",\"data\":\"message\"}\r\n");                                                //就算数据帧语句长度
  
  /* 要先通知服务器接下来要发送采集的数据 */
//  statue = ReqSendTcp(MsgLength);
//  if (statue != SUCCESS)
//    return statue;
  
  /* 发送数据包前包 */
  statue = SendDatTCP2Server_Ready(MsgLength);
  if (statue != SUCCESS)
    return statue;
                                 
  /* 合成主机数据上报给服务器 */
  sprintf ((char*)MsgSendBuf, "{\"did\":\"%s\",\"ph\":%.1f,\"cputemp\":%.1f,\"sensortemp\":%.1f,\"speed\":%.1f,\"water\":%s,\"power\":%s}\r\n",
      ChipID_String, ph, EnvirTemp, WaterTemp, WaterSpeed, WaterLevelString, PowerDownString);
  MsgLength = strlen((char*)MsgSendBuf);
  
  /* 发送主机数据 */
  statue = SendDat2Server(MsgSendBuf);  
  if (statue != SUCCESS)
    return statue;

    
  /* 将从机数据上报给服务器 */
  for (i = 1; i <= Flash_FifoBuf[0]; i++)
  {
    /* 获取相应的从机采集数据 */
    WaterLevel = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 7) & 0x01);
    Bool2String(!WaterLevel, WaterLevelString);
    WaterSpeed = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 12) & 0xff) + (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 8) & 0xf) / 10.0);
    EnvirTemp = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 24) & 0xff) + (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 20) & 0xf) / 10.0);
    ph = (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
    WaterTemp = (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
    Chip2String(Flash_FifoBuf[7 + (i - 1) * 3], (char*)SlaveChipIDString);
    
    /* 设定将发送的语句长度 */
    MsgLength = strlen("{\"type\":\"data\",\"data\":\"message\"}\r\n");
    
    /* 要先通知服务器接下来要发送采集的数据 */
//    statue = ReqSendTcp(MsgLength);
//    if (statue != SUCCESS)
//      return statue;
    
    /* 发送数据包前包 */
    statue = SendDatTCP2Server_Ready(MsgLength);
    if (statue != SUCCESS)
      return statue;
    
    sprintf ((char*)MsgSendBuf, "{\"did\":\"%s\",\"ph\":%.1f,\"cputemp\":%.1f,\"sensortemp\":%.1f,\"speed\":%.1f,\"water\":%s,\"power\":%s}\r\n",
                SlaveChipIDString, ph, EnvirTemp, WaterTemp, WaterSpeed, WaterLevelString, PowerDownString);
    
    statue = SendDat2Server(MsgSendBuf);   //发送从机数据
    if (statue != SUCCESS)
    return statue;
  }
    
  return SUCCESS;
}


/*
 * 函数名：GPRS_GPIO_Config
 * 描述  ：初始化A6的控制引脚
 * 输入  ：无
 * 返回  ：无
 */
void GPRS_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_A6_Rst | RCC_APB2Periph_GPIOx_A6_PWR, ENABLE); 
												   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_A6_Rst;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOx_A6_Rst, &GPIO_InitStructure);	
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_A6_PWR;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOx_A6_PWR, &GPIO_InitStructure);	

    A6_Rst(ON);     //复位脚拉高不复位
    A6_PWR(OFF);    //开机键拉低开机
}

