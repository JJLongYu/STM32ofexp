#include "general_TIM.h"

/**
 * @brief ʹ���ⲿʱ��ģʽ1�䵱ʱ��Դ�����Գ䵱������ʹ�ã�ÿ��һ�α�Ե����Ϳ��Լ���һ�Ρ�
 *   �˺������õ���TIM2��CH1��TI1FP1��
 *  ���Ըĳ����Ⲷ��ʱ����CH1��CH2��TI1FP1��TI1FP2
 */

void External_Clock_Mode_One_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0; //ʱ�ӵķ�Ƶϵ����Ҫ̫�ߣ����Ϊ1��Ƶ����Ϊ��������ʹ���ⲿʱ��Դ�䵱�����������Ƶ̫�����ɴ����ܶ�βŻ����һ��
    TIM_TimeBaseInitStruct.TIM_Period = 10000;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //�ڱ������п���д���Բ�д������PWM���벶���л�ʹ��
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //�ڱ������п���д���Բ�д������PWM���벶���л�ʹ��
    TIM_ICInitStruct.TIM_ICFilter = 0x3;                         //�൱�ڼ����ز���һ�Σ����忴�ֲᣬһ��Ϊ0x00
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
#if 0 //ʹ���ⲿʱ��ģʽ1��ETR��ʹ��ʱ�ǵð�IC�Ľṹ������֮���ɾ����
    TIM_ETRClockMode1Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0);
    TIM_SelectInputTrigger(TIM2, TIM_TS_ETRF);
#endif
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_External1);

    TIM_Cmd(TIM2, ENABLE);
}
/**
 * @brief ʹ���ⲿʱ��ģʽ2�䵱ʱ��Դ�����Գ䵱������ʹ�ã�ÿ��һ�α�Ե����Ϳ��Լ���һ�Ρ�
 * ��ӵ�е����ĺ���ȥ���á�һ�������͸㶨�ˡ�
 *
 */
void External_Clock_Mode_Two_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0; //ʱ�ӵķ�Ƶϵ����Ҫ̫�ߣ����Ϊ1��Ƶ����Ϊ��������ʹ���ⲿʱ��Դ�䵱�����������Ƶ̫�����ɴ����ܶ�βŻ����һ��
    TIM_TimeBaseInitStruct.TIM_Period = 10000;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    //�����ⲿʱ��ģʽ2��������TIM_ClockDivision���˲�ϵ�����ǺͲ����һ��ģ����Բ�����IC�Ľṹ��ȥ����ETR�ġ�
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0);

    TIM_Cmd(TIM2, ENABLE);
}
