/**
 * led.c — LED 驱动
 * 探索者板：LED0=PF9，LED1=PF10，均为低电平点亮
 */
#include "led.h"

/* 每个 LED 的 端口/引脚 表 */
static GPIO_TypeDef *led_port[2] = {LED0_GPIO_Port, LED1_GPIO_Port};
static const uint16_t led_pin[2] = {LED0_Pin, LED1_Pin};

void led_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOF_CLK_ENABLE();

  GPIO_InitStruct.Pin   = LED0_Pin | LED1_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;    /* 推挽输出 */
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* 默认熄灭（输出高） */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}

void led_on(uint8_t led)
{
  if (led == 0xFFU) {
    for (uint8_t i = 0; i < 2; i++) HAL_GPIO_WritePin(led_port[i], led_pin[i], GPIO_PIN_RESET);
  } else if (led < 2U) {
    HAL_GPIO_WritePin(led_port[led], led_pin[led], GPIO_PIN_RESET);
  }
}

void led_off(uint8_t led)
{
  if (led == 0xFFU) {
    for (uint8_t i = 0; i < 2; i++) HAL_GPIO_WritePin(led_port[i], led_pin[i], GPIO_PIN_SET);
  } else if (led < 2U) {
    HAL_GPIO_WritePin(led_port[led], led_pin[led], GPIO_PIN_SET);
  }
}

void led_toggle(uint8_t led)
{
  if (led < 2U)
    HAL_GPIO_TogglePin(led_port[led], led_pin[led]);
}
