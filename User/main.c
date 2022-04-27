#include "stm32f10x.h" // 相当于51单片机中的  #include <reg51.h>
#include "JLY_gpio.h"
#include "JLY_LED.h"
#include "EXTI.h"
#include "SysTick.h"

int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	LED_GPIO_Config();
	SysTick_Configuration();
	while (1)
	{
		/* code */
	}
}
