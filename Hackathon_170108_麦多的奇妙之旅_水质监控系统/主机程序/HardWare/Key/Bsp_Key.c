#include "common.h"
#include "Bsp_Key.h"

uint8_t KeyStatue = KeyDefault;

/*
 * 函数名：Key_Init
 * 描述  ：键盘按键初始化
 * 输入  ：无
 * 返回  ：无
 */
void Key_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_KeyUp | RCC_APB2Periph_KeyDn | RCC_APB2Periph_KeyOK |
                         RCC_APB2Periph_KeyLF | RCC_APB2Periph_KeyRG ,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyUp; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOx_KeyUp, &GPIO_InitStructure);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyDn; 
  GPIO_Init(GPIOx_KeyDn, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyOK; 
  GPIO_Init(GPIOx_KeyOK, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyLF; 
  GPIO_Init(GPIOx_KeyLF, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyRG; 
  GPIO_Init(GPIOx_KeyRG, &GPIO_InitStructure); 
}

/*
 * 函数名：ReadTouchWay
 * 描述  ：确定通信方式
 * 输入  ：无
 * 返回  ：无
 */
void ReadTouchWay (void)    
{
  uint8_t statue = 0;
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
  statue = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
  if (statue)         
    Connect_Way = USE_ZGB;      //默认是ZGB
  else
    Connect_Way = USE_CAN;      //按下是CAN
}



/*
 * 函数名：Key_Scan
 * 描述  ：按键扫描
 * 输入  ：无
 * 返回  ：无
 */
void Key_Scan (void)
{
  static uint8_t statue = 0;
  
  int PressCount = 0;
  if (statue == 0)
  {
    statue = 1;
    
    if (KeyStatue == KeyDefault)
    {
      /* 左 */
      if (KEY_LF == 0)
      {
        Delay_ms(10);
        if (KEY_LF == 0)
          KeyStatue = KeyLF;
        while(KEY_LF == 0);
        Delay_ms(10);
      }
      
      /* 上 */
      if (KEY_UP == 0)
      {
        Delay_ms(10);
        if (KEY_UP == 0)
          KeyStatue = KeyUp;
        while(KEY_UP == 0);
        Delay_ms(10);
      }
      
      /* 右 */
      if (KEY_RG == 0)
      {
        Delay_ms(10);
        if (KEY_RG == 0)
          KeyStatue = KeyRG;
        while(KEY_RG == 0);
        Delay_ms(10);
      }
      
      /* 功能键 */
      if (KEY_OK == 0)
      {
        Delay_ms(8);
        if (KEY_OK == 0)
          KeyStatue = KeyOK;
        while(KEY_OK == 0)
        {
          Delay_ms(10);
          PressCount++;
          if (PressCount >= 500)    //长按以后发送设备绑定信息
          {
            if (TCPIsUsed == 1)
            {
              Beep_ON_Nop(100);
              Beep_ON_Nop(100);
            }
            else
            {
              DevBound();
            }
          }
        }
        Delay_ms(10);
      }
      
      /* 下 */
      if (KEY_DN == 0)
      {
        Delay_ms(10);
        if (KEY_DN == 0)
          KeyStatue = KeyDn;
        while(KEY_DN == 0);
        Delay_ms(10);
      }
      
      if (KeyStatue != KeyDefault)
      {
        LED_Green(ON);
        FlashPage();
      }
    }
    statue = 0;
  }
}





void Key_Scan1 (void)
{
  int PressCount = 0;
  
  if (KeyStatue == KeyDefault)
  {
    /* 左 */
    if (KEY_LF == 0)
    {
      Delay_ms(10);
      if (KEY_LF == 0)
        KeyStatue = KeyLF;
      while(KEY_LF == 0);
      Delay_ms(10);
    }
    
    /* 上 */
    if (KEY_UP == 0)
    {
      Delay_ms(10);
      if (KEY_UP == 0)
        KeyStatue = KeyUp;
      while(KEY_UP == 0);
      Delay_ms(10);
    }
    
    /* 右 */
    if (KEY_RG == 0)
    {
      Delay_ms(10);
      if (KEY_RG == 0)
        KeyStatue = KeyRG;
      while(KEY_RG == 0);
      Delay_ms(10);
    }
    
    /* 功能键 */
    if (KEY_OK == 0)
    {
      Delay_ms(8);
      if (KEY_OK == 0)
        KeyStatue = KeyOK;
      while(KEY_OK == 0)
      {
        Delay_ms(10);
        PressCount++;
        if (PressCount >= 500)    //长按以后发送设备绑定信息
        {
          DevBound();
        }
      }
      Delay_ms(10);
    }
    
    /* 下 */
    if (KEY_DN == 0)
    {
      Delay_ms(10);
      if (KEY_DN == 0)
        KeyStatue = KeyDn;
      while(KEY_DN == 0);
      Delay_ms(10);
    }
    
    if (KeyStatue != KeyDefault)
    {
      FlashPage();
    }
    
  }
  
}


VOID task_KeyScan (void)
{
  UINT32 uwRet = LOS_OK;
  
  LED_Green(ON);
  
  while(1)
  {
    Key_Scan();
    
    uwRet = LOS_TaskDelay(100);//系统延时1S
		if(uwRet !=LOS_OK)
      return;
  }
}

UINT32 KeyScanTskHandle;

UINT32 creat_KeyScanTask (void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;                   //任务优先级，0最高
	task_init_param.pcName = "KeyScan";                //???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_KeyScan;//任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;//任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&KeyScanTskHandle,&task_init_param);//创建任务
  
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}





