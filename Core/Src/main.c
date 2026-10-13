/**
 * main.c — 正点原子探索者 STM32F407ZGT6 工程入口
 *
 * 时钟树：HSE 8MHz --PLL(M=8,N=336,P=2)--> SYSCLK 168MHz
 *         AHB 168MHz | APB1 42MHz | APB2 84MHz
 *
 * 演示：LED0 500ms 闪烁；按键控制 LED1；printf 输出到 USART1(115200)
 */
#include "main.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include <stdio.h>

static void SystemClock_Config(void);
static void Error_Handler(void);

int main(void)
{
  uint32_t last_blink = 0;
  uint8_t  led0_state = 0;

  /* HAL 库初始化（SysTick 1ms 时基 + NVIC 分组） */
  HAL_Init();

  /* 168MHz 系统时钟 */
  SystemClock_Config();

  /* 板级外设 */
  led_init();
  key_init();
  usart1_init(USART1_BAUDRATE);

  printf("\r\n=== ALIENTEK Explorer STM32F407 ===\r\n");
  printf("SYSCLK = %lu Hz\r\n", HAL_RCC_GetSysClockFreq());
  printf("HCLK   = %lu Hz, APB1 = %lu Hz, APB2 = %lu Hz\r\n",
         HAL_RCC_GetHCLKFreq(),
         HAL_RCC_GetPCLK1Freq(),
         HAL_RCC_GetPCLK2Freq());
  printf("build: %s %s\r\n", __DATE__, __TIME__);

  while (1)
  {
    /* ---- LED0 每 500ms 翻转 ---- */
    if (HAL_GetTick() - last_blink >= 500)
    {
      last_blink = HAL_GetTick();
      led0_state = !led0_state;
      led0_state ? led_on(0) : led_off(0);
    }

    /* ---- 按键扫描：KEY0/1/2 翻转 LED1，WK_UP 打印运行时间 ---- */
    uint8_t key = key_scan(0);
    switch (key)
    {
      case KEY0_PRES:
        led_toggle(1);
        printf("[KEY0] LED1 -> %s\r\n",
               HAL_GPIO_ReadPin(LED1_GPIO_Port, LED1_Pin) ? "OFF" : "ON");
        break;
      case KEY1_PRES:
        printf("[KEY1] tick = %lu ms\r\n", HAL_GetTick());
        break;
      case KEY2_PRES:
        printf("[KEY2] hello STM32F407!\r\n");
        break;
      case WKUP_PRES:
        printf("[WK_UP] uptime = %lu ms\r\n", HAL_GetTick());
        break;
      default:
        break;
    }
  }
}

/**
 * 系统时钟：HSE 8MHz -> PLL -> SYSCLK 168MHz
 * AHB /1 = 168MHz, APB1 /4 = 42MHz, APB2 /2 = 84MHz
 */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* 先开 PWR 时钟并设电压档位 1（168MHz 必须 Scale1） */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* PLL: 8MHz / M(8) * N(336) / P(2) = 168MHz；Q=7 -> 48MHz(USB) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM       = 8;
  RCC_OscInitStruct.PLL.PLLN       = 336;
  RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ       = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    Error_Handler();

  /* 总线分频 + Flash 5 等待周期 */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK   |
                                RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1  |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;   /* HCLK  168MHz */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;     /* APB1   42MHz */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;     /* APB2   84MHz */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    Error_Handler();
}

/**
 * 致命错误兜底：LED 快闪 + 死循环
 * 注意关中断后 SysTick 停走，HAL_Delay 失效，用空转延时
 */
static void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    led_on(0xFF);
    for (volatile uint32_t i = 0; i < 1000000; i++) { __NOP(); }
    led_off(0xFF);
    for (volatile uint32_t i = 0; i < 1000000; i++) { __NOP(); }
  }
}

#ifdef USE_FULL_ASSERT
/**
 * HAL 断言失败回调（stm32f4xx_hal_conf.h 打开 USE_FULL_ASSERT 后生效）
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  printf("ASSERT: %s:%lu\r\n", (const char *)file, (unsigned long)line);
}
#endif
