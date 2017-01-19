#include "common.h"

//extern uint8_t Connect_Way;                      //通讯方式标志位 USE_CAN   USE_ZGB
long SlaveNeedAddBuf[MAX_SlaveNum] = {0};           //需要上传到服务器的从机列表

uint8_t TmpOffGetRec = FAILUED;                     //从机温度补偿设置回包
uint8_t PHOffGetRec_Neutral = FAILUED;              //从机PH中性点设置回包
uint8_t PHOffGetRec_Acidity = FAILUED;              //酸性回包
uint8_t PHOffGetRec_Basicity = FAILUED;             //碱性回包

uint16_t WarmClearStatue = 0;     //是否所有警报都已报警
uint8_t WarmUpCount[MAX_SlaveNum + 1][6];

/*
 * 函数名：Can_ReceMsgProcess
 * 描述  ：CAN中断处理函数
 * 输入  ：RxMessage：接受到的消息
 * 返回  ：无
 */
void Can_ReceMsgProcess (CanRxMsg* RxMessage)
{
//  uint8_t i = 0;
//  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
//  uint8_t WaterLevel = 0;         //水位
//  float WaterSpeed = 0.0;         //流速
//  float EnvirTemp = 0.0;          //板温
//  float ph = 0.0;                 //ph
//  float WaterTemp = 0.0;          //水温
//  
//  uint8_t statue = 0;             //临时状态标志位
//  uint8_t SlaveSerial = 0;        //在Falsh中查找设备时存放序列号的变量
//  u8 Can_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};    //CAN发送用的数组
//  long ChipID = 0;
//  
//  uint8_t statueFromServer = 0;
//  uint8_t ErrorCount = 0;
//  uint8_t SlaveSearchCount = 0;

//  
//  /* 读取包内的从机ID */
//  ChipID = ((RxMessage->Data[1] << 24) + (RxMessage->Data[2] << 16) \
//                    + (RxMessage->Data[3] << 8) + (RxMessage->Data[4]));
//  
//  /* 接收到的是入网申请 */
//  if ((RxMessage->Data[6] == 0xaa) && (RxMessage->Data[7] == 0xaa)) 
//  {    
//    Can_DatBuf[1] = RxMessage->Data[1];           //将ID写入回发包内
//    Can_DatBuf[2] = RxMessage->Data[2];
//    Can_DatBuf[3] = RxMessage->Data[3];
//    Can_DatBuf[4] = RxMessage->Data[4];
//    Can_DatBuf[6] = 0xaa;                         //表示这是入网有关的数据包
//    Can_DatBuf[7] = 0xaa;
//    
//    switch(RxMessage->Data[5])                    //判断是入网申请里的哪种包
//    {
//      case 0xaa:                                  //申请入网
//        {
//          statue = Add_SlaveID(ChipID);           //将ID添加入Flash中，如果已存在 函数内不会添加
//          SlaveSerial = Search_SlaveID(ChipID);   //获取到此设备在Flash中存放的位置序号
//          
//          switch(statue)              
//          {
//            case SUCCESS:                         //添加从机成功，等待服务器进一步确定
//              {
//                /* 
//                 *  这里应该回发0x01表示已受理申请，等待服务器确定
//                 *  但后来协议做了些修改，这些验证变得无关紧要，因此
//                 *  直接回发0x03表示入网成功
//                */
//                Can_DatBuf[5] = 0x03;                             //入网成功
//                Can_Send_Msg(Can_DatBuf, 8);                      //向从机回复数据
////                Flash_FifoBuf[SlaveSerial + 1] |= (1 << 31);    //置位已接收申请标志位，后来这个取消了
////                Flash_WriteBufToPage();         //将BUF写入Flash中更新，后来取消了，随意更新Flash会导致一些程序内的交叉错误
//                if((((Flash_FifoBuf[SlaveSerial + 1] >> 31) & 0x01) == 0) &&           //设备未配对且未向服务器发送申请
//                    (((Flash_FifoBuf[SlaveSerial + 1] >> 30) & 0x01) == 0))
//                {
//                  SlaveNeedAddBuf[0] += 1;    //数组第一位表示当前待添加的从机数量
//                  SlaveNeedAddBuf[SlaveNeedAddBuf[0]] = ChipID;
//                }
//                
//                SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
//                if (SlaveSearchCount == (SlaveLostCount + 1))
//                  Beep_ON_Nop(100);
//                Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //将丢失标志位清空
//                
//                Beep_ON_Nop(100);                  //提示一声表示设备添加成功
//                FlashPage();                       //刷新显示页面
//              }
//              break;
//            case FLASH_ID_EXIST:                  //该从机申请已经提交过
//              {
//                Can_DatBuf[5] = 0x02;             //申请已提交过，提醒从机不要再发送申请信息
//                Can_Send_Msg(Can_DatBuf, 8);      //向从机回复数据
//                
//                SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
//                if (SlaveSearchCount == (SlaveLostCount + 1))
//                  Beep_ON_Nop(100);
//                Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //将丢失标志位清空
//              }
//              break;
//            default:
//              break;      
//          }    
//        }
//        break;
//      case 0x01:                                  //如果是对入网许可的回复
//        {
//          /*
//           *  入网的协议在设计之后根据上层实际情况作了些修改
//           *  这个回复变得不重要，不回发不影响从机的入网，只是
//           *  考虑到以后对功能的需求，这块并没有删除，只是不作
//           *  处理
//           */
////          Flash_ReadPageToBUF();                      //更新一下BUF区
////          statue = Search_SlaveID(ChipID);            //找到这个从机数据存放区的位置
////          Flash_FifoBuf[statue + 1] |= 0x80000000;    //在数据位的最高位置1，表示这个从机的申请已经被通过
////          Flash_WriteBufToPage();                     //将更新后的数据写入Flash
////          Flash_FifoBuf[SlaveSerial] &= ~(1 << 31);
//        }
//        break;
//      default:
//        break;
//    }
//  }
//  /* 接收到采集数据反馈 */
//  if ((RxMessage->Data[7] == 0x01) || (RxMessage->Data[7] == 0x02) || (RxMessage->Data[7] == 0x03))
//  {
//    RxMessage->Data[5]--;   //自减是为了CAN程序和ZGB的一致，数据在发出前自增了一次，原因在从机的ZGB发送中有写
//    RxMessage->Data[6]--;

//    /* 查找到阈值 */
//    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
//    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
//    WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
//    WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
//    EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
//    EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
//    WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
//    WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);

//    
//    SlaveSerial = Search_SlaveID(ChipID);   //获取到此设备在Flash中存放的位置序号
//    switch (RxMessage->Data[7])             //判断是数据的第几个包，CAN一次数据分为三个包
//    {
//      case 0x01:
//            /* PH值存放 */
//            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xff << 12);                 //清零该位置
//            Flash_FifoBuf[SlaveSerial + 1] |= (RxMessage->Data[5] << 12);  
//      
//            /* 水温存放 */
//            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xff << 4); 
//            Flash_FifoBuf[SlaveSerial + 1] |= (RxMessage->Data[6] << 4);     //整数部分
//        break;
//      case 0x02:
//            /* 板温存放 */
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xff << 24);
//            Flash_FifoBuf[SlaveSerial + 2] |= (RxMessage->Data[5] << 24);                 //整数部分
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xf << 20);
//            Flash_FifoBuf[SlaveSerial + 2] |= ((RxMessage->Data[6] & 0xf) << 20);         //小数部分
//            /* 水温存放 */
//            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 0); 
//            Flash_FifoBuf[SlaveSerial + 1] |= (((RxMessage->Data[6] >> 5) & 0xf) << 0);   //小数部分
//        break;
//      case 0x03:
//            /* 流速存放 */
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xff << 12);
//            Flash_FifoBuf[SlaveSerial + 2] |= (RxMessage->Data[5] << 12);                 //整数部分
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xf << 8);
//            Flash_FifoBuf[SlaveSerial + 2] |= (((RxMessage->Data[6] >> 5) & 0xf) << 8);   //小数部分
//            /* 水位存放 */
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0x1 << 7);
//            Flash_FifoBuf[SlaveSerial + 2] |= (((RxMessage->Data[6] >> 3) & 0x01) << 7);  //水位只有一位    
//      
//      
//            /* 提取接收到的采集数据 */
//            WaterLevel = ((Flash_FifoBuf[SlaveSerial + 2] >> 7) & 0x01);
//            WaterSpeed = ((Flash_FifoBuf[SlaveSerial + 2] >> 12) & 0xff) + (((Flash_FifoBuf[SlaveSerial + 2] >> 8) & 0xf) / 10.0);
//            EnvirTemp = ((Flash_FifoBuf[SlaveSerial + 2] >> 24) & 0xff) + (((Flash_FifoBuf[SlaveSerial + 2] >> 20) & 0xf) / 10.0);
//            ph = (((Flash_FifoBuf[SlaveSerial + 1] >> 16) & 0xf) + ((Flash_FifoBuf[SlaveSerial + 1] >> 12) & 0xf) / 10.0);
//            WaterTemp = (((Flash_FifoBuf[SlaveSerial + 1] >> 4) & 0xff) + ((Flash_FifoBuf[SlaveSerial + 1] >> 0) & 0xf) / 10.0);
//            
//            for (i = 1; i <= Flash_FifoBuf[0]; i++)
//              if (ChipID == (Flash_FifoBuf[7 + i * 3]))
//                break;
//              
//            /* 判断水位是否需要报警 */
//            if (!WaterLevel)                                                    //水位报警
//              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 24);                      //置位报警标志位
//            else                                                                //水位恢复正常
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 1] >> 24) & 0x01)
//                WarmClearStatue++;
//              
//              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 24);                   //将标志位清空
//              WarmUpCount[i][3] = 0;
//            }
//              
//            /* 判断流速是否需要报警 */
//            if ((WaterSpeed > WaterSpeedMax) || (WaterSpeed < WaterSpeedMin))   //流速报警
//              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 22);
//            else
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 1] >> 22) & 0x01)
//                WarmClearStatue++;
//              
//              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 22);                   //流速正常
//              WarmUpCount[i][2] = 0;
//            }
//              
//            
//            /* 判断室温是否需要报警 */
//            if ((EnvirTemp > EnvirTempMax) || (EnvirTemp < EnvirTempMin))       //室温报警
//              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 20);
//            else
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 1] >> 20) & 0x01)
//                WarmClearStatue++;
//              
//              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 20);                   //室温正常
//              WarmUpCount[i][0] = 0;
//            }
//              
//            /* 判断PH是否需要报警 */
//            if ((ph > PHMax) || (ph < PHMin))                                   //PH报警
//              Flash_FifoBuf[SlaveSerial + 2] |= (1 << 5);
//            else
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 2] >> 5) & 0x01)
//                WarmClearStatue++;
//              
//              WarmUpCount[i][4] = 0;
//              Flash_FifoBuf[SlaveSerial + 2] &= ~(0x3 << 5);                    //PH正常
//            }
//              
//            /* 判断水温是否需要报警 */
//            if ((WaterTemp > WaterTempMax) || (WaterTemp < WaterTempMin))       //水温报警
//              Flash_FifoBuf[SlaveSerial + 2] |= (1 << 3);
//            else
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 2] >> 3) & 0x01)
//                WarmClearStatue++;
//              
//              WarmUpCount[i][1] = 0;
//              Flash_FifoBuf[SlaveSerial + 2] &= ~(0x3 << 3);                    //水温正常
//            }
//              
//      
//            SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
//            if (SlaveSearchCount == (SlaveLostCount + 1))
//              Beep_ON_Nop(100);
//            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //将丢失标志位清空
//            
//            FlashPage();        //接收一次数据后刷新页面
//    
//        break;
//      default:
//        break;
//    }
//  }
//  if (RxMessage->Data[7] == 0xcc)   //如果是温度补偿的回包
//  {
//    TmpOffGetRec = SUCCESS;
//  }
//  switch (RxMessage->Data[7])
//  {
//    case 0xd0:
//      PHOffGetRec_Neutral = SUCCESS;
//      break;
//    case 0xd1:
//      PHOffGetRec_Acidity = SUCCESS;
//      break;
//    case 0xd2:
//      PHOffGetRec_Basicity = SUCCESS;
//      break;
//    default:
//      break;
//  }
}


/*
 * 函数名：ZGB_ReceMsgProcess
 * 描述  ：ZGB中断处理函数
 * 输入  ：Uart2_ReceiveDat：接受到的消息
 * 返回  ：无
 */
void ZGB_ReceMsgProcess (uint8_t Uart2_ReceiveDat[])
{
  uint8_t i = 0;
  uint8_t SlaveSerial = 0;        //在Falsh中查找设备时存放序列号的变量
  uint8_t WaterLevel = 0;         //水位
  float WaterSpeed = 0.0;         //流速
  float EnvirTemp = 0.0;          //板温
  float ph = 0.0;                 //ph
  float WaterTemp = 0.0;          //水温
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
  
  u8 ZGB_DatBuf[14] = {0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; //回发的包
  long ChipID = 0;
  uint8_t statue = 0;   //, statueFromServer = 0
//  uint8_t ErrorCount = 0;
  uint8_t SlaveSearchCount = 0;
  
  ChipID = ((Uart2_ReceiveDat[1] << 24) + (Uart2_ReceiveDat[2] << 16) \
                    + (Uart2_ReceiveDat[3] << 8) + (Uart2_ReceiveDat[4]));
  
  /* 入网申请 */
  if ((Uart2_ReceiveDat[12] == 0xaa) && (Uart2_ReceiveDat[13] == 0xaa)) 
  {
    ZGB_DatBuf[1] = Uart2_ReceiveDat[1];
    ZGB_DatBuf[2] = Uart2_ReceiveDat[2];
    ZGB_DatBuf[3] = Uart2_ReceiveDat[3];
    ZGB_DatBuf[4] = Uart2_ReceiveDat[4];
    ZGB_DatBuf[12] = 0xaa;                        //表示这是入网有关的数据包
    ZGB_DatBuf[13] = 0xaa;
    
    switch(Uart2_ReceiveDat[11])                  //查看具体是入网相关里的哪种包
    {
      case 0xaa:                                  //申请入网
        {
          statue = Add_SlaveID(ChipID);
          SlaveSerial = Search_SlaveID(ChipID);   //获取到此设备在Flash中存放的位置序号
          switch(statue)
          {
            case SUCCESS:                         //添加从机成功，等待服务器进一步确定
              {
                /* 
                 *  根据最初的协议，这里应该回发0x01表示已接收申请但还需要服务器通过
                 *  回发0x03表示服务器已通过申请，这是根据实际情况做的修改
                */
                ZGB_DatBuf[11] = 0x03;             //已受理申请，等待服务器确定
                
                UserDelay_KeyScan(2000);                    //ZGB有点问题，刚接收完消息立刻回发发不出去，因此等待一会
                USART2_SendBuf(ZGB_DatBuf);        //向从机回复数据
                USART2_SendByte('\n');             //ZGB根据回车符判断一帧数据是否结束
                
                if((((Flash_FifoBuf[SlaveSerial + 1] >> 31) & 0x01) == 0) &&           //设备未配对且未向服务器发送申请
                    (((Flash_FifoBuf[SlaveSerial + 1] >> 30) & 0x01) == 0))
                {
                  SlaveNeedAddBuf[0] += 1;    //数组第一位表示当前待添加的从机数量
                  SlaveNeedAddBuf[SlaveNeedAddBuf[0]] = ChipID;
                }
                
                SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
                if (SlaveSearchCount == (SlaveLostCount + 1))
                  Beep_ON_Nop(100);
                Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //将丢失标志位清空
                
                Beep_ON_Nop(100);                  //提示一声表示设备添加成功
                FlashPage();                       //刷新显示页面
              }
              break;
            case FLASH_ID_EXIST:                  //该从机申请已经提交过
              {
                UserDelay_KeyScan(2000);
                ZGB_DatBuf[11] = 0x02;            //申请已提交过，提醒从机不要再发送申请信息
                USART2_SendBuf(ZGB_DatBuf);       //向从机回复数据
                USART2_SendByte('\n');
                
                SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
                if (SlaveSearchCount == (SlaveLostCount + 1))
                  Beep_ON_Nop(100);
                Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //将丢失标志位清空
              }
              break;
            default:
              break;      
          }    
        }
        break;
     case 0x01:                                   //如果是对入网许可的回复
        {
        }
       break;
     default:
       break;
    }
  }
  
  /* 采集的数据反馈 */
  if ((Uart2_ReceiveDat[12] == 0xbb) && (Uart2_ReceiveDat[13] == 0xbb)) 
  {    
    /* 查找到阈值 */
    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
    WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
    WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
    EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
    EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
    WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
    WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
    
    switch(Uart2_ReceiveDat[11])    //查看具体是采集数据里的哪种包，目前只写了一种，这样写方便后期加功能
    {
      case 0x01:
        {
          SlaveSerial = Search_SlaveID(ChipID);   //获取到此设备在Flash中存放的位置序号
          if (SlaveSerial != 0)                   //如果Flash中存在此ID
          {                                       
            Uart2_ReceiveDat[5]--;
            Uart2_ReceiveDat[6]--;
            Uart2_ReceiveDat[7]--;
            Uart2_ReceiveDat[8]--;
            Uart2_ReceiveDat[9]--;
            Uart2_ReceiveDat[10]--;
            
            /* PH值存放 */
            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xff << 12);                 
            Flash_FifoBuf[SlaveSerial + 1] |= (Uart2_ReceiveDat[5] << 12);  
            
            /* 水温存放 */
            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xff << 4); 
            Flash_FifoBuf[SlaveSerial + 1] |= (Uart2_ReceiveDat[6] << 4);    
            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 0); 
            Flash_FifoBuf[SlaveSerial + 1] |= (((Uart2_ReceiveDat[8] >> 4) & 0xf) << 0);  
            
            /* 板温存放 */
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xff << 24);
            Flash_FifoBuf[SlaveSerial + 2] |= (Uart2_ReceiveDat[7] << 24);   
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xf << 20);
            Flash_FifoBuf[SlaveSerial + 2] |= ((Uart2_ReceiveDat[8] & 0xf) << 20);  
            
            /* 流速存放 */
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xff << 12);
            Flash_FifoBuf[SlaveSerial + 2] |= (Uart2_ReceiveDat[9] << 12);  
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xf << 8);
            Flash_FifoBuf[SlaveSerial + 2] |= (((Uart2_ReceiveDat[10] >> 4) & 0xf) << 8);  
            
            /* 水位存放 */
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0x1 << 7);
            Flash_FifoBuf[SlaveSerial + 2] |= (((Uart2_ReceiveDat[10] >> 3) & 0x01) << 7);   
            
            UserDelay_KeyScan(500);
            ZGB_DatBuf[1] = Uart2_ReceiveDat[1];
            ZGB_DatBuf[2] = Uart2_ReceiveDat[2];
            ZGB_DatBuf[3] = Uart2_ReceiveDat[3];
            ZGB_DatBuf[4] = Uart2_ReceiveDat[4];
            ZGB_DatBuf[11] = 0x02;
            ZGB_DatBuf[12] = 0xbb;              //表示这是入网有关的数据包
            ZGB_DatBuf[13] = 0xbb;
            USART2_SendBuf(ZGB_DatBuf);         //向从机回复已经收到数据
            USART2_SendByte('\n');
            
            WaterLevel = ((Flash_FifoBuf[SlaveSerial + 2] >> 7) & 0x01);
            WaterSpeed = ((Flash_FifoBuf[SlaveSerial + 2] >> 12) & 0xff) + (((Flash_FifoBuf[SlaveSerial + 2] >> 8) & 0xf) / 10.0);
            EnvirTemp = ((Flash_FifoBuf[SlaveSerial + 2] >> 24) & 0xff) + (((Flash_FifoBuf[SlaveSerial + 2] >> 20) & 0xf) / 10.0);
            ph = (((Flash_FifoBuf[SlaveSerial + 1] >> 16) & 0xf) + ((Flash_FifoBuf[SlaveSerial + 1] >> 12) & 0xf) / 10.0);
            WaterTemp = (((Flash_FifoBuf[SlaveSerial + 1] >> 4) & 0xff) + ((Flash_FifoBuf[SlaveSerial + 1] >> 0) & 0xf) / 10.0);
            
            for (i = 1; i <= Flash_FifoBuf[0]; i++)
              if (ChipID == (Flash_FifoBuf[7 + i * 3]))
                break;
            
            
            /* 水位 */
            if (WaterLevel)                                                    //水位报警
              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 24);
            else                                                                //水位恢复正常
            {
              if ((Flash_FifoBuf[SlaveSerial + 1] >> 24) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 24);
              
              WarmUpCount[i][3] = 0;
            }
              
            /* 流速 */
            if ((WaterSpeed > WaterSpeedMax) || (WaterSpeed < WaterSpeedMin))   //流速报警
              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 22);
            else
            {
              if ((Flash_FifoBuf[SlaveSerial + 1] >> 22) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 22);                   //流速正常
              
              WarmUpCount[i][2] = 0;
            }
              
            /* 板温 */
            if ((EnvirTemp > EnvirTempMax) || (EnvirTemp < EnvirTempMin))       //板温报警
              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 20);
            else
            {
              if ((Flash_FifoBuf[SlaveSerial + 1] >> 20) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 20);                   //板温正常
              
              WarmUpCount[i][0] = 0;
            }
              
            
            /* PH */
            if ((ph > PHMax) || (ph < PHMin))                                   //PH报警
              Flash_FifoBuf[SlaveSerial + 2] |= (1 << 5);
            else
            {
              if ((Flash_FifoBuf[SlaveSerial + 2] >> 5) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 2] &= ~(0x3 << 5);                    //PH正常
              
              WarmUpCount[i][4] = 0;
            }
              
            
            /* 水温 */
            if ((WaterTemp > WaterTempMax) || (WaterTemp < WaterTempMin))       //水温报警
              Flash_FifoBuf[SlaveSerial + 2] |= (1 << 3);
            else
            {
              if ((Flash_FifoBuf[SlaveSerial + 2] >> 3) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 2] &= ~(0x3 << 3);                    //水温正常
              
              WarmUpCount[i][1] = 0;
            }
              
            SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
            if (SlaveSearchCount == (SlaveLostCount + 1))
              Beep_ON_Nop(100);
            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);
            
            FlashPage();                                                        //刷新页面
          }
        }
        break;
      default:
        break;
    }   
  }
  if ((Uart2_ReceiveDat[12] == 0xcc) && (Uart2_ReceiveDat[13] == 0xcc))   //如果是标定的回复包
  {
    switch(Uart2_ReceiveDat[11])
    {
      case 0x01:
        TmpOffGetRec = SUCCESS;
        
        break;
      case 0x02:
          switch (Uart2_ReceiveDat[10])
          {
            case 0x01:
              PHOffGetRec_Neutral = SUCCESS;
              break;
            case 0x02:
              PHOffGetRec_Acidity = SUCCESS;
              break;
            case 0x03:
              PHOffGetRec_Basicity = SUCCESS;
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

/*
 * 函数名：TouchSlaveProcess_ZGB
 * 描述  ：通过ZGB向从机索要采集到的数据
 * 输入  ：无
 * 返回  ：无
 */
void TouchSlaveProcess_ZGB (void)
{
  static int i = 1;
  u8 ZGB_DatBuf[14] = {0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  int SearchCount = 0;
  
  if (Flash_FifoBuf[0] >= 1)
  {
//    SearchCount = ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 26) & 0xf);
//    if (SearchCount < SlaveLostCount)   //如果该设备未丢失
//    {
      ZGB_DatBuf[1] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 24) & 0xff);
      ZGB_DatBuf[2] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 16) & 0xff);
      ZGB_DatBuf[3] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 8) & 0xff);
      ZGB_DatBuf[4] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 0) & 0xff);
      ZGB_DatBuf[11] = 0x01;
      ZGB_DatBuf[12] = 0xbb;
      ZGB_DatBuf[13] = 0xbb;
      USART2_SendBuf(ZGB_DatBuf);          //向从机发送
      USART2_SendByte('\n');
    
      SearchCount = ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 26) & 0xf);
      if (SearchCount < SlaveLostCount)
      {
        SearchCount++;
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] &= ~(0xf << 26);                 //丢失次数加1  如果接受到消息会清空的
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= ((SearchCount & 0xf) << 26);
      }
      
//    }
    
    i++;
    if (i > Flash_FifoBuf[0])
      i = 1;
  }
  
}

/*
 * 函数名：TouchSlaveProcess_CAN
 * 描述  ：通过CAN向从机索要采集到的数据
 * 输入  ：无
 * 返回  ：无
 */
void TouchSlaveProcess_CAN (void)
{
  static int i = 1;
  int SearchCount = 0;
  u8 CAN_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  

  CAN_DatBuf[1] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 24) & 0xff);
  CAN_DatBuf[2] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 16) & 0xff);
  CAN_DatBuf[3] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 8) & 0xff);
  CAN_DatBuf[4] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 0) & 0xff);
  CAN_DatBuf[5] = 0x01;
  CAN_DatBuf[6] = 0xbb;
  CAN_DatBuf[7] = 0xbb;
  Can_Send_Msg(CAN_DatBuf, 8);
  
  SearchCount = ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 26) & 0xf);
  if (SearchCount < SlaveLostCount)
  {
    SearchCount++;
    Flash_FifoBuf[7 + (i - 1) * 3 + 1] &= ~(0xf << 26);                 //丢失次数加1  如果接受到消息会清空的
    Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= ((SearchCount & 0xf) << 26);
  }
  
  i++;
  if (i > Flash_FifoBuf[0])
    i = 1;
}

/*
 * 函数名：Test_AllowInNet
 * 描述  ：组网时写的测试函数
 * 输入  ：无
 * 返回  ：无
 */
//void Test_AllowInNet (void)
//{
//  u8 CAN_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 
//  CAN_DatBuf[1] = ((Flash_FifoBuf[6] >> 24) & 0xff);
//  CAN_DatBuf[2] = ((Flash_FifoBuf[6] >> 16) & 0xff);
//  CAN_DatBuf[3] = ((Flash_FifoBuf[6] >> 8) & 0xff);
//  CAN_DatBuf[4] = (Flash_FifoBuf[6] & 0xff);
//  CAN_DatBuf[5] = 0x03;               //申请已提交过，提醒从机不要再发送申请信息
//  CAN_DatBuf[6] = 0xaa;
//  CAN_DatBuf[7] = 0xaa;
//  Can_Send_Msg(CAN_DatBuf, 8);
//}



void task_GetSlaveDat (void)
{
  UINT32 uwRet = LOS_OK;
  
  while(1)
  {
    if (Connect_Way == USE_CAN)
      TouchSlaveProcess_CAN();
    else
      TouchSlaveProcess_ZGB();
    
//    USART1_SendBuf ("SlaveDatGet_Task\r\n");
    
    uwRet = LOS_TaskDelay(1000 * TouchSlaveTime);//系统延时1S
		if(uwRet !=LOS_OK)
      return;
  }
}


UINT32 g_GetSlaveDatTskHandle;

UINT32 creat_GetSlaveDatTask (void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 4;                   //任务优先级，0最高
	task_init_param.pcName = "SlaveDat";                //???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_GetSlaveDat;//任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_GetSlaveDatTskHandle,&task_init_param);//创建任务
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}


