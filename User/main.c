#include "stm32f10x.h" // �൱��51��Ƭ���е�  #include <reg51.h>
#include "delay.h"
#include "JLY_gpio.h"
#include "JLY_LED.h"
#include "EXTI.h"

int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	LED_GPIO_Config();
	JLY_EXTI_Init();
	while (1)
	{
		/* code */
	}
}
