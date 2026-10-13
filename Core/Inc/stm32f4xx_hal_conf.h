/**
 * stm32f4xx_hal_conf.h — HAL 配置（正点原子探索者 STM32F407ZGT6）
 *
 * 外部晶振 HSE = 8MHz，经 PLL 倍频到 168MHz（在 main.c SystemClock_Config 配置）。
 * 只使能当前用到的外设模块，需要新外设时在下面打开对应 MODULE_ENABLED，
 * 并在 CMake 无需改动（HAL 源码全部参与编译）。
 */
#ifndef STM32F4xx_HAL_CONF_H
#define STM32F4xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== 模块使能 ==================== */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED

/* 按需打开：
#define HAL_ADC_MODULE_ENABLED
#define HAL_CAN_MODULE_ENABLED
#define HAL_CRC_MODULE_ENABLED
#define HAL_CRYP_MODULE_ENABLED
#define HAL_DAC_MODULE_ENABLED
#define HAL_DCMI_MODULE_ENABLED
#define HAL_DMA2D_MODULE_ENABLED
#define HAL_ETH_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
#define HAL_FLASH_EX_MODULE_ENABLED   (已含在 FLASH)
#define HAL_FMC_MODULE_ENABLED
#define HAL_SRAM_MODULE_ENABLED
#define HAL_SDRAM_MODULE_ENABLED
#define HAL_NAND_MODULE_ENABLED
#define HAL_NOR_MODULE_ENABLED
#define HAL_PCCARD_MODULE_ENABLED
#define HAL_HASH_MODULE_ENABLED
#define HAL_HCD_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_I2S_MODULE_ENABLED
#define HAL_IWDG_MODULE_ENABLED
#define HAL_LTDC_MODULE_ENABLED
#define HAL_PCD_MODULE_ENABLED
#define HAL_RCC_EX_MODULE_ENABLED     (已含在 RCC)
#define HAL_RTC_MODULE_ENABLED
#define HAL_SAI_MODULE_ENABLED
#define HAL_SD_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_EX_MODULE_ENABLED    (已含在 UART)
#define HAL_USART_MODULE_ENABLED
#define HAL_WWDG_MODULE_ENABLED
*/

/* ==================== 晶振/时钟参数 ==================== */
/* 探索者板外部高速晶振 = 8MHz */
#define HSE_VALUE              8000000U
#define HSE_STARTUP_TIMEOUT    100U

#define HSI_VALUE             16000000U   /* 内部高速 RC = 16MHz */
#define LSI_VALUE             32000U      /* 内部低速 RC = 32kHz */
#define LSE_VALUE             32768U      /* 外部低速晶振 32.768kHz */
#define LSE_STARTUP_TIMEOUT   5000U
#define EXTERNAL_CLOCK_VALUE  12288000U   /* I2S 外部时钟 */

/* 内核电压 2.7V~3.6V，对应 FLASH 等待周期由 HAL 按 168MHz 自动取 5WS */
#define VDD_VALUE             3300U

/* SysTick 中断优先级（最低，不抢占业务中断） */
#define TICK_INT_PRIORITY     0x0FU

/* 不用 RTOS；FreeRTOS 时 HAL 时基需换用 TIM，这里保持默认 SysTick */
#define USE_RTOS              0U

/* FLASH 预取 / ART 指令与数据缓存：全开，榨 168MHz 性能 */
#define PREFETCH_ENABLE       1U
#define INSTRUCTION_CACHE_ENABLE   1U
#define DATA_CACHE_ENABLE           1U

#define USE_SPI_CRC           0U

/* ==================== 回调注册方式 ==================== */
/* 0 = 弱函数重写（常用）；1 = 运行时 RegisterCallback */
#define USE_HAL_UART_REGISTER_CALLBACKS   0U
#define USE_HAL_ADC_REGISTER_CALLBACKS    0U

/* ==================== 断言 ==================== */
/* #define USE_FULL_ASSERT 1U */   /* 打开后 HAL 函数入口做参数检查 */

#include <stdint.h>

/* ==================== HAL 头文件汇总（勿动） ==================== */
#ifdef HAL_RCC_MODULE_ENABLED
  #include "stm32f4xx_hal_rcc.h"
#endif
#ifdef HAL_GPIO_MODULE_ENABLED
  #include "stm32f4xx_hal_gpio.h"
#endif
#ifdef HAL_DMA_MODULE_ENABLED
  #include "stm32f4xx_hal_dma.h"
#endif
#ifdef HAL_CORTEX_MODULE_ENABLED
  #include "stm32f4xx_hal_cortex.h"
#endif
#ifdef HAL_ADC_MODULE_ENABLED
  #include "stm32f4xx_hal_adc.h"
#endif
#ifdef HAL_CAN_MODULE_ENABLED
  #include "stm32f4xx_hal_can.h"
#endif
#ifdef HAL_CRC_MODULE_ENABLED
  #include "stm32f4xx_hal_crc.h"
#endif
#ifdef HAL_CRYP_MODULE_ENABLED
  #include "stm32f4xx_hal_cryp.h"
#endif
#ifdef HAL_DAC_MODULE_ENABLED
  #include "stm32f4xx_hal_dac.h"
#endif
#ifdef HAL_DCMI_MODULE_ENABLED
  #include "stm32f4xx_hal_dcmi.h"
#endif
#ifdef HAL_DMA2D_MODULE_ENABLED
  #include "stm32f4xx_hal_dma2d.h"
#endif
#ifdef HAL_ETH_MODULE_ENABLED
  #include "stm32f4xx_hal_eth.h"
#endif
#ifdef HAL_EXTI_MODULE_ENABLED
  #include "stm32f4xx_hal_exti.h"
#endif
#ifdef HAL_FLASH_MODULE_ENABLED
  #include "stm32f4xx_hal_flash.h"
#endif
#ifdef HAL_SRAM_MODULE_ENABLED
  #include "stm32f4xx_hal_sram.h"
#endif
#ifdef HAL_NOR_MODULE_ENABLED
  #include "stm32f4xx_hal_nor.h"
#endif
#ifdef HAL_NAND_MODULE_ENABLED
  #include "stm32f4xx_hal_nand.h"
#endif
#ifdef HAL_PCCARD_MODULE_ENABLED
  #include "stm32f4xx_hal_pccard.h"
#endif
#ifdef HAL_SDRAM_MODULE_ENABLED
  #include "stm32f4xx_hal_sdram.h"
#endif
#ifdef HAL_HASH_MODULE_ENABLED
  #include "stm32f4xx_hal_hash.h"
#endif
#ifdef HAL_I2C_MODULE_ENABLED
  #include "stm32f4xx_hal_i2c.h"
#endif
#ifdef HAL_I2S_MODULE_ENABLED
  #include "stm32f4xx_hal_i2s.h"
#endif
#ifdef HAL_IWDG_MODULE_ENABLED
  #include "stm32f4xx_hal_iwdg.h"
#endif
#ifdef HAL_LTDC_MODULE_ENABLED
  #include "stm32f4xx_hal_ltdc.h"
#endif
#ifdef HAL_PWR_MODULE_ENABLED
  #include "stm32f4xx_hal_pwr.h"
#endif
#ifdef HAL_RTC_MODULE_ENABLED
  #include "stm32f4xx_hal_rtc.h"
#endif
#ifdef HAL_SAI_MODULE_ENABLED
  #include "stm32f4xx_hal_sai.h"
#endif
#ifdef HAL_SD_MODULE_ENABLED
  #include "stm32f4xx_hal_sd.h"
#endif
#ifdef HAL_SPI_MODULE_ENABLED
  #include "stm32f4xx_hal_spi.h"
#endif
#ifdef HAL_TIM_MODULE_ENABLED
  #include "stm32f4xx_hal_tim.h"
#endif
#ifdef HAL_UART_MODULE_ENABLED
  #include "stm32f4xx_hal_uart.h"
#endif
#ifdef HAL_USART_MODULE_ENABLED
  #include "stm32f4xx_hal_usart.h"
#endif
#ifdef HAL_IRDA_MODULE_ENABLED
  #include "stm32f4xx_hal_irda.h"
#endif
#ifdef HAL_SMARTCARD_MODULE_ENABLED
  #include "stm32f4xx_hal_smartcard.h"
#endif
#ifdef HAL_WWDG_MODULE_ENABLED
  #include "stm32f4xx_hal_wwdg.h"
#endif
#ifdef HAL_PCD_MODULE_ENABLED
  #include "stm32f4xx_hal_pcd.h"
#endif
#ifdef HAL_HCD_MODULE_ENABLED
  #include "stm32f4xx_hal_hcd.h"
#endif
#ifdef HAL_FMPI2C_MODULE_ENABLED
  #include "stm32f4xx_hal_fmpi2c.h"
#endif
#ifdef HAL_FMPSMBUS_MODULE_ENABLED
  #include "stm32f4xx_hal_fpsmbus.h"
#endif
#ifdef HAL_SPDIFRX_MODULE_ENABLED
  #include "stm32f4xx_hal_spdifrx.h"
#endif
#ifdef HAL_DFSDM_MODULE_ENABLED
  #include "stm32f4xx_hal_dfsdm.h"
#endif
#ifdef HAL_LPTIM_MODULE_ENABLED
  #include "stm32f4xx_hal_lptim.h"
#endif

/* 断言宏：默认关闭；打开 USE_FULL_ASSERT 后定位到 assert_failed */
#ifdef USE_FULL_ASSERT
  #include "stdio.h"
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t *file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif

#ifdef __cplusplus
}
#endif

#endif /* STM32F4xx_HAL_CONF_H */
