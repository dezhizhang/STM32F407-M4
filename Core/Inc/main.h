/**
 * main.h — 全局头文件
 */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* ---------------- 板级引脚定义（正点原子探索者 STM32F407ZGT6） ----------------
 * LED0  -> PF9   （低电平点亮）
 * LED1  -> PF10  （低电平点亮）
 * KEY0  -> PE4   （低电平按下，需上拉）
 * KEY1  -> PE3   （低电平按下，需上拉）
 * KEY2  -> PE2   （低电平按下，需上拉）
 * WK_UP -> PA0   （高电平按下，需下拉）
 * USART1_TX -> PA9（板载 CH340G，USB_TTL 接 P6）
 * USART1_RX -> PA10
 */
#define LED0_Pin       GPIO_PIN_9
#define LED0_GPIO_Port GPIOF
#define LED1_Pin       GPIO_PIN_10
#define LED1_GPIO_Port GPIOF

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
