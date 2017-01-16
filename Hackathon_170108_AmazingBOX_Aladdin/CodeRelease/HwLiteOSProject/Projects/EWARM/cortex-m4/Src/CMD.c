#include <stdint.h>
#include <string.h>
#include "Uart.h"
#include "CMD.h"
#include "LightCtrl.h"

//------------------------------------------------------------------------------
// Process incoming uart command
void CmdProcess(uint8_t CMD, uint8_t* Data)
{
  switch (CMD)
  {
  case CMD_CTRL_LIGHT_OUT:
    // Release auto control first
    if ((*(uint16_t*)Data) < CMD_PAYLOAD_LIGHT_OUT_MAX)
    {
      LightCtrlSetSrc(LIGHTCTRL_SRC_MANUAL);
      LightCtrlSet(*((uint16_t*)Data));
    }
    break;
  default:
    break;
  }
}

//------------------------------------------------------------------------------
// Send command via uart
void CmdSend(uint8_t CMD, int Len, uint8_t* Data)
{
  UartDataFrame_t UartDataFrame;
  UartDataFrame.CMD = CMD;
  UartDataFrame.Len = Len;
  memcpy(UartDataFrame.Payload, Data, Len);
  UartDataSend(&UartDataFrame);
}