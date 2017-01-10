#include <stdint.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "los_hwi.h"
#include "los_task.h"
#include "los_sem.h"
#include "HumanDetector.h"
#include "LightCtrl.h"

//------------------------------------------------------------------------------
#pragma message("Altered for demo")
#define RESTORE_VALUE_S                                                       15

//------------------------------------------------------------------------------
static int l_Status = HD_STATUS_INACTIVE;
static LITE_OS_SEC_BSS UINT32 l_HumanDetectTaskID;
static UINT32 l_DetectTaskMutex = NULL;
static int l_DetectDelay_s = RESTORE_VALUE_S;
static void HumanDetectTask(void);

#ifdef __cplusplus
extern "C" {
#endif
static void HwIsrHd(void);
#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
void HumanDetectorInit(void)
{
  TSK_INIT_PARAM_S HumanDetectTaskParam;
  
  // Register hardware interrupt
  LOS_HwiCreate(EXTI15_10_IRQn, 0, 0, HwIsrHd, 0);
  
  // Create detect task mutex
  LOS_SemCreate(0, &l_DetectTaskMutex);
  
  // Create & start detect task
  (VOID)memset(&HumanDetectTaskParam, 0, sizeof(HumanDetectTaskParam));
  HumanDetectTaskParam.pfnTaskEntry    = (TSK_ENTRY_FUNC)HumanDetectTask;
  HumanDetectTaskParam.uwStackSize     = 512;
  HumanDetectTaskParam.pcName          = "DataUpdateTask";
  HumanDetectTaskParam.usTaskPrio      = 5;
  HumanDetectTaskParam.uwResved        = 0; // This task should not be deleted
  
  if (LOS_OK != LOS_TaskCreate(&l_HumanDetectTaskID, &HumanDetectTaskParam))
  {
    // Debug print
    printf("\r\n[DEBUG]Start HD task failed!!");
  }
}

//------------------------------------------------------------------------------
int HumanDetectGetStatus(void)
{
  return l_Status;
}

//------------------------------------------------------------------------------
static void HumanDetectTask(void)
{
  for (;;)
  {
    // Block task when no human around sensor
    LOS_SemPend(l_DetectTaskMutex, LOS_WAIT_FOREVER);
    
    // Reload counter when someone trigger the sensor
    while (--l_DetectDelay_s != 0)
    {
      LOS_TaskDelay(1000);
    }
    
    // Release manual control
    LightCtrlSetSrc(LIGHTCTRL_SRC_AUTO);
    
    l_Status = HD_STATUS_INACTIVE;
  }
}

#ifdef __cplusplus
extern "C" {
#endif

static void HwIsrHd(void)
{
  if (RESET != __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10))
  {
    // Reload counter when someone trigger the sensor
    l_DetectDelay_s = RESTORE_VALUE_S;
    
    if (HD_STATUS_INACTIVE == l_Status)
    {
      l_Status = HD_STATUS_ACTIVE;
      LOS_SemPost(l_DetectTaskMutex);
    }
    
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
  }
}

#ifdef __cplusplus
}
#endif