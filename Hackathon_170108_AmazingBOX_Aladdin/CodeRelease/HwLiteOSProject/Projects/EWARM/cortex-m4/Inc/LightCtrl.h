#pragma once

enum LIGHTCTRL_SRC
{
  LIGHTCTRL_SRC_AUTO = 0,
  LIGHTCTRL_SRC_MANUAL
};

void LightCtrlSet(uint16_t Value);
uint16_t LightCtrlGet(void);
int LightCtrlGetSrc(void);
void LightCtrlSetSrc(int CtrlSrc);