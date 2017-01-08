#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "LightCtrl.h"

//------------------------------------------------------------------------------
static int l_CtrlSrc = LIGHTCTRL_SRC_AUTO;

//------------------------------------------------------------------------------
void LightCtrlSet(uint16_t Value)
{
  if (Value < 1000)
  {
    TIM3->CCR1 = Value;
  }
}

//------------------------------------------------------------------------------
uint16_t LightCtrlGet(void)
{
  return (uint16_t)TIM3->CCR1;
}

//------------------------------------------------------------------------------
void LightCtrlSetSrc(int CtrlSrc)
{
  l_CtrlSrc = CtrlSrc;
}

//------------------------------------------------------------------------------
int LightCtrlGetSrc(void)
{
  return l_CtrlSrc;
}