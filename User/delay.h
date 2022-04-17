#ifndef _DELAY_H_
#define _DELAY_H_


#include "stm32f10x.h"
#include "core_cm3.h"

void SysTick_Delay_us(uint32_t us);
void SysTick_Delay_ms(uint32_t ms);
void Delay_us(uint16_t us);
void Delay_ms(uint32_t ms);



#endif

