#pragma once

enum UartDataParseState
{
  RX_STATE_PARSING = 0,
  RX_STATE_SUCCESS,
  RX_STATE_FAIL
};
    
#define MAX_PAYLOAD_LEN                                                        2

// Exported type
typedef struct
{
  int Len;
  uint8_t CMD;
  uint8_t Payload[MAX_PAYLOAD_LEN];
} UartDataFrame_t;

void UartInit(void);
int UartDataParse(uint8_t ch, UartDataFrame_t* RxData);
void UartDataSend(UartDataFrame_t* TxData);