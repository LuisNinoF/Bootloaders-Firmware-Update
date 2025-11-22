#ifndef __ESP82XX_H__
#define __ESP82XX_H__

#include "stm32f4xx.h"

void debug_uart_init(void);
void esp_uart_init(void);
void esp_rs_pin_init(void);
void esp_rs_pin_enable(void);

#endif
