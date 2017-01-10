#include "stm32f4xx_hal.h"
#include "BspAPDS9960.h"

#define APDS9960_DEVICE_ADDRESS 0x39

#define APDS9960_REG_ADDR_ENABLE 0x80

extern I2C_HandleTypeDef hi2c1;

static uint8_t TxBuf[16];

void BspAPDS9960Init(void)
{
//  if (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c1, APDS9960_DEVICE_ADDRESS, 1, 0xFFFF))
//  {
//    printf("\r\nAPDS9960 initialize OK!");
//  }
  uint8_t aTxBuffer[] = "12345";
  uint8_t aRxBuffer[128];
  
//  hi2c1.Instance->CR1 |= I2C_CR1_START;
//  hi2c1.Instance->DR = 0x31;
//  hi2c1.Instance->DR = 0x32;
//  hi2c1.Instance->DR = 0x33;
//  hi2c1.Instance->DR = 0x34;
//  hi2c1.Instance->DR = 0x35;
  
  HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)APDS9960_DEVICE_ADDRESS, (uint8_t*)aTxBuffer, sizeof(aTxBuffer), 10000);
//  
//  HAL_I2C_Master_Receive(&hi2c1, (uint16_t)APDS9960_DEVICE_ADDRESS, (uint8_t*)aRxBuffer, sizeof(aRxBuffer), 10000);
  
//  while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)APDS9960_DEVICE_ADDRESS, (uint8_t*)aTxBuffer, sizeof(aTxBuffer), 10000)!= HAL_OK)
//  {
////    /* Error_Handler() function is called when Timeout error occurs.
////       When Acknowledge failure occurs (Slave don't acknowledge it's address)
////       Master restarts communication */
////    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
////    {
////      Error_Handler();
////    }
//  }
}

void BspAPDS9960SendTest(void)
{
//  TxBuf[0] = APDS9960_DEVICE_ADDRESS; // Write
//  TxBuf[1] = 0; // Write
//  TxBuf[2] = 0; // Acknowlege
//  TxBuf[3] = APDS9960_REG_ADDR_ENABLE;
//  TxBuf[4] = 0;
//  TxBuf[5] = 0xFF;
//  
//  int Ret = HAL_I2C_Master_Transmit(&hi2c1, APDS9960_DEVICE_ADDRESS, TxBuf, 6, 0xFFFF);
//  
  int Ret = HAL_I2C_IsDeviceReady(&hi2c1, APDS9960_DEVICE_ADDRESS, 10, 0xFFFF);
  printf("\r\nRet = %d", Ret);
}