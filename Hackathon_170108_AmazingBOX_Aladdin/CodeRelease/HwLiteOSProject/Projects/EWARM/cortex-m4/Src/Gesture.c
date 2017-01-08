#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "los_hwi.h"

#ifdef __cplusplus
extern "C" {
#endif
static void HwIsrGesture(void);
#ifdef __cplusplus
}
#endif

void GestureInit(void)
{
  LOS_HwiCreate(EXTI3_IRQn, 0, 0, HwIsrGesture, 0);
}


#ifdef __cplusplus
extern "C" {
#endif
static void HwIsrGesture(void)
{
  if (RESET != __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3))
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
  }
}
#ifdef __cplusplus
}
#endif