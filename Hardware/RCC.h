#ifndef _RCC_ 
#define _RCC

#include "stm32f10x.h"

void HSE_SetSysclk(uint32_t RCC_PLLMul_x);
void HSI_SetSysClk(uint32_t RCC_PLLMul_x);
void MCO_GPIO_Config(uint8_t RCC_MCO);

#endif
