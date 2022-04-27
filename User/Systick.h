#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f10x.h"
#include "core_cm3.h"

#define TICK_PER_SECOND 4 // 每秒钟中断次数

#define Delay_us delay_us

void SysTick_Configuration(void);
void SysTick_Delay_us(uint32_t us);
void SysTick_Delay_ms(uint32_t ms);
void Delay_us(uint16_t us);
void Delay_ms(uint32_t ms);

#endif
