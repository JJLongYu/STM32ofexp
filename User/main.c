#include "stm32f10x.h" // 相当于51单片机中的  #include <reg51.h>
#include "JLY_gpio.h"
#include "JLY_LED.h"
#include "EXTI.h"
#include "SysTick.h"
#include "basic_TIM.h"
#include "general_TIM.h"

int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	LED_GPIO_Config();
	DBGMCU_Config(DBGMCU_TIM2_STOP,ENABLE);
	Pulse_Width_Measurement();

	while (1)
	{
		/* code */
	}
}
