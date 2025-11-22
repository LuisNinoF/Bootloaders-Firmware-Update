#ifndef __BSP_H__
#define __BSP_H__

#include <stdint.h>
#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);
void button_init(void);
bool get_btn_state(void);

#define PIN5		(1U<<5)
#define LED_PIN		PIN5
#define PIN13		(1U<<13)
#define BTN_PIN		PIN13

#endif
