#include "common.h"

uint8_t Connect_Way;    // USE_CAN   USE_ZGB
uint8_t MsgSendStatue = SUCCESS;

u8 ZGB_Buf[14] = {0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
u8 CanBuf[8] = {0x0f, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

void Can_ReceMsgProcess (CanRxMsg* RxMessage)
{
  static int32_t TmpOffic = 0;
  static float PH_Acidity = 0.0;
  static float PH_Neutral = 0.0;
  static float PH_Basicity = 0.0;
  float DifTmp = 0.0;
//  printf ("接受到的数据: %d, %d, %d, %d\r\n", RxMessage->Data[1], RxMessage->Data[2], RxMessage->Data[3], RxMessage->Data[4]);
//  printf ("the chipid is: %d, %d, %d, %d\r\n", ChipID[0], ChipID[1], ChipID[2], ChipID[3]);
  /* 先确定是给自己发的消息 */
  if ((ChipID[0] == RxMessage->Data[4]) && (ChipID[1] == RxMessage->Data[3]) && \
      (ChipID[2] == RxMessage->Data[2]) && (ChipID[3] == RxMessage->Data[1]))
  {
    GetMasterNet_Statue = CheckLostTime;
//    printf ("the dat is to myself\r\n");
//    printf ("the [6] and [7] is: %d, %d\r\n", RxMessage->Data[6], RxMessage->Data[7]);
    
    /* 如果是申请入网有关的数据 */
    if ((RxMessage->Data[6] == 0xaa) && (RxMessage->Data[7] == 0xaa)) 
    {
      switch(RxMessage->Data[5])
      {
        case 0x01:    //申请已经接受，等待服务器确认
          {
//            printf ("wait checked by server\r\n");
            GetNet_Statue = Net_HasBeenAsked;
          }
          break;
        case 0x02:    //已经提交过申请，不再重复提交，等待服务器确认
          {
//            printf ("wait checked by server, will not sub again\r\n");
            GetNet_Statue = Net_HasBeenAsked;
          }
          break;
        case 0x03:
          {
//            printf ("Net Get Success\r\n");
            GetNet_Statue = Net_GetSuccess;
          }
          
        default:
          break;        
      }
    }
    if ((RxMessage->Data[6] == 0xbb) && (RxMessage->Data[7] == 0xbb))
    {
      switch(RxMessage->Data[5])
      {
        case 0x01:
          {
            Delay_ms(1000);
//            printf ("i want send msg\r\n");
            CanBuf[1] = ChipID[3];
            CanBuf[2] = ChipID[2];
            CanBuf[3] = ChipID[1];
            CanBuf[4] = ChipID[0];
            CanBuf[5] = ((Flash_FifoBuf[5] >> 6) & 0xff) + 1;      //PH存放
            CanBuf[6] = ((Flash_FifoBuf[5] & 0x1f) << 2) + ((Flash_FifoBuf[6] >> 30)& 0x3) + 1;       //水温整数
            CanBuf[7] = 0x01;
            Can_Send_Msg(CanBuf, 8);
            Delay_ms(500);
            
//            printf("the dat i send is %d, %d\r\n", CanBuf[5], CanBuf[6]);
            
            
            CanBuf[1] = ChipID[3];
            CanBuf[2] = ChipID[2];
            CanBuf[3] = ChipID[1];
            CanBuf[4] = ChipID[0];
            CanBuf[5] = ((Flash_FifoBuf[6] >> 18) & 0xff) + 1;       //板温整数
            CanBuf[6] = 0x00;     //先清零
            CanBuf[6] |= ((Flash_FifoBuf[6] >> 26) & 0xf) + 1;       //水温小数
            CanBuf[6] |= ((Flash_FifoBuf[6] >> 14) & 0xf) + 1;       //板温小数
            CanBuf[7] = 0x02;
            Can_Send_Msg(CanBuf, 8);
            Delay_ms(500);
            
//            printf("the dat i send is %d, %d\r\n", CanBuf[5], CanBuf[6]);
            
            CanBuf[1] = ChipID[3];
            CanBuf[2] = ChipID[2];
            CanBuf[3] = ChipID[1];
            CanBuf[4] = ChipID[0];
            CanBuf[5] = ((Flash_FifoBuf[6] >> 6) & 0xff) + 1;         //流速整数
            CanBuf[6] = 0x00;     //先清零
            CanBuf[6] |= (((Flash_FifoBuf[6] >> 2) & 0xf) << 5) + 1;  //流速小数
            CanBuf[6] |= (((Flash_FifoBuf[5] >> 24) & 0x01) << 3) + 1;     //水位
            CanBuf[7] = 0x03;
            Can_Send_Msg(CanBuf, 8);
            Delay_ms(500);
//            printf("the dat i send is %d, %d\r\n", CanBuf[5], CanBuf[6]);
          }
          break;
        default:
          break;
      }
    } 

    if (RxMessage->Data[7] == 0xc0) 
    {
      TmpOffic = 0x00;
      TmpOffic = (RxMessage->Data[5] << 24) + (RxMessage->Data[6] << 16);
    }
    if (RxMessage->Data[7] == 0xc1)
    {
      TmpOffic = (RxMessage->Data[5] << 8) + (RxMessage->Data[6]);
      DifTmp = (TmpOffic / 10.0);
      OfficDatBuf[0] = ((DifTmp + (OfficDatBuf[0] / 10.0)) * 10);
      
      CanBuf[1] = ChipID[3];
      CanBuf[2] = ChipID[2];
      CanBuf[3] = ChipID[1];
      CanBuf[4] = ChipID[0];
      CanBuf[5] = 0xff;
      CanBuf[6] = 0xff;
      CanBuf[7] = 0xcc;
      Can_Send_Msg(CanBuf, 8);
      
      Flash_WriteOfficDat();            //写入Flash
    }      
    
    CanBuf[1] = ChipID[3];
    CanBuf[2] = ChipID[2];
    CanBuf[3] = ChipID[1];
    CanBuf[4] = ChipID[0];
    CanBuf[5] = 0xff;
    CanBuf[6] = 0xff;
    
    switch (RxMessage->Data[7])
    {
      case 0xd0:
        PH_Neutral = GetPH();
        Delay_ms(1000);
        CanBuf[7] = 0xd0;
        Can_Send_Msg(CanBuf, 8);      
        break;
      case 0xd1:
        PH_Acidity = GetPH();
        Delay_ms(1000);
      
        CanBuf[7] = 0xd1;
        Can_Send_Msg(CanBuf, 8);      
        break;
      case 0xd2:
        PH_Basicity = GetPH();
        Delay_ms(1000);
      
        CanBuf[7] = 0xd2;
        Can_Send_Msg(CanBuf, 8);      
      
        OfficDatBuf[3] = ((6.86 - 4.00) / (PH_Neutral - PH_Acidity) * 1000000);      //中性和酸性之间
        OfficDatBuf[4] = ((9.18 - 6.86) / (PH_Basicity - PH_Neutral) * 1000000);     //中性和碱性之间
        Flash_WriteOfficDat();            //写入Flash
        break;
      default:
        break;
    }
  }
}


void Can_ReqGetNet (void)
{
  u8 CanBuf[8] = {0x0f, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
  CanBuf[1] = ChipID[3];
  CanBuf[2] = ChipID[2];
  CanBuf[3] = ChipID[1];
  CanBuf[4] = ChipID[0];
  Can_Send_Msg(CanBuf, 8);
  
//  printf ("%d, %d, %d, %d\r\n", CanBuf[1], CanBuf[2], CanBuf[3], CanBuf[4]);
}


void ZGB_ReceMsgProcess (uint8_t Uart2_ReceiveDat[])
{
  static float PH_Acidity = 0.0;
  static float PH_Neutral = 0.0;
  static float PH_Basicity = 0.0;
  float DifTmp = 0.0;
  
  //printf ("the chipid is: %d, %d, %d, %d\r\n", ZGB_Buf[0], ZGB_Buf[1], ZGB_Buf[2], ZGB_Buf[3]);
  /* 先确定是给自己发的消息 */
  if ((ChipID[0] == Uart2_ReceiveDat[4]) && (ChipID[1] == Uart2_ReceiveDat[3]) && \
      (ChipID[2] == Uart2_ReceiveDat[2]) && (ChipID[3] == Uart2_ReceiveDat[1]))
  {
    GetMasterNet_Statue = CheckLostTime;
    
//    printf ("the dat is to myself\r\n");
    
    /* 如果是申请入网有关的数据 */
    if ((Uart2_ReceiveDat[12] == 0xaa) && (Uart2_ReceiveDat[13] == 0xaa)) 
    {
      switch(Uart2_ReceiveDat[11])
      {
        case 0x01:    //申请已经接受，等待服务器确认
          {
//            printf ("wait checked by server\r\n");
            GetNet_Statue = Net_HasBeenAsked;
          }
          break;
        case 0x02:    //已经提交过申请，不再重复提交，等待服务器确认
          {
//            printf ("wait checked by server, will not sub again\r\n");
            GetNet_Statue = Net_HasBeenAsked;
          }
          break;
        case 0x03:
          {
//            printf ("Net Get Success\r\n");
            GetNet_Statue = Net_GetSuccess;
          }
          
        default:
          break;        
      }
    }
    if((Uart2_ReceiveDat[12] == 0xbb) && (Uart2_ReceiveDat[13] == 0xbb))
    {
//      printf ("receive the massage, the dat is: %d\r\n", Uart2_ReceiveDat[11]);
      switch(Uart2_ReceiveDat[11])
      {
        case 0x01:
          {
            Delay_ms(2000);
//            printf ("i want send msg\r\n");
            ZGB_Buf[1] = ChipID[3];
            ZGB_Buf[2] = ChipID[2];
            ZGB_Buf[3] = ChipID[1];
            ZGB_Buf[4] = ChipID[0];
            ZGB_Buf[5] = ((Flash_FifoBuf[5] >> 6) & 0xff) + 1;      //PH存放
            ZGB_Buf[6] = ((Flash_FifoBuf[5] & 0x1f) << 2) + ((Flash_FifoBuf[6] >> 30)& 0x3) + 1;       //水温整数
            ZGB_Buf[7] = ((Flash_FifoBuf[6] >> 18) & 0xff) + 1;       //板温整数
            ZGB_Buf[8] = 0x00;    //先清零
            ZGB_Buf[8] |= (((Flash_FifoBuf[6] >> 26) & 0xf) << 4);       //水温小数
            ZGB_Buf[8] |= ((Flash_FifoBuf[6] >> 14) & 0xf) + 1;       //板温小数
            ZGB_Buf[9] = ((Flash_FifoBuf[6] >> 6) & 0xff) + 1;         //流速整数
            ZGB_Buf[10] = 0x00;   //先清零
            ZGB_Buf[10] |= (((Flash_FifoBuf[6] >> 2) & 0xf) << 5);  //流速小数
            ZGB_Buf[10] |= (((Flash_FifoBuf[5] >> 24) & 0x01) << 3) + 1;     //水位
            ZGB_Buf[11] = 0x01;
            ZGB_Buf[12] = 0xbb;
            ZGB_Buf[13] = 0xbb;
            
//            WaterTemp = (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0);
//            printf("the water level is: %d", (Flash_FifoBuf[5] >> 6) & 0x01);
//            printf ("envirtmpint is: %d, envirtmpfloat is: %d\r\n", ZGB_Buf[7], ZGB_Buf[8]);
//            printf ("ZGB_Buf[5] is: %d", ZGB_Buf[5]);
            USART2_SendBuf(ZGB_Buf);
            USART2_SendByte('\n');
            
//            printf ("the Flash_FifoBuf[6] is: %d\r\n", ((Flash_FifoBuf[6] >> 26) & 0xf));
//            printf ("the ZGB_Buf[8] is: %d\r\n", ZGB_Buf[8]);
            MsgSendStatue = FAILED;
          }
          break;
        case 0x02:
          MsgSendStatue = SUCCESS;
          break;
        default:
          break;
      }
    }
    if((Uart2_ReceiveDat[12] == 0xcc) && (Uart2_ReceiveDat[13] == 0xcc))  //如果是标定有关的数据包
    {
      switch(Uart2_ReceiveDat[11])
      {
        case 0x01:      //如果是温度标定的数据包
          DifTmp = ((((Uart2_ReceiveDat[5] - 1) << 24) + ((Uart2_ReceiveDat[6] - 1) << 16)
                              + ((Uart2_ReceiveDat[7] - 1) << 8) + (Uart2_ReceiveDat[8] - 1)) / 10.0);
        
          if (Uart2_ReceiveDat[9] == 0x01)
            DifTmp = -DifTmp;
          
          OfficDatBuf[0] = ((DifTmp + (OfficDatBuf[0] / 10.0)) * 10);
        
          Delay_ms(2000);
          ZGB_Buf[1] = ChipID[3];
          ZGB_Buf[2] = ChipID[2];
          ZGB_Buf[3] = ChipID[1];
          ZGB_Buf[4] = ChipID[0];
          ZGB_Buf[5] = 0xff;
          ZGB_Buf[6] = 0xff;
          ZGB_Buf[7] = 0xff;
          ZGB_Buf[8] = 0xff;
          ZGB_Buf[9] = 0xff;
          ZGB_Buf[10] = 0xff;
          ZGB_Buf[11] = 0x01;
          ZGB_Buf[12] = 0xcc; 
          ZGB_Buf[13] = 0xcc;

          USART2_SendBuf(ZGB_Buf);
          USART2_SendByte('\n');
        
//        printf ("the off is : %f\r\n",OfficDatBuf[0] / 10.0);
          break;
        case 0x02:    //PH标定
          
          ZGB_Buf[1] = ChipID[3];
          ZGB_Buf[2] = ChipID[2];
          ZGB_Buf[3] = ChipID[1];
          ZGB_Buf[4] = ChipID[0];
          ZGB_Buf[11] = 0x02;
          ZGB_Buf[12] = 0xcc;
          ZGB_Buf[13] = 0xcc;
        
          switch (Uart2_ReceiveDat[10])
          {
            case 0x01:
              PH_Neutral = GetPH();
              Delay_ms(1000);
              ZGB_Buf[10] = 0x01;
              
              USART2_SendBuf(ZGB_Buf);
              USART2_SendByte('\n');
              
              break;
            case 0x02:
              PH_Acidity = GetPH();
              Delay_ms(1000);
            
              ZGB_Buf[10] = 0x02;
              
              USART2_SendBuf(ZGB_Buf);
              USART2_SendByte('\n');
              break;
            case 0x03:
              PH_Basicity = GetPH();
              Delay_ms(1000);
            
              ZGB_Buf[10] = 0x03;
              
              USART2_SendBuf(ZGB_Buf);
              USART2_SendByte('\n');
            
              OfficDatBuf[3] = ((6.86 - 4.00) / (PH_Neutral - PH_Acidity) * 1000000);      //中性和酸性之间
              OfficDatBuf[4] = ((9.18 - 6.86) / (PH_Basicity - PH_Neutral) * 1000000);     //中性和碱性之间
              Flash_WriteOfficDat();            //写入Flash
              
              break;
            default:
              break;
          }
          
          break;
        default:
          break;
      }
    }
  } 
}


void Zgb_ReqGetNet (void)
{
  u8 ZGB_Buf[14] = {0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  ZGB_Buf[1] = ChipID[3];
  ZGB_Buf[2] = ChipID[2];
  ZGB_Buf[3] = ChipID[1];
  ZGB_Buf[4] = ChipID[0];
  ZGB_Buf[11] = 0xaa;
  ZGB_Buf[12] = 0xaa;
  ZGB_Buf[13] = 0xaa;
  USART2_SendBuf(ZGB_Buf);
  
  USART2_SendByte('\n');
//  printf ("%d, %d, %d, %d\r\n", ZGB_Buf[1], ZGB_Buf[2], ZGB_Buf[3], ZGB_Buf[4]);
}


void task_GetNetAgain (void)
{
  u16 Rand = 0;
  UINT32 uwRet = LOS_OK;
  
  while(1)
  {
    if (GetMasterNet_Statue > 0)
      GetMasterNet_Statue--;
    
    if (GetMasterNet_Statue == 0)
    {
      GetNet_Statue = Net_NoNet;                //重置有网络标志位
      LED_Green(OFF);
      
      do
      {
        if (Connect_Way == USE_CAN)
          Can_ReqGetNet();
        else
          Zgb_ReqGetNet();
        
        Rand = GetRand();
        Delay_ms(Rand);           //延时是为ZGB考虑的，ZGB通讯速率较慢
        
        switch(GetNet_Statue)
        {
          case Net_Refused:       //如果提示主机拒绝了入网申请
            {
              while(1);           //死循环
            }
          case Net_HasBeenAsked : //如果提示已经发送申请等待服务器确认
            break;
          default:                //如果一直未收到主机的信息或是其他错误数据
            break;
        }
      }while((GetNet_Statue == Net_NoNet) && (GetMasterNet_Statue == 0)); 
      LED_Green(ON);
    }
    
    uwRet = LOS_TaskDelay(1000 * 60 * GetNetAgainTime);//系统延时1S
		if(uwRet !=LOS_OK)
      return;
  }
}


UINT32 g_GetNetTskHandle;
UINT32 creat_GetNetAgainTask (void)
{
  UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 3;                   //任务优先级，0最高
	task_init_param.pcName = "GetNetAgain";                //???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_GetNetAgain;//任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_GetNetTskHandle,&task_init_param);//创建任务
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}



void task_SendAgain (void)
{
  u16 Rand = 0;
  UINT32 uwRet = LOS_OK;
  
  while(1)
  {
    Rand = GetRand();
    if ((MsgSendStatue == FAILED) && (Connect_Way == USE_ZGB) && (GetMasterNet_Statue != 0))    //检测使用ZGB时最近一次的消息有没有发送成功
    {
      Delay_ms(Rand);
      USART2_SendBuf(ZGB_Buf);                                    //如果不成功，重新发送直到成功
      USART2_SendByte('\n');
    }
    
    uwRet = LOS_TaskDelay(Rand);//系统延时1S
		if(uwRet !=LOS_OK)
      return;
  }
}

UINT32 g_SendAgainTskHandle;
UINT32 creat_SendAgainTask (void)
{
  UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 5;                   //任务优先级，0最高
	task_init_param.pcName = "SendAgain";                //???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_SendAgain;//任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_GetNetTskHandle,&task_init_param);//创建任务
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}
