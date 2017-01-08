#include <stdint.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "uart.h"
#include "los_task.h"
#include "los_sem.h"
#include "CMD.h"

//------------------------------------------------------------------------------
// Receive states
enum
{
  RX_STATE_SOF,
  RX_STATE_LEN,
  RX_STATE_CMD,
  RX_STATE_DATA,
  RX_STATE_FCS
};

// Receive parameters
#define UART_DATA_SOF                                                       0xFE
#define HEAD_LEN                                                               4
#define UART_RX_DATA_BUF_SIZE                       (HEAD_LEN + MAX_PAYLOAD_LEN)
#define UART_TX_DATA_BUF_SIZE                              UART_RX_DATA_BUF_SIZE

//------------------------------------------------------------------------------
static UINT32 l_UartRxTaskMutex = NULL;
static LITE_OS_SEC_BSS UINT32 l_UartRxTaskID;
static bool f_RxBusy = FALSE;
static UartDataFrame_t l_RxData;

//------------------------------------------------------------------------------
static void UartRxTask(void);
static uint8_t FcsCaculate(UartDataFrame_t* Data);
#ifdef __cplusplus
extern "C" {
#endif
static void HwIsrUsart(void);
#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
extern UART_HandleTypeDef huart1;

//------------------------------------------------------------------------------
void UartInit(void)
{
  TSK_INIT_PARAM_S UartRxTaskParam;
  
  // Create detect task mutex
  LOS_SemCreate(0, &l_UartRxTaskMutex);
  
  // Create & start detect task
  (VOID)memset(&UartRxTaskParam, 0, sizeof(UartRxTaskParam));
  UartRxTaskParam.pfnTaskEntry    = (TSK_ENTRY_FUNC)UartRxTask;
  UartRxTaskParam.uwStackSize     = 1024;
  UartRxTaskParam.pcName          = "UartRxTask";
  UartRxTaskParam.usTaskPrio      = 5;
  UartRxTaskParam.uwResved        = 0; // This task should not be deleted
  
  if (LOS_OK != LOS_TaskCreate(&l_UartRxTaskID, &UartRxTaskParam))
  {
    // Debug print
    printf("\r\n[DEBUG]Start HD task failed!!");
  }
  
  // Register hardware interrupt
  LOS_HwiCreate(USART1_IRQn, 0, 0, HwIsrUsart, 0);
}

//------------------------------------------------------------------------------
static void UartRxTask(void)
{
  for (;;)
  {
    // Waiting valid data
    LOS_SemPend(l_UartRxTaskMutex, LOS_WAIT_FOREVER);
    
    // Handle
    CmdProcess(l_RxData.CMD, l_RxData.Payload);
    
    // Clear rx busy flag
    f_RxBusy = FALSE;
  }
}

//------------------------------------------------------------------------------
int UartDataParse(uint8_t ch, UartDataFrame_t* RxData)
{
  int Ret = RX_STATE_PARSING;
  static int RxState = RX_STATE_SOF;
  static int PayloadIndex = 0;
  switch (RxState)
  {
  case RX_STATE_SOF:
    Ret = RX_STATE_PARSING;
    if (ch == UART_DATA_SOF)
    {
      RxState = RX_STATE_LEN;
    }
    break;
  case RX_STATE_LEN:
    if (ch <= MAX_PAYLOAD_LEN)
    {
      RxData->Len = ch;
      PayloadIndex = 0;
      RxState = RX_STATE_CMD;
    }
    else
    {
      RxState = RX_STATE_SOF;
      Ret = RX_STATE_FAIL;
    }
    break;
  case RX_STATE_CMD:
    RxData->CMD = ch;
    if (0 == RxData->Len)
    {
      RxState = RX_STATE_FCS;
    }
    else
    {
      RxState = RX_STATE_DATA;
    }
    break;
  case RX_STATE_DATA:
    {
      RxData->Payload[PayloadIndex++] = ch;
      if (PayloadIndex >= RxData->Len)
      {
        RxState = RX_STATE_FCS;
      }
    }
    break;
  case RX_STATE_FCS:
    if (ch == FcsCaculate(RxData))
    {
      Ret = RX_STATE_SUCCESS;
    }
    else
    {
      Ret = RX_STATE_FAIL;
    }
    RxState = RX_STATE_SOF;
    break;
  }
  return Ret;
}

//------------------------------------------------------------------------------
void UartDataSend(UartDataFrame_t* TxData)
{
  uint8_t TmpTxBuf[UART_TX_DATA_BUF_SIZE];
  int Index = 0;
  
  // SOF
  TmpTxBuf[Index++] = UART_DATA_SOF;
  
  // LEN
  TmpTxBuf[Index++] = TxData->Len;
  
  // CMD
  TmpTxBuf[Index++] = TxData->CMD;
  
  // PAYLOAD
  for (int i = 0; i < TxData->Len; i++)
    TmpTxBuf[Index++] = TxData->Payload[i];
  
  // FCS
  TmpTxBuf[Index++] = FcsCaculate(TxData);
  
  // Send
  HAL_UART_Transmit(&huart1, TmpTxBuf, Index, 0xFFFF);
}

//------------------------------------------------------------------------------
static uint8_t FcsCaculate(UartDataFrame_t* Data)
{
  uint8_t Ret = 0;
  
  Ret ^= Data->CMD;
  Ret ^= Data->Len;
  for (int i = 0; i < Data->Len; i++)
  {
    Ret ^= Data->Payload[i];
  }
  return Ret;
}

#ifdef __cplusplus
extern "C" {
#endif
//------------------------------------------------------------------------------
static void HwIsrUsart(void)
{
  if (USART1->SR & UART_FLAG_RXNE)
  {
    // Read first
    uint8_t ch = (uint8_t)USART1->DR;
    
    // Temporary use flag, replace with dymatic memory later
    if (FALSE == f_RxBusy)
    {
      if (RX_STATE_SUCCESS == UartDataParse(ch, &l_RxData))
      {
        f_RxBusy = TRUE;
        LOS_SemPost(l_UartRxTaskMutex);
      }
    }
  }
}
#ifdef __cplusplus
}
#endif