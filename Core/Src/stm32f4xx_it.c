/**
 * stm32f4xx_it.c — 中断服务函数
 */
#include "main.h"
#include "stm32f4xx_it.h"

/**
 * SysTick 1ms 中断：HAL 时基。HAL_Delay / HAL_GetTick 靠它走。
 */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/* ---------------- 以下为故障兜底，正常永远进不来 ---------------- */

void NMI_Handler(void) { }

void HardFault_Handler(void)
{
  /* 硬件错误：栈溢出/野指针/非法指令。挂在这里方便调试器抓现场 */
  while (1) { }
}

void MemManage_Handler(void)  { while (1) { } }
void BusFault_Handler(void)   { while (1) { } }
void UsageFault_Handler(void) { while (1) { } }

void SVC_Handler(void)    { }
void DebugMon_Handler(void) { }
void PendSV_Handler(void) { }
