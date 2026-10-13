/**
 * key.c — 按键驱动
 * KEY0=PE4 / KEY1=PE3 / KEY2=PE2：低电平按下（内部上拉）
 * WK_UP=PA0：高电平按下（内部下拉）
 */
#include "key.h"

void key_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* KEY0/1/2：输入上拉 */
  GPIO_InitStruct.Pin  = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* WK_UP：输入下拉 */
  GPIO_InitStruct.Pin  = GPIO_PIN_0;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * 按键扫描（需周期性调用，如 10ms 一次）
 * mode: 0=不支持连按（按住只报一次） 1=支持连按
 * 返回：0=无按键，否则 KEY0_PRES..WKUP_PRES
 */
uint8_t key_scan(uint8_t mode)
{
  static uint8_t key_up = 1;   /* 1=松开状态，0=按下状态 */

  if (mode) key_up = 1;        /* 连按模式：每次都允许重新检测 */

  if (key_up && (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET ||
                 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET ||
                 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_RESET ||
                 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET))
  {
    HAL_Delay(10);             /* 消抖 */
    key_up = 0;
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) return KEY0_PRES;
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET) return KEY1_PRES;
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_RESET) return KEY2_PRES;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)   return WKUP_PRES;
  }
  else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_SET &&
           HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_SET &&
           HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_SET &&
           HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
  {
    key_up = 1;                /* 全部松开 */
  }
  return 0;
}
