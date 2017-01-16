#include "device.h"

void device_init(void){
    PM25_init();
    relay_init();
    USART_Config();
    DHT11_Init();
    LED_init();
    Key_init();
}


