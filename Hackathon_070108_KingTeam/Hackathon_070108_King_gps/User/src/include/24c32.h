#ifndef __24c32_H
#define __24c32_H

#include "stm32f10x.h"
#include "types.h"

#define I2C_SCK	        GPIO_Pin_6  //out
#define I2C_SDA	        GPIO_Pin_7
#define EEPROM_PORT     GPIOB
/* 初始化 */
void at24c32_init(void);
/* 写入一个字节内容 */
void at24c32_write(uint16 addr, unsigned char val);
/* 读出一个字节内容 */
unsigned char at24c32_read(uint16 addr);
/* 写入一块内容 */
void eeprom_block_write(uint16_t addr, uint16_t len, const uint8 *buf);
/* 读出一块内容 */
void eeprom_block_read(uint16_t addr, uint16_t len, uint8_t *buf);
/* 擦出内容 */
void erase_eeprom(uint16 startAddr, uint16 len);

#endif /* __MAIN_H */




