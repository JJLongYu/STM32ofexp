#ifndef _EXTI_H__
#define _EXTI_H__

#include "stm32f10x.h"

#define EXTI_GPIO_CLK RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO  
#define EXTI_GPIO_PIN GPIO_Pin_0
#define EXTI_GPIO_PORT GPIOB
#define EXTI_GPIO_PortSource_X GPIO_PortSourceGPIOB
#define EXTI_GPIO_PinSource_X GPIO_PinSource0
#define EXTI_LINE_X EXTI_Line0

#define EXTIx_IRQn EXTI0_IRQn
#define EXTI_IRQHandler EXTI0_IRQHandler

void JLY_EXTI_Init(void);

#endif
