#include "common.h"

int8_t PageSerial = HomePage;         //记录当前在哪个页面
int8_t DatPage = DisMastDat;          //记录当前显示的是主机还是从机数据
int8_t TmpOffPage = DisMastTmpOff;    //记录当前显示的是主机还是从机温度补偿
int8_t PHOffPage = DisMastPHOff;      //记录当前显示的是主机还是从机PH标定

/*
 * 函数名：InitDis
 * 描述  ：初始化界面的提示语显示
 * 输入  ：str: 要显示的字符
 * 返回  ：无
 */
void InitDis (char* str)
{
  uint8_t SendBuf[50];
  sprintf ((char*)SendBuf, "t0.txt=\"%s\"", str);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);      //屏幕命令已三个连续的0xff作结束
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  Delay_ms(100);
}

/*
 * 函数名：Scr_SendCmd
 * 描述  ：向屏幕发送命令
 * 输入  ：str: 要发送的命令
 * 返回  ：无
 */
void Scr_SendCmd (uint8_t* str)
{
  USART1_SendBuf(str);
  USART1_SendByte(0xff);    
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
}

/*
 * 函数名：DisSensorVal
 * 描述  ：数据显示页面的数据更新
 * 输入  ： Sensor：要更新的传感器种类
 *          color： 要显示的颜色
 *          val：   要显示的值
 * 返回  ：无
 */
void DisSensorVal (uint8_t Sensor, uint8_t color, float val)
{
  uint8_t SendBuf[30];                //发送给屏幕的buf
  
  switch (Sensor)
  {
    /* 室温显示 */
    case SensorEnvirTemp:          
    {
      sprintf ((char*)SendBuf, "t16.txt=\"%.1f\"", val);    //合成室温值
      Scr_SendCmd(SendBuf);                                 //发送命令
      switch (color)
      {
        case Color_Red:
          sprintf ((char*)SendBuf, "t16.pco=RED");          //合成红色
          Scr_SendCmd(SendBuf);                             //发送命令
          break;
        case Color_Green:
          sprintf ((char*)SendBuf, "t16.pco=1024");         //合成绿色
          Scr_SendCmd(SendBuf);                             //发送命令
          break;
        default:
          break;
      }
    }
      break;
    /* 水温显示 */
    case SensorWaterTemp:        
    {
      sprintf ((char*)SendBuf, "t17.txt=\"%.1f\"", val);    
      Scr_SendCmd(SendBuf); 
      switch (color)
      {
        case Color_Red:
          sprintf ((char*)SendBuf, "t17.pco=RED");   
          Scr_SendCmd(SendBuf);       
          break;
        case Color_Green:
          sprintf ((char*)SendBuf, "t17.pco=1024");   
          Scr_SendCmd(SendBuf);        
          break;
        default:
          break;
      }
    }
      break;
    /* 泵状态显示 */
    case SensorWaterSpeed:
    {
      sprintf ((char*)SendBuf, "t18.txt=\"%.1f\"", val);    
      Scr_SendCmd(SendBuf); 
      switch (color)
      {
        case Color_Red:
          sprintf ((char*)SendBuf, "t18.pco=RED");   
          Scr_SendCmd(SendBuf);       
          break;
        case Color_Green:
          sprintf ((char*)SendBuf, "t18.pco=1024");  
          Scr_SendCmd(SendBuf);       
          break;
        default:
          break;
      }
    }
      break;
    /* 水位显示 */
    case SensorWaterLevel:
    {
      /* 如果为0表示暂时无报警，因此显示“正常” */
      if (!val)           //
      {
        sprintf ((char*)SendBuf, "t19.txt=\"正常\""); 
        Scr_SendCmd(SendBuf); 
        sprintf ((char*)SendBuf, "t19.pco=1024");  
        Scr_SendCmd(SendBuf); 
      }        
      /* 存在报警即表示水位过高 */
      else         
      {
        sprintf ((char*)SendBuf, "t19.txt=\"过低\"");  
        Scr_SendCmd(SendBuf); 
        sprintf ((char*)SendBuf, "t19.pco=RED"); 
        Scr_SendCmd(SendBuf); 
      }
    }
      break;
    /* PH显示 */
    case SensorPH:
    {
      sprintf ((char*)SendBuf, "t20.txt=\"%.1f\"", val);    
      Scr_SendCmd(SendBuf); 
      switch (color)
      {
        case Color_Red:
          sprintf ((char*)SendBuf, "t20.pco=RED");   
          Scr_SendCmd(SendBuf);       
          break;
        case Color_Green:
          sprintf ((char*)SendBuf, "t20.pco=1024");   
          Scr_SendCmd(SendBuf);        
          break;
        default:
          break;
      }
    }
      break;
    /* 电源状态显示 */
    case SensorPower:
    {
      if (!val)
      {
        sprintf ((char*)SendBuf, "t21.txt=\"正常\"");   
        Scr_SendCmd(SendBuf); 
        sprintf ((char*)SendBuf, "t21.pco=1024");
        Scr_SendCmd(SendBuf);  
      }
      else
      {
        sprintf ((char*)SendBuf, "t21.txt=\"掉电\"");   
        Scr_SendCmd(SendBuf);         
        sprintf ((char*)SendBuf, "t21.pco=RED");  
        Scr_SendCmd(SendBuf);  
      }
    }
      break;
    /* 显示下方的页面编号 */
    case SerialSerial:
    {
      if ((int)val == DisMastDat)
        sprintf ((char*)SendBuf, "t26.txt=\"主机(%d/%d)\"", (int)val + 1, Flash_FifoBuf[0] + 1);
      else
        sprintf ((char*)SendBuf, "t26.txt=\"%04X(%d/%d)\"",Flash_FifoBuf[7 + ((int)val - 1) * 3] & 0xffff, (int)val + 1, Flash_FifoBuf[0] + 1);
      Scr_SendCmd(SendBuf);  
    }
      break;
    case Time_UpDat:
    {
      
//      extern int User_year, User_month, User_day, User_hour, User_min, User_sec;
      
//      sprintf ((char*)SendBuf, "t0.txt=\"%d\"", User_year);
//      Scr_SendCmd(SendBuf);  
//      sprintf ((char*)SendBuf, "t2.txt=\"%d\"", User_month);
//      Scr_SendCmd(SendBuf); 
//      sprintf ((char*)SendBuf, "t4.txt=\"%d\"", User_day);
//      Scr_SendCmd(SendBuf); 
//      sprintf ((char*)SendBuf, "t6.txt=\"%d\"", User_hour);
//      Scr_SendCmd(SendBuf); 
//      sprintf ((char*)SendBuf, "t8.txt=\"%d\"", User_min);
//      Scr_SendCmd(SendBuf); 
    }
      break;
    default:
      break;    
  }
}


/*
 * 函数名：FlashPage
 * 描述  ：页面刷新
 * 输入  ：无
 * 返回  ：无
 */
void FlashPage (void)
{
  switch (PageSerial)                               //判断当前所在页面
  {
    case HomePage:                                  //最开始的进度条界面
      break;
    case DisDatPage:                                //如果当前是数据显示页面
    {
      switch (KeyStatue)                            //判断按下的是哪个键
      {
        case KeyLF:                                 //左键
          DatPage--;                                //页面序号减一
          if (DatPage < 0)                          //如果已经是数据页面的第一页
          {
            DatPage = 0;                           //转到阈值设置页面
//            Scr_SendCmd("page LimSet");   //跳转页面
//            PageSerial = LimSetPage;                //所在页面标志位重置
            KeyStatue = KeyDefault;                 //按键重置
//            UserDelay_KeyScan(100);
//            FlashLimSetPage(KeyDefault);            //手动刷新阈值页面
            return;
          }          
          break;
        case KeyRG:                                 //右键
          DatPage++;                                //页面序号加一
          if (DatPage >= Flash_FifoBuf[0])          //判断页面是否到了最右边
            DatPage = Flash_FifoBuf[0];       
          break;
        default:
          break;
      }
      FlashDatPage();                               //刷新页面
    }
      break;
    case LimSetPage:                                //如果是设置阈值页面
      {
//        FlashLimSetPage(KeyStatue);                 //进度阈值页面函数
      }
      break;
    case TimeSetPage:
//      FlashTimeSetPage(KeyStatue);                  //刷新时间设置页面
      break;
    case TmpOfficeSetPage:                          //刷新温度补偿页面
//      FlashTmpOffPage(KeyStatue); 
      break;
    case PHOfficeSetPage:                           //刷新PH标定页面
//      FlashPHOffPage(KeyStatue);
      break;
    default:
      break;
  }
  KeyStatue = KeyDefault;                           //按键标志位置位
}

/*
 * 函数名：TimeSetFlashPos
 * 描述  ：时间设置界面字体颜色设置
 * 输入  ：Pos_x：要编辑的字符串在屏中的x位置
 *         Pos_y：要编辑的字符串在屏中的y位置
 *         Color：要显示的颜色
 * 返回  ：无
 */
void TimeSetFlashPos (int8_t Pos_x, int8_t Pos_y, uint8_t Color)
{
  if ((Pos_x == 0) && (Pos_y == 0))                 //年
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t1.pco=YELLOW");   //显示黄色
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t1.pco=RED");      //显示红色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t1.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 0))       //月
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t3.pco=YELLOW");   //显示黄色
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t3.pco=RED");      //显示红色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t3.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  if ((Pos_x == 2) && (Pos_y == 0))       //日
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t5.pco=YELLOW");   //显示黄色
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t5.pco=RED");      //显示红色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t5.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 1))     //小时
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t7.pco=YELLOW");   //显示黄色
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t7.pco=RED");      //显示红色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t7.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 1))       //分钟
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t9.pco=YELLOW");   //显示黄色
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t9.pco=RED");      //显示红色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t9.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  if ((Pos_x == 2) && (Pos_y == 1))         //秒
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t11.pco=YELLOW");   //显示黄色
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t11.pco=RED");      //显示红色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t11.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  
  if ((Pos_x == 1) && (Pos_y == 2))         //保存
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t15.pco=YELLOW");
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t15.pco=32799");       //紫色
        break;
      default:
        break;
    }
  } 
  
  if ((Pos_x == 2) && (Pos_y == 2))       //下一页
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t23.pco=YELLOW");
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t23.pco=32799");       //紫色
        break;
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 2))       //上一页
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t22.pco=YELLOW");
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t22.pco=32799");       //紫色
        break;
      default:
        break;
    }
  }
}


/*
 * 函数名：TmpOffFlashPos
 * 描述  ：温度补偿设置界面字体颜色设置
 * 输入  ：Pos_x：要编辑的字符串在屏中的x位置
 *         Pos_y：要编辑的字符串在屏中的y位置
 *         Color：要显示的颜色
 * 返回  ：无
 */
void TmpOffFlashPos (int8_t Pos_x, int8_t Pos_y, uint8_t Color)
{
  if ((Pos_x == 0) && (Pos_y == 0))                 //确定
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t3.pco=YELLOW");   //显示黄色
        break;
      case Color_Red:
        Scr_SendCmd((uint8_t*)"t3.pco=RED");      //显示红色
        break;
      case Color_Green:
        Scr_SendCmd((uint8_t*)"t3.pco=1024");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t3.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 0))                 //确定
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t4.pco=YELLOW");   //显示黄色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t4.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 1))                 //上一设备
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t5.pco=YELLOW");   //显示黄色
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t5.pco=32799");       //紫色
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 1))                 //下一设备
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t6.pco=YELLOW");   //显示黄色
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t6.pco=32799");       //紫色
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 2))                 //上一页
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t22.pco=YELLOW");   //显示黄色
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t22.pco=32799");       //紫色
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 2))                 //下一页
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t23.pco=YELLOW");   //显示黄色
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t23.pco=32799");       //紫色
      default:
        break;
    }
  }
}


/*
 * 函数名：PHOffFlashPos
 * 描述  ：PH标定设置界面字体颜色设置
 * 输入  ：Pos_x：要编辑的字符串在屏中的x位置
 *         Pos_y：要编辑的字符串在屏中的y位置
 *         Color：要显示的颜色
 * 返回  ：无
 */
void PHOffFlashPos (int8_t Pos_x, int8_t Pos_y, uint8_t Color)
{
  if ((Pos_x == 1) && (Pos_y == 0))         //标定
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t4.pco=YELLOW");   //显示黄色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t4.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 1))                 //上一设备
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t5.pco=YELLOW");   //显示黄色
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t5.pco=32799");       //紫色
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 1))                 //下一设备
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t6.pco=YELLOW");   //显示黄色
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t6.pco=32799");       //紫色
      default:
        break;
    }
  }
  if ((Pos_x == 1) && (Pos_y == 2))                 //下一设备
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t23.pco=YELLOW");   //显示黄色
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t23.pco=32799");       //紫色
      default:
        break;
    }
  }
  
}


/*
 * 函数名：FlashPHOffPage
 * 描述  ：PH标定刷新页面
 * 输入  ：Key_Statue：按键状态
 * 返回  ：无
 */
void FlashPHOffPage (uint8_t Key_Statue)
{
  uint8_t statue = 0;
  float PH;
  uint8_t SendBuf[30];      //向屏幕发送的buf  
  static int8_t WritePos[2] = {1, 2};   //光标所在点    
  static int8_t WriteLastPos[2] = {1, 2};   //光标上次所在点
  static uint8_t IsWrite = 0;     //读写状态
  float DifPH = 0.0;             //温度差值
  static int8_t valPoint = 0;     //页面编号
  static uint8_t FirstCommin = 1; //第一次进入页面
  static uint8_t DemarcateChice = Neutral;    //初始为中性
  
  static float PH_Acidity = 0.0;
  static float PH_Neutral = 0.0;
  static float PH_Basicity = 0.0;
  
  u8 ZGB_DatBuf[14] = {0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};   //ZGB包
  u8 Can_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};    //CAN发送用的数组

  if (Key_Statue != KeyDefault)           //按下任何一个按键，之前的文本提示框置位
  {
    Scr_SendCmd("t7.txt=\"\"");           //文本框显示空
  }
  if (PHOffGetRec_Neutral == SUCCESS)     //中性标定完成
  {
    PHOffGetRec_Neutral = FAILUED;        //置位标志位
    Scr_SendCmd("t7.txt=\"请继续标定酸性\"");
    DemarcateChice = Acidity;             //将标定状态改为酸性
    FirstCommin = 1;                      //第一次进入页面标志位，涉及到页面数据的初始化
  }
  if (PHOffGetRec_Acidity == SUCCESS)
  {
    PHOffGetRec_Acidity = FAILUED;
    Scr_SendCmd("t7.txt=\"请继续标定碱性\"");
    DemarcateChice = Basicity;
    FirstCommin = 1;
  }
  if (PHOffGetRec_Basicity == SUCCESS)
  { 
    PHOffGetRec_Basicity = FAILUED;
    Scr_SendCmd("t7.txt=\"标定成功!\"");
    Delay_ms(1000);

    FirstCommin = 1;                  //所有页面信息初始化
    WritePos[0] = 1;          
    WritePos[1] = 2;
    WriteLastPos[0] = 1;
    WriteLastPos[1] = 2;
    IsWrite = 0;
    valPoint = 0;
    DemarcateChice = Neutral;
    FirstCommin = 1;
    PHOffPage = DisMastPHOff;
    
    PageSerial = DisDatPage;          //跳转到数据显示页面
    Scr_SendCmd((uint8_t*)"page DisDat");
    
    return;
  }
  //提示中的PH值
  switch (DemarcateChice)
  {
    case Acidity:
      sprintf ((char*)SendBuf, "t3.txt=\"%s\"", "4.00");        //将值送去屏幕打印
      Scr_SendCmd(SendBuf);
      break;
    case Neutral:
      sprintf ((char*)SendBuf, "t3.txt=\"%s\"", "6.86");        //将值送去屏幕打印
      Scr_SendCmd(SendBuf);
      break;
    case Basicity:
      sprintf ((char*)SendBuf, "t3.txt=\"%s\"", "9.18");        //将值送去屏幕打印
      Scr_SendCmd(SendBuf);
      break;
    default:
      break;
  }
  
  if (PHOffPage == DisMastPHOff)                                //如果显示的是主机PH值
  {
    if(FirstCommin)
    {
      PH = ((Flash_FifoBuf[5] >> 10) & 0xf) + (((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);
      sprintf ((char*)SendBuf, "t2.txt=\"%.1f\"", PH);          //将值送去屏幕打印
      Scr_SendCmd(SendBuf);
    }

    /* 显示页面最下方的页码 */
    sprintf ((char*)SendBuf, "t26.txt=\"主机(%d/%d)\"", (int)valPoint + 1, Flash_FifoBuf[0] + 1);
    Scr_SendCmd(SendBuf);
  }
  else
  {
    if(FirstCommin)
    {
      PH = (((Flash_FifoBuf[7 + (valPoint - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (valPoint - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
      sprintf ((char*)SendBuf, "t2.txt=\"%.1f\"", PH);          //将值送去屏幕打印
      Scr_SendCmd(SendBuf);
    }
    
    sprintf ((char*)SendBuf, "t26.txt=\"%04X(%d/%d)\"",Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xffff, (int)valPoint + 1, Flash_FifoBuf[0] + 1);
    Scr_SendCmd(SendBuf);
  }
  
  switch (Key_Statue)           //判断按键
  {
    case KeyDefault:
      break;
    case KeyLF:                 //向左  x坐标减一
      if ((WritePos[1] == 1) && (WritePos[0] > 0))
      {
        WritePos[0]--;
      }
      break;
    case KeyUp:                 //向上  y坐标减一
      if (WritePos[1] == 1)
      {
        WritePos[1]--;
        WritePos[0] = 1;
      }
      else if (WritePos[1] > 0)
        WritePos[1]--;
      break;
    case KeyRG:                 //向右  x坐标加一
      if (WritePos[0] < 1)      //限制为两列
        WritePos[0]++;
      break;
    case KeyDn:                 //向下  y坐标加一
      if (WritePos[1] == 1)
      {
        WritePos[1]++;
        WritePos[0] = 1;
      }
      if (WritePos[1] < 2)      //限制为两列
          WritePos[1]++;
      break;
    case KeyOK:
      if ((WritePos[1] == 2) && (WritePos[0] == 1))      //下一页
      {
        PageSerial = TmpOfficeSetPage;
        Scr_SendCmd((uint8_t*)"page TempOffsetSet");
        FlashTmpOffPage(KeyDefault);
        
        WritePos[0] = 1;          
        WritePos[1] = 2;
        WriteLastPos[0] = 1;
        WriteLastPos[1] = 2;
        IsWrite = 0;
        valPoint = 0;
        DemarcateChice = Neutral;
        FirstCommin = 1;
        PHOffPage = DisMastPHOff;
      }

      if ((WritePos[1] == 0) && (WritePos[0] == 1))   //标定
      {
        if (PHOffPage == DisMastPHOff)
        {
          switch (DemarcateChice)
          {
            Scr_SendCmd("t2.txt=\"正在标定\"");
            PH_Neutral = GetPH();
            Delay_ms(200);
            
            case Neutral:     //中性
              DemarcateChice = Acidity;
              Scr_SendCmd("t2.txt=\"稍等继续标定酸性\"");
              break;
            case Acidity:
              DemarcateChice = Basicity;
              Scr_SendCmd("t2.txt=\"稍等继续标定碱性\"");
              break;
            case Basicity:
              DemarcateChice = Neutral;
//              OfficDatBuf[3] = ((6.86 - 4.00) / (PH_Neutral - PH_Acidity));      //中性和酸性之间
//              OfficDatBuf[4] = ((9.18 - 6.86) / (PH_Basicity - PH_Neutral));     //中性和碱性之间            
            
              /* 计算出PH酸性和碱性斜率 */
              statue = UpOffsetDat(PH_BN_Offset, ((9.18 - 6.86) / (PH_Basicity - PH_Neutral)));
              statue = UpOffsetDat(PH_AN_Offset, ((6.86 - 4.00) / (PH_Neutral - PH_Acidity)));
            
              OfficDatBuf[3] = ((6.86 - 4.00) / (PH_Neutral - PH_Acidity)) * 1000000;
              OfficDatBuf[4] = ((9.18 - 6.86) / (PH_Basicity - PH_Neutral)) * 1000000; 
            
              Scr_SendCmd("t7.txt=\"正在上传\"");
              if(TCPIsUsed == 0)
              {
                TCPIsUsed = 1;
                TIM_Cmd(TIM2, DISABLE);	      //关闭任务调度定时器
                LED_Green(ON);                //打开绿色指示灯
                
                statue = UpOffsetDat(PH_BN_Offset, OfficDatBuf[4]);   //发送数据
                
                LED_Green(OFF);               //关闭绿色指示灯
                TIM_Cmd(TIM2, ENABLE);
                TCPIsUsed = 0;  
              }
              else
              {
                Scr_SendCmd("t7.txt=\"当前网络忙，请稍后再试\"");
              }
              
              
              if(TCPIsUsed == 0)
              {
                TCPIsUsed = 1;
                TIM_Cmd(TIM2, DISABLE);	      //关闭任务调度定时器
                LED_Green(ON);                //打开绿色指示灯
                
                statue = UpOffsetDat(PH_AN_Offset, OfficDatBuf[3]);
                
                if (statue == SUCCESS)
                Scr_SendCmd("t2.txt=\"标定成功\"");
                else
                  Scr_SendCmd("t7.txt=\"数据上传失败，请重试\"");
              
                LED_Green(OFF);               //关闭绿色指示灯
                TIM_Cmd(TIM2, ENABLE);
                TCPIsUsed = 0;  
              }
              else
              {
                Scr_SendCmd("t7.txt=\"当前网络忙，请稍后再试\"");
              }
              
              Delay_ms(1000);
            
              WritePos[0] = 1;              //初始化标志位
              WritePos[1] = 2;
              WriteLastPos[0] = 1;
              WriteLastPos[1] = 2;
              IsWrite = 0;
              valPoint = 0;
              DemarcateChice = Neutral;
              FirstCommin = 1;
              PHOffPage = DisMastPHOff;
              
              PageSerial = DisDatPage;      //跳转页面到数据显示页面
              Scr_SendCmd((uint8_t*)"page DisDat");

              FlashPage();
              break;
            default:
              break;
          }
        }
        else
        {
          if (Connect_Way == USE_CAN)
          {
            Can_DatBuf[1] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 24) & 0xff);
            Can_DatBuf[2] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 16) & 0xff);
            Can_DatBuf[3] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 8) & 0xff);
            Can_DatBuf[4] = (Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xff);
            Can_DatBuf[5] = 0xff;
            Can_DatBuf[6] = 0xff;
            switch (DemarcateChice)
            {
              Scr_SendCmd("t2.txt=\"正在标定\"");
              case Neutral:     //中性
                Can_DatBuf[7] = 0xd0;
                break;
              case Acidity:
                Can_DatBuf[7] = 0xd1;
                break;
              case Basicity:
                Can_DatBuf[7] = 0xd2;
                break;
              default:
                break;
            }
            Can_Send_Msg(Can_DatBuf, 8);   
            FirstCommin = 0;
          }
          else
          {
            ZGB_DatBuf[1] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 24) & 0xff);
            ZGB_DatBuf[2] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 16) & 0xff);
            ZGB_DatBuf[3] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 8) & 0xff);
            ZGB_DatBuf[4] = (Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xff);
            ZGB_DatBuf[5] = 0xff;
            ZGB_DatBuf[6] = 0xff;
            ZGB_DatBuf[7] = 0xff;
            ZGB_DatBuf[8] = 0xff;
            ZGB_DatBuf[9] = 0xff;
            ZGB_DatBuf[11] = 0x02;
            ZGB_DatBuf[12] = 0xcc;
            ZGB_DatBuf[13] = 0xcc;
            switch (DemarcateChice)
            {
              Scr_SendCmd("t2.txt=\"正在标定\"");
              case Neutral:     //中性
                ZGB_DatBuf[10] = 0x01;
                break;
              case Acidity:
                ZGB_DatBuf[10] = 0x02;
                break;
              case Basicity:
                ZGB_DatBuf[10] = 0x03;
                break;
              default:
                break;
            }
            USART2_SendBuf(ZGB_DatBuf);       //向从机发送校验包
            USART2_SendByte('\n');     
            FirstCommin = 0;
            
            Scr_SendCmd("t7.txt=\"若15s无反应，重新点击标定按钮\"");
          }
        }
      }
      
      if (WritePos[1] == 1)      //上一设备或下一设备
      {
        switch (WritePos[0])
        {
          case 0:     //上一设备
            if (valPoint > 0)
            {
              valPoint--;
              DemarcateChice = Neutral;
              FirstCommin = 1;
              Scr_SendCmd("t2.txt=\"正在获取\"");
            }
            if (valPoint == 0)
            {
              PHOffPage = DisMastPHOff;
            }                
            break;
          case 1:     //下一设备
            if (valPoint < Flash_FifoBuf[0])
            {
              valPoint++;
              DemarcateChice = Neutral;
              FirstCommin = 1;
              Scr_SendCmd("t2.txt=\"正在获取\"");
              PHOffPage = !DisMastPHOff;
            }
            break;
        }
      }
      break;
  }
  
  if (Key_Statue != KeyOK)                                                //如果按下的不是OK键
  {
    if ((WriteLastPos[1] == 1) || (WriteLastPos[1] == 2))                 //保存和下一页字体的颜色是紫色的
      PHOffFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Popul);      
    else
    {
      PHOffFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Black);     //恢复之前所在的按键夜色
    }   
    
    PHOffFlashPos(WritePos[0], WritePos[1], Color_Yellow);              //将现在所在的空间变成黄色
    WriteLastPos[0] = WritePos[0];                                        //更新上一次所在位置
    WriteLastPos[1] = WritePos[1];
  }
}


/*
 * 函数名：FlashTmpOffPage
 * 描述  ：温度补偿刷新页面
 * 输入  ：Key_Statue：按键状态
 * 返回  ：无
 */
void FlashTmpOffPage (uint8_t Key_Statue)
{
  uint8_t statue = 0;
  float WaterTemp;
  uint8_t SendBuf[30];      //向屏幕发送的buf  
  static int8_t WritePos[2] = {0, 2};   //光标所在点    
  static int8_t WriteLastPos[2] = {0, 2};   //光标上次所在点
  static uint8_t IsWrite = 0;     //读写状态
  static float TmpSetVal = 0;   //实际温度设置值  
  float DifTmp = 0.0;             //温度差值
  static int8_t valPoint = 0;     //页面编号
  static uint8_t FirstCommin = 1; //第一次进入页面
  
  u8 ZGB_DatBuf[14] = {0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};   //ZGB包
  u8 Can_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};    //CAN发送用的数组
  
  if (TmpOffGetRec == SUCCESS)
  {
    Scr_SendCmd("t7.txt=\"设置成功!\"");
    
    Delay_ms(1000);
    Scr_SendCmd("t7.txt=\"数据将在一段时间后更新\"");
    TmpOffGetRec = FAILUED;
    Delay_ms(1000);
    
    FirstCommin = 1;
    WritePos[0] = 0;          
    WritePos[1] = 2;
    WriteLastPos[0] = 0;
    WriteLastPos[1] = 2;
    IsWrite = 0;
    valPoint = 0;
    TmpOffPage = DisMastTmpOff;
    
    PageSerial = DisDatPage;
    Scr_SendCmd((uint8_t*)"page DisDat");
    return;
  }
  
  if (TmpOffPage == DisMastTmpOff)    //如果显示的是主机温度值
  {
    WaterTemp = ((Flash_FifoBuf[5] & 0x3f) << 2) + ((Flash_FifoBuf[6] >> 30) & 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0);
    sprintf ((char*)SendBuf, "t2.txt=\"%.1f\"", WaterTemp);        //将值送去屏幕打印
    Scr_SendCmd(SendBuf);

    if (FirstCommin)
    {
      FirstCommin = 0;
      TmpSetVal = WaterTemp;
    }
    sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", TmpSetVal);        //将值送去屏幕打印
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t26.txt=\"主机(%d/%d)\"", (int)valPoint + 1, Flash_FifoBuf[0] + 1);
    Scr_SendCmd(SendBuf);
  }
  else        //如果显示的是从机的温度
  {
    sprintf ((char*)SendBuf, "t26.txt=\"%04X(%d/%d)\"",Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xffff, (int)valPoint + 1, Flash_FifoBuf[0] + 1);
    Scr_SendCmd(SendBuf);

    WaterTemp = (((Flash_FifoBuf[7 + (valPoint - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (valPoint - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
    sprintf ((char*)SendBuf, "t2.txt=\"%.1f\"", WaterTemp);        //将值送去屏幕打印
    Scr_SendCmd(SendBuf);
    
    if (FirstCommin)
    {
      FirstCommin = 0;
      TmpSetVal = WaterTemp;
    }
    
    sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", TmpSetVal);        //将值送去屏幕打印
    Scr_SendCmd(SendBuf);
  }
  
  if(Key_Statue != KeyDefault)
  {
    Scr_SendCmd("t7.txt=\"\"");
  }
  if (IsWrite == 0)               //移动光标状态
  {
    switch (Key_Statue)           //判断按键
    {
      case KeyDefault:
        break;
      case KeyLF:                 //向左  x坐标减一
          if (WritePos[0] > 0)
            WritePos[0]--;
        break;
      case KeyUp:                 //向上  y坐标减一
          if (WritePos[1] > 0)
          WritePos[1]--;
        break;
      case KeyRG:                 //向右  x坐标加一
        if (WritePos[0] < 1)      //限制为两列
          WritePos[0]++;
        break;
      case KeyDn:                 //向下  y坐标加一
        if (WritePos[1] < 2)      //限制为两列
            WritePos[1]++;
        break;
      case KeyOK:
        if (WritePos[1] == 2)      //上一页或下一页
        {
          switch (WritePos[0])
          {
            case 0:
              PageSerial = PHOfficeSetPage;
              Scr_SendCmd((uint8_t*)"page PHOffsetSet");
              FlashPHOffPage(KeyDefault);
              break;
            case 1:
              PageSerial = TimeSetPage;
              Scr_SendCmd((uint8_t*)"page TimeSet");
              FlashTimeSetPage(KeyDefault); 
              break;
          }
          FirstCommin = 1;
          WritePos[0] = 0;          
          WritePos[1] = 2;
          WriteLastPos[0] = 0;
          WriteLastPos[1] = 2;
          IsWrite = 0;
          valPoint = 0;
          TmpOffPage = DisMastTmpOff;
        }
        if ((WritePos[1] == 0) && (WritePos[0] == 1))   //确定
        {
          if (TmpOffPage == DisMastTmpOff)    //如果是主机按下的
          {
            DifTmp = TmpSetVal - WaterTemp;
            DifTmp += OfficDatBuf[0] / 10.0;
            
            Scr_SendCmd("t7.txt=\"正在上传\"");
            
            if(TCPIsUsed == 0)
            {
              TCPIsUsed = 1;
              TIM_Cmd(TIM2, DISABLE);	      //关闭任务调度定时器
              LED_Green(ON);                //打开绿色指示灯
              
              statue = UpOffsetDat(Tmp_Offset, DifTmp);
              
              if (statue == SUCCESS)
              {
                OfficDatBuf[0] = DifTmp * 10;
                Scr_SendCmd("t7.txt=\"已补偿完成\"");
              }
              else
                Scr_SendCmd("t7.txt=\"数据上传失败，请重试\"");
              
              UserDelay_KeyScan(1000);
              
              LED_Green(OFF);               //关闭绿色指示灯
              TIM_Cmd(TIM2, ENABLE);
              TCPIsUsed = 0;  
            }
            else
            {
              Scr_SendCmd("t7.txt=\"当前网络忙，请稍后再试\"");
              Delay_ms(1000);
            }
            
            FirstCommin = 1;
            WritePos[0] = 0;
            WritePos[1] = 2;
            WriteLastPos[0] = 0;
            WriteLastPos[1] = 2;
            IsWrite = 0;
            valPoint = 0;
            TmpOffPage = DisMastTmpOff;
            PageSerial = DisDatPage;
            Scr_SendCmd((uint8_t*)"page DisDat");
            return;
          }
          else        //从机温度设置
          {
            DifTmp = TmpSetVal - WaterTemp;
            DifTmp = DifTmp * 10;    //放大十倍
            TmpOffGetRec = FAILUED;   //标志位置位
            
            if (Connect_Way == USE_CAN)   //如果是CAN方式通信
            {
              Can_DatBuf[1] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 24) & 0xff);
              Can_DatBuf[2] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 16) & 0xff);
              Can_DatBuf[3] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 8) & 0xff);
              Can_DatBuf[4] = (Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xff);
              Can_DatBuf[5] = (((int)DifTmp >> 24) & 0xff);
              Can_DatBuf[6] = (((int)DifTmp >> 16) & 0xff);
              Can_DatBuf[7] = 0xc0;
              Can_Send_Msg(Can_DatBuf, 8);
              
              UserDelay_KeyScan(100);
              Can_DatBuf[5] = (((int)DifTmp >> 8) & 0xff);
              Can_DatBuf[6] = ((int)DifTmp & 0xff);
              Can_DatBuf[7] = 0xc1;
              Can_Send_Msg(Can_DatBuf, 8);

              Scr_SendCmd("t7.txt=\"若15s无反应，重新点击补偿按钮\"");
            }
            else    //ZGB方式通信
            {
              ZGB_DatBuf[1] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 24) & 0xff);
              ZGB_DatBuf[2] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 16) & 0xff);
              ZGB_DatBuf[3] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 8) & 0xff);
              ZGB_DatBuf[4] = (Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xff);
              if (DifTmp < 0)
              {
                DifTmp = -DifTmp;
                ZGB_DatBuf[9] = 0x01;
              }
              else
                ZGB_DatBuf[9] = 0x02;
                
              ZGB_DatBuf[5] = ((((int)DifTmp >> 24) & 0xff) + 1);
              ZGB_DatBuf[6] = ((((int)DifTmp >> 16) & 0xff) + 1);
              ZGB_DatBuf[7] = ((((int)DifTmp >> 8) & 0xff) + 1);
              ZGB_DatBuf[8] = (((int)DifTmp & 0xff) + 1);
              ZGB_DatBuf[10] = 0xff;
              ZGB_DatBuf[11] = 0x01;
              ZGB_DatBuf[12] = 0xcc;
              ZGB_DatBuf[13] = 0xcc;
              
              USART2_SendBuf(ZGB_DatBuf);       //向从机发送校验包
              USART2_SendByte('\n');
              
              Scr_SendCmd("t7.txt=\"正在与从机通信\"");
              UserDelay_KeyScan(500);
              
              Scr_SendCmd("t7.txt=\"若15s无反应，重新点击补偿按钮\"");
            }
          }
        }
        
        if (WritePos[1] == 1)      //上一设备或下一设备
        {
          switch (WritePos[0])
          {
            case 0:     //上一设备
              
              if (valPoint > 0)
              {
                valPoint--;
                FirstCommin = 1;
                Scr_SendCmd("t2.txt=\"正在获取\"");
                Scr_SendCmd("t3.txt=\"正在获取\"");
              }
              if (valPoint == 0)
              {
                TmpOffPage = DisMastTmpOff;
              }                
              break;
            case 1:     //下一设备
              if (valPoint < Flash_FifoBuf[0])
              {
                valPoint++;
                FirstCommin = 1;
                Scr_SendCmd("t2.txt=\"正在获取\"");
                Scr_SendCmd("t3.txt=\"正在获取\"");
                TmpOffPage = !DisMastTmpOff;
              }
              break;
          }
        }

        if ((WritePos[1] == 0) && (WritePos[0] == 0))     //按下的是实际水温的值
        {
          TmpOffFlashPos(WritePos[0], WritePos[1], Color_Red);
          IsWrite = 1;     
        }
        break;
       }
      if (Key_Statue != KeyOK)                                                //如果按下的不是OK键
      {
        if ((WriteLastPos[1] == 1) || (WriteLastPos[1] == 2))                 //保存和下一页字体的颜色是紫色的
          TmpOffFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Popul);      
        else
        {
          TmpOffFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Black);     //恢复之前所在的按键夜色
        }   
        
        TmpOffFlashPos(WritePos[0], WritePos[1], Color_Yellow);              //将现在所在的空间变成黄色
        WriteLastPos[0] = WritePos[0];                                        //更新上一次所在位置
        WriteLastPos[1] = WritePos[1];
      }
  }
  else
  {
    /* 实际水温 */
    if ((WritePos[0] == 0) && (WritePos[1] == 0))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          TmpSetVal += 0.5;                                                      
          if (TmpSetVal > 100)                                                
            TmpSetVal = 100; 
        
          sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", TmpSetVal);               
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          TmpSetVal -= 0.5;
          if (TmpSetVal < 0)
            TmpSetVal = 0;
          
          sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", TmpSetVal);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t3.pco=YELLOW");
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
  }
}

/*
 * 函数名：FlashTimeSetPage
 * 描述  ：时间设置页面刷新
 * 输入  ：无
 * 返回  ：无
 */
void FlashTimeSetPage (uint8_t Key_Statue)
{
  static int8_t WritePos[2] = {0, 2};
  static int8_t WriteLastPos[2] = {0, 2};
  static uint8_t IsWrite = 0;
  uint8_t SendBuf[50];
  static uint8_t FistCominTemp = 1;
  static int year, month, day, hour, minute, second;
  
  if (FistCominTemp)
  {
    sprintf ((char*)SendBuf, "t14.txt=\"%s\"", ChipID_String);    //显示设备ID
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t1.txt=\"%4d\"", calendar.w_year);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t3.txt=\"%2d\"", calendar.w_month);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", calendar.w_date);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t7.txt=\"%2d\"", calendar.hour);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t9.txt=\"%2d\"", calendar.min);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t11.txt=\"%2d\"", calendar.sec);
    Scr_SendCmd(SendBuf);
    
    year = calendar.w_year;
    month = calendar.w_month;
    day = calendar.w_date;
    hour = calendar.hour;
    minute = calendar.min;
    second = calendar.sec;
    
    FistCominTemp = 0;
  }
  
  if (IsWrite == 0)               //移动光标状态
  {
    switch (Key_Statue)           //判断按键
    {
      case KeyDefault:
        break;
      case KeyLF:                 //向左  x坐标减一
          if (WritePos[0] > 0)
            WritePos[0]--;
//          if ((WritePos[0] == 1) && (WritePos[1] == 2))
//            WritePos[0] = 0;
        break;
      case KeyUp:                 //向上  y坐标减一
          if (WritePos[1] > 0)
          WritePos[1]--;
        break;
      case KeyRG:                 //向右  x坐标加一
        if (WritePos[0] < 2)      //限制为两列
          WritePos[0]++;
//        if ((WritePos[0] == 1) && (WritePos[1] == 2))
//          WritePos[0] = 2;
        break;
      case KeyDn:                 //向下  y坐标加一
        if (WritePos[1] < 2)      //限制为两列
            WritePos[1]++;
//        if ((WritePos[0] == 1) && (WritePos[1] == 2))
//          WritePos[1]--;
        break;
      case KeyOK:
        if (WritePos[1] == 2)     //保存或上一页
        {
          switch (WritePos[0])
          {
            case 0:
              PageSerial = TmpOfficeSetPage;
              Scr_SendCmd((uint8_t*)"page TempOffsetSet");
              FlashTmpOffPage(KeyDefault);
              break;
            case 1:
              RTC_Set(year, month, day, hour, minute, second);
              PageSerial = DisDatPage;
              DatPage = 0; 
              Scr_SendCmd((uint8_t*)"page DisDat");
              FlashDatPage(); 
              break;
            case 2:
              PageSerial = LimSetPage;
              Scr_SendCmd((uint8_t*)"page LimSet");
              FlashLimSetPage(KeyDefault);
              break;
          }
//          if(WritePos[0] == 0)    //保存
//          {
//            RTC_Set(year, month, day, hour, minute, second);
//            PageSerial = DisDatPage;
//            DatPage = 0; 
//            Scr_SendCmd((uint8_t*)"page DisDat");
//            FlashDatPage(); 
//          }
//          else                    //下一页
//          {
//            PageSerial = LimSetPage;
//            Scr_SendCmd((uint8_t*)"page LimSet");
//            FlashLimSetPage(KeyDefault);
//          }
          FistCominTemp = 1;
          WritePos[0] = 0;          
          WritePos[1] = 2;
          WriteLastPos[0] = 0;
          WriteLastPos[1] = 2;
        }
        else
        {
          TimeSetFlashPos(WritePos[0], WritePos[1], Color_Red);
          IsWrite = 1;     
        }
        
        break;
    }
    
    if (Key_Statue != KeyOK)                                                //如果按下的不是OK键
    {
      if (WriteLastPos[1] == 2)                                             //保存和下一页字体的颜色是紫色的
        TimeSetFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Popul);      
      else
      {
        TimeSetFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Black);     //恢复之前所在的按键夜色
      }   
      
      TimeSetFlashPos(WritePos[0], WritePos[1], Color_Yellow);              //将现在所在的空间变成黄色
      WriteLastPos[0] = WritePos[0];                                        //更新上一次所在位置
      WriteLastPos[1] = WritePos[1];
    }
  }
  else
  {
    /* 年份 */
    if ((WritePos[0] == 0) && (WritePos[1] == 0))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          year += 1;                                                      
          if (year > 2099)                                                
            year = 2000; 
        
          sprintf ((char*)SendBuf, "t1.txt=\"%4d\"", year);               
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          year -= 1;
          if (year < 2000)
            year = 2099;
          
          sprintf ((char*)SendBuf, "t1.txt=\"%4d\"", year);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t1.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    /* 月份 */
    if ((WritePos[0] == 1) && (WritePos[1] == 0))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          month += 1;                                           
          if (month > 12)                                     
            month = 1; 
          
          if (((month == 2) || (month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day == 31))
            day = 30;
            
          sprintf ((char*)SendBuf, "t3.txt=\"%2d\"", month);   
          Scr_SendCmd(SendBuf);
          
          sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", day);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          month -= 1;
          if (month < 1)
            month = 12;
          
          if (((month == 2) || (month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day == 31))
            day = 30;
          
          sprintf ((char*)SendBuf, "t3.txt=\"%2d\"", month);  
          Scr_SendCmd(SendBuf);
          
          sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", day);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t3.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    /* 日期 */
    if ((WritePos[0] == 2) && (WritePos[1] == 0))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          day += 1;                                           
          if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))
          {
            if (day > 31)
              day = 1;
          }
          else
          {
            if (day > 30)
              day = 1;
          }        
          sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", day);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          day -= 1;
          if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))
          {
            if (day < 1)
              day = 31;
          }
          else
          {
            if (day < 1)
              day = 30;
          }  
          
          sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", day);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t5.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    /* 小时 */
    if ((WritePos[0] == 0) && (WritePos[1] == 1))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          hour += 1;                                           
          if (hour > 23)                                      
            hour = 0; 
        
          sprintf ((char*)SendBuf, "t7.txt=\"%2d\"", hour);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          hour -= 1;
          if (hour < 0)
            hour = 23;
          
          sprintf ((char*)SendBuf, "t7.txt=\"%2d\"", hour);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t7.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    /* 分钟 */
    if ((WritePos[0] == 1) && (WritePos[1] == 1))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          minute += 1;                                           
          if (minute > 59)                                     
            minute = 0; 
        
          sprintf ((char*)SendBuf, "t9.txt=\"%2d\"", minute);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          minute -= 1;
          if (minute < 0)
            minute = 59;
          
          sprintf ((char*)SendBuf, "t9.txt=\"%2d\"", minute);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t9.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    if ((WritePos[0] == 2) && (WritePos[1] == 1))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          second += 1;                                       
          if (second > 59)                                   
            second = 0; 
        
          sprintf ((char*)SendBuf, "t11.txt=\"%2d\"", second);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          second -= 1;
          if (second < 0)
            second = 59;
          
          sprintf ((char*)SendBuf, "t11.txt=\"%2d\"", second);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t11.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
  }
}

/*
 * 函数名：LimPage_FlashPos
 * 描述  ：阈值设置页面的数据颜色显示
 * 输入  ：Pos_x：要修改的控件的x坐标位置
 *         Pos_y：要修改的空间的y坐标位置
 *         Color：显示的颜色
 * 返回  ：无
 */
void LimPage_FlashPos (int8_t Pos_x, int8_t Pos_y, uint8_t Color)
{
  /* 室温上限 */
  if ((Pos_x == 0) && (Pos_y == 0))  
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t3.pco=YELLOW");   //显示黄色
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t3.pco=RED");      //显示红色
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t3.pco=BLACK");      //显示黑色
      default:
        break;
    }
  }   
  /* 室温下限 */
  if ((Pos_x == 1) && (Pos_y == 0))
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t4.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t4.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t4.pco=BLACK");
      default:
        break;
    }
  } 
  /* 水温上限 */
  if ((Pos_x == 0) && (Pos_y == 1)) 
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t5.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t5.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t5.pco=BLACK");
      default:
        break;
    }
  } 
  /* 水温下限 */
  if ((Pos_x == 1) && (Pos_y == 1))
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t6.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t6.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t6.pco=BLACK");
      default:
        break;
    }
  } 
  /* 泵状态上限 */
  if ((Pos_x == 0) && (Pos_y == 2))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t7.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t7.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t7.pco=BLACK");
      default:
        break;
    }
  } 
  /* 泵状态下限 */
  if ((Pos_x == 1) && (Pos_y == 2)) 
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t8.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t8.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t8.pco=BLACK");
      default:
        break;
    }
  } 
  /* PH值上限 */
  if ((Pos_x == 0) && (Pos_y == 3))   
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t9.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t9.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t9.pco=BLACK");
      default:
        break;
    }
  } 
  /* PH值下限 */
  if ((Pos_x == 1) && (Pos_y == 3))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t13.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t13.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t13.pco=BLACK");
      default:
        break;
    }
  } 
  /* 保存 */
  if ((Pos_x == 0) && (Pos_y == 4))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t15.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t15.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t15.pco=BLACK");
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t15.pco=32799");   //紫色
        break;
      default:
        break;
    }
  } 
  /* 下一页 */
  if ((Pos_x == 1) && (Pos_y == 4))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t23.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t23.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t23.pco=BLACK");
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t23.pco=32799");   //紫色
        break;
      default:
        break;
    }
  } 
  /* 上一页 */
  if ((Pos_x == -1) && (Pos_y == 4))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t22.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t22.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t22.pco=BLACK");
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t22.pco=32799");   //紫色
        break;
      default:
        break;
    }
  }
}

/*
 * 函数名：FlashLimSetPage
 * 描述  ：刷新阈值设置页面
 * 输入  ：Key_Statue：按下的按键
 * 返回  ：无
 */
void FlashLimSetPage (uint8_t Key_Statue)
{
  static int8_t WritePos[2] = {-1, 4};          //[0]表示X坐标  [1]表示Y坐标
  static int8_t WriteLastPos[2] = {-1, 4};      //[0]表示上一次X坐标  [1]表示上一次Y坐标
  static uint8_t IsWrite = 0;                   //用户正在设定    0:否    1：是
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;       //各参数上下限
  static float PHMax_Set, PHMin_Set, WaterTempMax_Set, WaterTempMin_Set, EnvirTempMax_Set, EnvirTempMin_Set, WaterSpeedMax_Set, WaterSpeedMin_Set; //各参数上下限
  uint8_t SendBuf[15];                          //要发送给屏幕的字符串
  static uint8_t FistCominTemp = 1;             //是否是进入该页面而不是之前就已经在本页面
  uint8_t TempInt, TempFloat;                   //存放阈值时的临时变量
  
  if (FistCominTemp)                            //如果本次进入函数是第一次打开页面
  {
    /* 先获取设置的各参数的上下限 */
    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
    WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
    WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
    EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
    EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
    WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
    WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
    
    /* 将阈值信息赋值给设定值 */
    PHMax_Set = PHMax;
    PHMin_Set = PHMin;
    WaterTempMax_Set = WaterTempMax;
    WaterTempMin_Set = WaterTempMin;
    EnvirTempMax_Set = EnvirTempMax;
    EnvirTempMin_Set = EnvirTempMin;
    WaterSpeedMax_Set = WaterSpeedMax;
    WaterSpeedMin_Set = WaterSpeedMin;
    
    /* 将当前阈值显示 */
    sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", EnvirTempMax_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t4.txt=\"%.1f\"", EnvirTempMin_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t5.txt=\"%.1f\"", WaterTempMax_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t6.txt=\"%.1f\"", WaterTempMin_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t7.txt=\"%.1f\"", WaterSpeedMax_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t8.txt=\"%.1f\"", WaterSpeedMin_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t9.txt=\"%.1f\"", PHMax_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t13.txt=\"%.1f\"", PHMin_Set);
    Scr_SendCmd(SendBuf);
    
    FistCominTemp = 0;                              //置位标志位，下一次刷新页面时不会再做这些
  }

  if (IsWrite == 0)                                 //如果不是在编辑状态，就是移动光标
  {
    switch (Key_Statue)                             //判断按键
    {
      case KeyDefault:
        break;
      case KeyLF:                                   //向左   x坐标减一
        if (WritePos[1] == 4)
        {
          if (WritePos[0] > -1)
            WritePos[0]--;
        }
        else
        {
          if (WritePos[0] > 0)
            WritePos[0]--;
        }
        break;
      case KeyUp:                                   //向上  y坐标减一
        if ((WritePos[0] != -1) || (WritePos[1] != 4))
        {
          if (WritePos[1] > 0)
          WritePos[1]--;
        }
        break;
      case KeyRG:                                   //向右  x坐标加一
        if (WritePos[0] < 1)                        //限制为两列
          WritePos[0]++;
        break;
      case KeyDn:                                   //向下  y坐标加一
        if (WritePos[1] < 4)                        //限制为两列
          WritePos[1]++;
        break;
      case KeyOK:
        if(WritePos[1] == 4)                        //如果是在保存或下一页这一行
        {
          if (WritePos[0] == 0)                     //保存
          {
            /* 板温上限保存 */
            TempInt = (int)EnvirTempMax_Set;
            TempFloat = ((int)(EnvirTempMax_Set * 10) % 10); 
            Flash_FifoBuf[2] &= ~(0xff << 16);
            Flash_FifoBuf[2] |= ((TempInt & 0xff) << 16);                   //上限设置
            Flash_FifoBuf[2] &= ~(0xf << 12);
            Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 12);
            
            /* 板温下限保存 */
            TempInt = (int)EnvirTempMin_Set;
            TempFloat = ((int)(EnvirTempMin_Set * 10) % 10); 
            Flash_FifoBuf[2] &= ~(0xff << 4);
            Flash_FifoBuf[2] |= ((TempInt & 0xff) << 4);                    //下限设置
            Flash_FifoBuf[2] &= ~(0xf << 0);
            Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 0);
            
            /* 水温上限保存 */
            TempInt = (int)WaterTempMax_Set;
            TempFloat = ((int)(WaterTempMax_Set * 10) % 10); 
            
            Flash_FifoBuf[1] &= ~(0xff << 8);
            Flash_FifoBuf[1] |= ((TempInt & 0xff) << 8);                    //上限设置        
            Flash_FifoBuf[1] &= ~(0xf << 4);
            Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 4);
          
            /* 水温下限保存 */
            TempInt = (int)WaterTempMin_Set;
            TempFloat = ((int)(WaterTempMin_Set * 10) % 10); 
            
            Flash_FifoBuf[1] &= ~(0xf);
            Flash_FifoBuf[1] |= ((TempInt >> 4)& 0xf);                      //下限设置
            Flash_FifoBuf[2] &= ~(0xff << 24);
            Flash_FifoBuf[2] |= ((TempInt & 0xf) << 28);
            Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 24);      
      
            /* 泵状态上限保存 */
            TempInt = (int)WaterSpeedMax_Set;
            TempFloat = ((int)(WaterSpeedMax_Set * 10) % 10); 
            
            Flash_FifoBuf[3] &= ~(0xff << 24);
            Flash_FifoBuf[3] |= ((TempInt & 0xff) << 24);                   //上限设置
            Flash_FifoBuf[3] &= ~(0xf << 20);
            Flash_FifoBuf[3] |= ((TempFloat & 0xf) << 20);
      
            /* 泵状态下限保存 */
            TempInt = (int)WaterSpeedMin_Set;
            TempFloat = ((int)(WaterSpeedMin_Set * 10) % 10); 
            
            Flash_FifoBuf[3] &= ~(0xff << 12);
            Flash_FifoBuf[3] |= ((TempInt & 0xff) << 12);                   //下限
            Flash_FifoBuf[3] &= ~(0xf << 8);
            Flash_FifoBuf[3] |= ((TempFloat & 0xf) << 8);
      
            /* PH上限保存 */
            TempInt = (int)PHMax_Set;
            TempFloat = ((int)(PHMax_Set * 10) % 10); 
            
            Flash_FifoBuf[1] &= ~(0xff << 24);
            Flash_FifoBuf[1] |= ((TempInt & 0xf) << 28);                    //上限
            Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 24);
      
            /* PH下限保存 */
            TempInt = (int)PHMin_Set;
            TempFloat = ((int)(PHMin_Set * 10) % 10); 
            
            Flash_FifoBuf[1] &= ~(0xff << 16);
            Flash_FifoBuf[1] |= ((TempInt & 0xf) << 20);                    //下限
            Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 16);  
            
            PageSerial = DisDatPage;                                        //跳转到数据显示页面
            DatPage = 0;                                                    //标志位重置
            Scr_SendCmd((uint8_t*)"page DisDat");                           //跳转页面            
            FlashDatPage();                                                 //刷新页面            
          }
            
          if (WritePos[0] == -1)                                            //打开时间设置页面
          {
            PageSerial = TimeSetPage;
            Scr_SendCmd((uint8_t*)"page TimeSet");                          //跳转页面  
            FlashTimeSetPage(KeyDefault);             
          }
          
          if (WritePos[0] == 1)                                             //下一页
          {
            PageSerial = DisDatPage;                                        //跳转到数据显示页面
            DatPage = 0;                                                    //标志位重置
            Scr_SendCmd((uint8_t*)"page DisDat");                           //跳转页面  
            FlashDatPage();                                                 //刷新页面                      
          }
          
          FistCominTemp = 1;
          /* 置位位置标志位，以便下次打开本页面 */
          WritePos[0] = -1;          
          WritePos[1] = 4;
          WriteLastPos[0] = -1;
          WriteLastPos[1] = 4;
        }
        else
        {
          LimPage_FlashPos(WritePos[0], WritePos[1], Color_Red);            //将选中的控件变红
          IsWrite = 1;                                                      //将读写位置1，表示当前是修改值状态
        }
        break;
      default:
        break;
    }
    
    if (Key_Statue != KeyOK)                                                //如果按下的不是OK键
    {
      if (WriteLastPos[1] == 4)                                             //保存和下一页字体的颜色是紫色的
        LimPage_FlashPos(WriteLastPos[0], WriteLastPos[1], Color_Popul);      
      else
      {
        LimPage_FlashPos(WriteLastPos[0], WriteLastPos[1], Color_Black);    //恢复之前所在的按键夜色
      }   
      
      LimPage_FlashPos(WritePos[0], WritePos[1], Color_Yellow);             //将现在所在的空间变成黄色
      WriteLastPos[0] = WritePos[0];                                        //更新上一次所在位置
      WriteLastPos[1] = WritePos[1];
    }
  }
  else                                    //用户正在编辑
  {
    if ((WritePos[0] == 0) && (WritePos[1] == 0))                           //室温上限设置
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          EnvirTempMax_Set += 1;                                            //室温加1
          if (EnvirTempMax_Set > 100)                                       //限幅100
            EnvirTempMax_Set = 100; 
        
          sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", EnvirTempMax_Set);    //发送给屏幕
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          EnvirTempMax_Set -= 1;
          if (EnvirTempMax_Set <= 0)
            EnvirTempMax_Set = 0;
          
          sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", EnvirTempMax_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t3.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
      
    }   
    
    if ((WritePos[0] == 1) && (WritePos[1] == 0))                            //室温下限设置
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          EnvirTempMin_Set += 1;
          if (EnvirTempMin_Set > 100)
            EnvirTempMin_Set = 100;
        
          sprintf ((char*)SendBuf, "t4.txt=\"%.1f\"", EnvirTempMin_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          EnvirTempMin_Set -= 1;
          if (EnvirTempMin_Set <= 0)
            EnvirTempMin_Set = 0;
          
          sprintf ((char*)SendBuf, "t4.txt=\"%.1f\"", EnvirTempMin_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t4.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
      
    if ((WritePos[0] == 0) && (WritePos[1] == 1))                             //水温上限设置
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          WaterTempMax_Set += 1;
          if (WaterTempMax_Set > 100)
            WaterTempMax_Set = 100;
        
          sprintf ((char*)SendBuf, "t5.txt=\"%.1f\"", WaterTempMax_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          WaterTempMax_Set -= 1;
          if (WaterTempMax_Set <= 0)
            WaterTempMax_Set = 0;
          
          sprintf ((char*)SendBuf, "t5.txt=\"%.1f\"", WaterTempMax_Set);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t5.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }  
    
    if ((WritePos[0] == 1) && (WritePos[1] == 1))                               //水温下限设置
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          WaterTempMin_Set += 1;
          if (WaterTempMin_Set > 100)
            WaterTempMin_Set = 100;
        
          sprintf ((char*)SendBuf, "t6.txt=\"%.1f\"", WaterTempMin_Set); 
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          WaterTempMin_Set -= 1;
          if (WaterTempMin_Set <= 0)
            WaterTempMin_Set = 0;
          
          sprintf ((char*)SendBuf, "t6.txt=\"%.1f\"", WaterTempMin_Set); 
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t6.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }  
      
    if ((WritePos[0] == 0) && (WritePos[1] == 2))                               //泵状态上限设置
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          WaterSpeedMax_Set += 1;
          if (WaterSpeedMax_Set > 100)
            WaterSpeedMax_Set = 100;
        
          sprintf ((char*)SendBuf, "t7.txt=\"%.1f\"", WaterSpeedMax_Set); 
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          WaterSpeedMax_Set -= 1;
          if (WaterSpeedMax_Set <= 0)
            WaterSpeedMax_Set = 0;
          
          sprintf ((char*)SendBuf, "t7.txt=\"%.1f\"", WaterSpeedMax_Set); 
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t7.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    if ((WritePos[0] == 1) && (WritePos[1] == 2))                               //泵状态下限设置
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          WaterSpeedMin_Set += 1;
          if (WaterSpeedMin_Set > 100)
            WaterSpeedMin_Set = 100;
        
          sprintf ((char*)SendBuf, "t8.txt=\"%.1f\"", WaterSpeedMin_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          WaterSpeedMin_Set -= 1;
          if (WaterSpeedMin_Set <= 0)
            WaterSpeedMin_Set = 0;
          
          sprintf ((char*)SendBuf, "t8.txt=\"%.1f\"", WaterSpeedMin_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t8.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
      
    if ((WritePos[0] == 0) && (WritePos[1] == 3))                                //PH上限设置
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          PHMax_Set += 1;
          if (PHMax_Set > 16)
            PHMax_Set = 16;
        
          sprintf ((char*)SendBuf, "t9.txt=\"%.1f\"", PHMax_Set);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          PHMax_Set -= 1;
          if (PHMax_Set <= 0)
            PHMax_Set = 0;
          
          sprintf ((char*)SendBuf, "t9.txt=\"%.1f\"", PHMax_Set);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t9.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    } 
    if ((WritePos[0] == 1) && (WritePos[1] == 3))                                 //PH下限设置   
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          PHMin_Set += 1;
          if (PHMin_Set > 100)
            PHMin_Set = 100;
        
          sprintf ((char*)SendBuf, "t13.txt=\"%.1f\"", PHMin_Set);    
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          PHMin_Set -= 1;
          if (PHMin_Set <= 0)
            PHMin_Set = 0;
          
          sprintf ((char*)SendBuf, "t13.txt=\"%.1f\"", PHMin_Set);    
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t13.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
  }
}

/*
 * 函数名：DisSensorLost
 * 描述  ：从机设备丢失时，在页面上显示设备丢失
 * 输入  ：无
 * 返回  ：无
 */
void DisSensorLost (void)
{
  uint8_t SendBuf[15];  
 
  Scr_SendCmd((uint8_t *)"t16.pco=RED");
  Scr_SendCmd((uint8_t *)"t16.txt=\"设备丢失\"");
  
  Scr_SendCmd((uint8_t *)"t17.pco=RED");
  Scr_SendCmd((uint8_t *)"t17.txt=\"设备丢失\"");
  
  Scr_SendCmd((uint8_t *)"t18.pco=RED");
  Scr_SendCmd((uint8_t *)"t18.txt=\"设备丢失\"");
  
  Scr_SendCmd((uint8_t *)"t19.pco=RED");
  Scr_SendCmd((uint8_t *)"t19.txt=\"设备丢失\"");
  
  Scr_SendCmd((uint8_t *)"t20.pco=RED");
  Scr_SendCmd((uint8_t *)"t20.txt=\"设备丢失\"");
  
  Scr_SendCmd((uint8_t *)"t21.pco=RED");
  Scr_SendCmd((uint8_t *)"t21.txt=\"设备丢失\"");
}


/*
 * 函数名：FlashDatPage
 * 描述  ：刷新数据显示页面
 * 输入  ：无
 * 返回  ：无
 */
void FlashDatPage (void)
{
  uint8_t SlaveSearchCount = 0;
  float PH, WaterTemp, EnvirTemp, WaterSpeed;         //采集到的数据
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;   //各参数上下限
  uint8_t WaterLevel, PowerDown;
  uint8_t WarmingTmp = 0;                             //存在报警标志位
  uint8_t SendBuf[30];                                //发送给屏幕的buf
  
  
  /* 先获取设置的各参数的上下限 */
  PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
  PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
  WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
  WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
  EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
  EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
  WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
  WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
  
//  WaterMaxTest = WaterTempMax;
//  WaterMinTest = WaterTempMin;
//  WaterTempTest = WaterTemp;
  
  /* 显示上方的时间 */
//  RTC_Get();
//  DisSensorVal(Time_UpDat, Color_Green, 0);       //后两个参数在这里无效，不使用，所以随便写两个
  
  sprintf ((char*)SendBuf, "t0.txt=\"%s\"", ChipID_String);    //显示设备ID
  Scr_SendCmd(SendBuf);
    
  /* 显示室温 */
  if (DatPage == DisMastDat)                      //如果显示的是主机数据
    EnvirTemp = ((Flash_FifoBuf[6] >> 18) & 0xff) + (((Flash_FifoBuf[6] >> 14) & 0xf) / 10.0);    //获取室温
  else
    EnvirTemp = ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 24) & 0xff) + (((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 20) & 0xf) / 10.0);
    
//  sprintf ((char*)SendBuf, "t16.txt=\"%.1f\"", EnvirTemp);        //将值送去屏幕打印
  if ((EnvirTemp > EnvirTempMax) || (EnvirTemp < EnvirTempMin))   //如果超限
  {
    DisSensorVal(SensorEnvirTemp, Color_Red, EnvirTemp);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorEnvirTemp, Color_Green, EnvirTemp);
  
  /* 显示水温 */
  if (DatPage == DisMastDat)
    WaterTemp = ((Flash_FifoBuf[5] & 0x3f) << 2) + ((Flash_FifoBuf[6] >> 30) & 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0);
  else
    WaterTemp = (((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
  if ((WaterTemp > WaterTempMax) || (WaterTemp < WaterTempMin))
  {
    DisSensorVal(SensorWaterTemp, Color_Red, WaterTemp);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorWaterTemp, Color_Green, WaterTemp);
  
  /* 获取泵状态 */
  if (DatPage == DisMastDat)
    WaterSpeed = ((Flash_FifoBuf[6] >> 6) & 0xff) + (((Flash_FifoBuf[6] >> 2) & 0xf) / 10.0);
  else
    WaterSpeed = ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 12) & 0xff) + (((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 8) & 0xf) / 10.0);
  if ((WaterSpeed > WaterSpeedMax) || (WaterSpeed < WaterSpeedMin))
  {
    DisSensorVal(SensorWaterSpeed, Color_Red, WaterSpeed);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorWaterSpeed, Color_Green, WaterSpeed);
  
  /* 显示水位 */
  if (DatPage == DisMastDat)
    WaterLevel = ((Flash_FifoBuf[5] >> 24) & 0x1);
  else
    WaterLevel = ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 7) & 0x01);

  if (WaterLevel == 1)
  {
    DisSensorVal(SensorWaterLevel, Color_Red, WaterLevel);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorWaterLevel, Color_Green, WaterLevel);
  
  /* 显示PH */
  if (DatPage == DisMastDat)
    PH = ((Flash_FifoBuf[5] >> 10) & 0xf) + (((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);
  else
    PH = (((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
  if ((PH > PHMax) || (PH < PHMin))                       //警告的字体颜色为红色
  {
    DisSensorVal(SensorPH, Color_Red, PH);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorPH, Color_Green, PH);
  
  /* 显示掉电情况 */
//  if (DatPage == DisMastDat)
    PowerDown = ((Flash_FifoBuf[5] >> 20) & 0x01);
  if (PowerDown == 1)
  {
    DisSensorVal(SensorPower, Color_Red, PowerDown);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorPower, Color_Green, PowerDown);
  
  /* 显示最下方的页面编号 */
  DisSensorVal(SerialSerial, Color_Green, DatPage);       //颜色在这里没有用到
    
  
  if (DatPage != DisMastDat)    //如果显示的是从机数据页面
  {
    SlaveSearchCount = ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 26) & 0xf);
    if  (SlaveSearchCount >= SlaveLostTime)  //如果设备已经丢失
    {
      DisSensorLost();
    }
  }
  

      
  /* 只要存在警报，报警 */
  if (WarmingTmp > 0)
  {
    //Beep_ON_Nop(700);
  }
}

/*
 * 函数名：GotoDisDatPage
 * 描述  ：将显示页面跳转到数据显示页面
 * 输入  ：无
 * 返回  ：无
 */
void GotoDisDatPage (void)
{  
  /* 切换页面 */
  Scr_SendCmd((uint8_t*)"page DisDat");  
  PageSerial = DisDatPage;
  UserDelay_KeyScan(500);
  
  FlashPage();               //刷新页面
}


void FlashLimPage (void)
{
  uint8_t SendBuf[50];
  int PosTable_x[] = {121, 227};
  int PosTable_y[] = {78, 110, 143, 174};
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
    
  PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
  PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
  WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
  WaterTempMin = (Flash_FifoBuf[1] & 0xf << 4) + ((Flash_FifoBuf[2] >> 28) & 0xf) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0);
  EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
  EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
  WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
  WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
  
  
  /* 清屏 */
  USART1_SendBuf("fill 90,75,200,125,36797");
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* PH最大值 */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[0], PosTable_y[0], PHMax);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* PH最小值*/
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[1], PosTable_y[0], PHMin);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* 水温最大值 */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[0], PosTable_y[1], WaterTempMax);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* 水温最小值 */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[1], PosTable_y[1], WaterTempMin);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* 板温最大值 */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[0], PosTable_y[2], EnvirTempMax);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* 板温最小值 */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[1], PosTable_y[2], EnvirTempMin);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* 流速最大值 */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[0], PosTable_y[3], WaterSpeedMax);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* 流速最小值 */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[1], PosTable_y[3], WaterSpeedMin);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
}

/*
 * 函数名：GoLimPage
 * 描述  ：服务器更新阈值后刷新页面显示三秒，之后回到显示页面
 * 输入  ：无
 * 返回  ：无
 */
void GoLimPage (void)
{
  /* 切换页面 */
  USART1_SendBuf("page LimUpDat");    //跳转到阈值显示页面
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  UserDelay_KeyScan(50);
  
  FlashLimPage();                     //显示新的阈值
  
  UserDelay_KeyScan(3000);
  
  switch(PageSerial)
  {
    case DisDatPage:
      Scr_SendCmd("page DisDat");  //返回原先的页面
     break;
    case LimSetPage:
      Scr_SendCmd("page LimSet");
      break;
    default:
      break;
  }
  //FlashPage();
  
}

/*
 * 函数名：ScrRest
 * 描述  ：屏幕复位
 * 输入  ：无
 * 返回  ：无
 */
void ScrRest (void)
{
  Delay_ms(500);
  USART1_SendBuf("rest");
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  Delay_ms(200);
}

/*
 * 函数名：ProBarGo
 * 描述  ：初始化界面进度条进度控制
 * 输入  ：无
 * 返回  ：无
 */
void ProBarGo (int val)
{
  static uint8_t i = 0;
  int k = 0;
  uint8_t SendBuf[10];
  
  if (val > i)
  {
    for (k= i; k < val; k++)
    {
      sprintf((char*)SendBuf, "j0.val=%d", k);
      USART1_SendBuf(SendBuf);
      USART1_SendByte(0xff);
      USART1_SendByte(0xff);
      USART1_SendByte(0xff);
      Delay_ms(50);
    }
    i = val;
  }
  if (val < i)
  {
    for (k= i; k > val; k--)
    {
      sprintf((char*)SendBuf, "j0.val=%d", k);
      USART1_SendBuf(SendBuf);
      USART1_SendByte(0xff);
      USART1_SendByte(0xff);
      USART1_SendByte(0xff);
      Delay_ms(10);
    }
    i = val;
  }
}


