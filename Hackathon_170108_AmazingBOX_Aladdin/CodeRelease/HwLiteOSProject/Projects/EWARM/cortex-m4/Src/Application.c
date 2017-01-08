#include <stdint.h>
#include <string.h>
#include "los_sys.h"
#include "los_tick.h"
#include "los_task.h"
#include "Data.h"
#include "LightCtrl.h"
#include "HumanDetector.h"
#include "CMD.h"

//------------------------------------------------------------------------------
// Report parameters
#define APPLICATION_MAIN_LOOP_INTERVAL_MS                                    100
#define APPLICATION_REPORT_INTERVAL_100MS                                      5

// Ajust parameters
#define TARGET_LIGHT                                                          40
#define TARGET_LIGHT_OFFSET                                                   10
#define LIGHT_OUT_STEP_VALUE                                                  50

//------------------------------------------------------------------------------
static LITE_OS_SEC_BSS UINT32 l_ApplicationTaskID;

//------------------------------------------------------------------------------
static void ApplicationTask(void);

//------------------------------------------------------------------------------
void AppInit(void)
{
  // Start data engine
  DataEngineInit();
  
  // Set up application
  TSK_INIT_PARAM_S ApplicationTaskParam;
  
  // Create & start data engine task
  (VOID)memset(&ApplicationTaskParam, 0, sizeof(ApplicationTaskParam));
  ApplicationTaskParam.pfnTaskEntry    = (TSK_ENTRY_FUNC)ApplicationTask;
  ApplicationTaskParam.uwStackSize     = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
  ApplicationTaskParam.pcName          = "ApplicationTask";
  ApplicationTaskParam.usTaskPrio      = 5;
  ApplicationTaskParam.uwResved        = 0; // This task should not be deleted
  
  if (LOS_OK != LOS_TaskCreate(&l_ApplicationTaskID, &ApplicationTaskParam))
  {
    // Debug print
    printf("\r\n[DEBUG]Start application task failed!!");
  }
}

//------------------------------------------------------------------------------
static void ApplicationTask(void)
{
  // Report counter
  int ReportCounter = 0;
  
  // Infinite loop
  for (;;)
  {
    // Ajust every 100ms
    LOS_TaskDelay(100);
    
    // Protect data
    LOS_TaskLock();
    
    // Ajust light
    // Get HD control source first
    if (LIGHTCTRL_SRC_AUTO == LightCtrlGetSrc())
    {
      // Get HD status
      if (HD_STATUS_ACTIVE == pSensorData->HDStatus)
      {
        if (pSensorData->LightIn > (TARGET_LIGHT + TARGET_LIGHT_OFFSET))
        {
          if (pSensorData->LightOut <= (CMD_PAYLOAD_LIGHT_OUT_MAX - LIGHT_OUT_STEP_VALUE))
          {
            LightCtrlSet(pSensorData->LightOut + LIGHT_OUT_STEP_VALUE);
          }
          else
          {
            LightCtrlSet(CMD_PAYLOAD_LIGHT_OUT_MAX);
          }
        }
        else if (pSensorData->LightIn < (TARGET_LIGHT - TARGET_LIGHT_OFFSET))
        {
          if (pSensorData->LightOut >= LIGHT_OUT_STEP_VALUE)
          {
            LightCtrlSet(pSensorData->LightOut - LIGHT_OUT_STEP_VALUE);
          }
          else
          {
            LightCtrlSet(0);
          }
        }
      }
      else
      {
        // Turn off when nobody around HD
        LightCtrlSet(0);
      }
    }
    
    // Report data
    if (++ReportCounter >= APPLICATION_REPORT_INTERVAL_100MS)
    {
      // Clear counter first
      ReportCounter = 0;
      
      // Package and report sensor data
      static uint8_t ReportPayload[CMD_PAYLOAD_REPORT_LEN];
      
      *(uint16_t*)(ReportPayload + CMD_PAYLOAD_REPORT_INDEX_LIGHT_OUT) = pSensorData->LightOut;
      *(uint8_t*)(ReportPayload + CMD_PAYLOAD_REPORT_INDEX_LIGHT_IN)   = pSensorData->LightIn;
      *(uint8_t*)(ReportPayload + CMD_PAYLOAD_REPORT_INDEX_HD)         = pSensorData->HDStatus;
      
      CmdSend(CMD_REPORT_ALL, CMD_PAYLOAD_REPORT_LEN, ReportPayload);
    }
    
    LOS_TaskUnlock();
  }
}