/**
 * led.h — LED 驱动（正点原子探索者）
 */
#ifndef __LED_H
#define __LED_H

#include "main.h"

void led_init(void);
void led_on(uint8_t led);     /* 0=LED0, 1=LED1, 0xFF=全部 */
void led_off(uint8_t led);
void led_toggle(uint8_t led);

#endif /* __LED_H */
