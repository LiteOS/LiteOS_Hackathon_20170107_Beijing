#include <stdint.h>
#include <string.h>
#include "los_task.h"
#include "stm32f4xx_hal.h"
#include "data.h"
#include "LightCtrl.h"
#include "HumanDetector.h"
#include "Uart.h"
#include "Gesture.h"

//------------------------------------------------------------------------------
#define UPDATE_INTERVAL_MS                                                    50

//------------------------------------------------------------------------------
static SensorData_t l_SensorData;
static LITE_OS_SEC_BSS UINT32 l_DataEngineTaskID;
SensorData_t* pSensorData = &l_SensorData;

//------------------------------------------------------------------------------
extern ADC_HandleTypeDef hadc1;

//------------------------------------------------------------------------------
static void DataUpdateTask(void);

//------------------------------------------------------------------------------
void DataEngineInit(void)
{
  TSK_INIT_PARAM_S DataEngineTaskParam;
  
  // Turn on human detector
  HumanDetectorInit();
  
  // Create & start data engine task
  (VOID)memset(&DataEngineTaskParam, 0, sizeof(DataEngineTaskParam));
  DataEngineTaskParam.pfnTaskEntry    = (TSK_ENTRY_FUNC)DataUpdateTask;
  DataEngineTaskParam.uwStackSize     = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
  DataEngineTaskParam.pcName          = "DataUpdateTask";
  DataEngineTaskParam.usTaskPrio      = 5;
  DataEngineTaskParam.uwResved        = 0; // This task should not be deleted
  
  if (LOS_OK != LOS_TaskCreate(&l_DataEngineTaskID, &DataEngineTaskParam))
  {
    // Debug print
    printf("\r\n[DEBUG]Start data engine failed!!");
  }
  
  // Turn on uart interrupt
  UartInit();
  
  // Turn on gesture interrupt
  GestureInit();
}

//------------------------------------------------------------------------------
static void DataUpdateTask(void)
{
  // Inifinit loop
  for (;;)
  {
    // Update sensor values every UPDATE_INTERVAL_MS ms
    LOS_TaskDelay(UPDATE_INTERVAL_MS);
    
    // Update Human Detector status
    l_SensorData.HDStatus = HumanDetectGetStatus();
    
    // Update LightIn value
    HAL_ADC_Start(&hadc1);
    l_SensorData.LightIn = (uint8_t)ADC1->DR;
    
    // Update LightOut value
    l_SensorData.LightOut = LightCtrlGet();
  }
}