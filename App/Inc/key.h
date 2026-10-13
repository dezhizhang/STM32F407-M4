/**
 * key.h — 按键驱动（正点原子探索者）
 */
#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define KEY0_PRES  1U   /* KEY0 按下 */
#define KEY1_PRES  2U   /* KEY1 按下 */
#define KEY2_PRES  3U   /* KEY2 按下 */
#define WKUP_PRES  4U   /* WK_UP 按下 */

void    key_init(void);
uint8_t key_scan(uint8_t mode);

#endif /* __KEY_H */
