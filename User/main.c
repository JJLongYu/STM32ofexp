#include "stm32f10x.h" // �൱��51��Ƭ���е�  #include <reg51.h>
#include "JLY_gpio.h"
#include "JLY_LED.h"
#include "EXTI.h"
#include "SysTick.h"

int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	LED_GPIO_Config();
	SysTick_Configuration();
	while (1)
	{
		/* code */
	}
}
