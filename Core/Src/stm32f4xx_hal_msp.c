/**
 * stm32f4xx_hal_msp.c — HAL MSP（MCU Support Package）
 * 外设底层初始化：时钟使能、引脚复用、DMA/NVIC 配置都集中在这。
 */
#include "main.h"

/**
 * HAL_DeInit 时反向清理（当前用不到，留空）
 */
void HAL_MspInit(void) { }
void HAL_MspDeInit(void) { }

/**
 * 全局 MSP：HAL_Init() 回调（SysTick 时基等在 HAL_Init 内部已处理，这里留空备用）
 */

/**
 * USART1 底层初始化（HAL_UART_Init 回调）
 * PA9  -> USART1_TX (AF7)
 * PA10 -> USART1_RX (AF7)
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (huart->Instance == USART1)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

/**
 * USART1 底层反初始化
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    __HAL_RCC_USART1_FORCE_RESET();
    __HAL_RCC_USART1_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

    __HAL_RCC_USART1_CLK_DISABLE();
  }
}
