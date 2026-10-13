/**
 * usart.h — 串口驱动（USART1，板载 CH340G）
 */
#ifndef __USART_H
#define __USART_H

#include "main.h"
#include <stdint.h>

#define USART1_BAUDRATE  115200U

extern UART_HandleTypeDef huart1;

void usart1_init(uint32_t baudrate);
HAL_StatusTypeDef usart1_send(const uint8_t *data, uint16_t len);
int  fputc_target_usart1(const char *buf, int len);   /* 供 syscalls._write 调用 */

#endif /* __USART_H */
