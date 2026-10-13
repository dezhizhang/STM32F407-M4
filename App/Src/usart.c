/**
 * usart.c — 串口驱动
 * USART1: PA9-TX / PA10-RX，115200-8-N-1（板载 CH340G 转 USB）
 * printf 经 syscalls.c 的 _write 重定向到这里
 */
#include "usart.h"

UART_HandleTypeDef huart1;

void usart1_init(uint32_t baudrate)
{
  huart1.Instance          = USART1;
  huart1.Init.BaudRate     = baudrate;
  huart1.Init.WordLength   = UART_WORDLENGTH_8B;
  huart1.Init.StopBits     = UART_STOPBITS_1;
  huart1.Init.Parity       = UART_PARITY_NONE;
  huart1.Init.Mode         = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);     /* 引脚/时钟在 stm32f4xx_hal_msp.c 中配置 */
}

/**
 * 阻塞发送（带超时），用于一般数据发送
 */
HAL_StatusTypeDef usart1_send(const uint8_t *data, uint16_t len)
{
  return HAL_UART_Transmit(&huart1, (uint8_t *)data, len, 100);
}

/**
 * printf 出口：syscalls.c 的 _write 调到这里
 */
int fputc_target_usart1(const char *buf, int len)
{
  if (huart1.Instance == NULL)
    return -1;    /* 串口未初始化 */
  HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, 100);
  return len;
}
