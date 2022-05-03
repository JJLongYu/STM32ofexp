#include "stm32f10x.h" // �൱��51��Ƭ���е�  #include <reg51.h>
#include "JLY_gpio.h"
#include "JLY_LED.h"
#include "EXTI.h"
#include "SysTick.h"
#include "basic_TIM.h"
#include "general_TIM.h"

int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	LED_GPIO_Config();
	DBGMCU_Config(DBGMCU_TIM2_STOP,ENABLE);
	Pulse_Width_Measurement();

	while (1)
	{
		/* code */
	}
}
