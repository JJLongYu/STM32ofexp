#include "advanced_TIM.h"

void PWM_IC_Measurement(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStruct.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //���õ�ʱtDTS��ʱ�䣬��IC�˲��й�
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 0xffff;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //ʹ��ֱ��ʱ��ͨ��1��Ӧ���ǹܽ�1��ʹ�÷�ֱ��ʱ��ͨ��1��Ӧ��ʱ�ܽ�2.
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; //���ö��ٸ���Ч����һ����������Ч��
    TIM_ICInitStruct.TIM_ICFilter = 0x0;               //����һ����ƽ�źų����೤ʱ�����Ϊ��һ����Ч���źţ�ʱ�����  = �� 1/��72M/TIM_ClockDivision��/0x0�������ֵ��*N
    TIM_PWMIConfig(TIM1, &TIM_ICInitStruct);           //����TIM_ICInit()�������������ڣ�TIM_ICInitֻʹ��һ��ͨ����TIM_PWMIConfigʹ������ͨ�����ҵڶ���ͨ���ļ��Իᷴת

    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); //������һ·Ϊ�������Ӧ����һ·�����Ϊ���ڣ��������ѡ��TIM_TS_TI1FP1����TIM_TS_TI2FP2��ֱ������ֱ���Լ�ͨ��1����ͨ��2�й�
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable); // Ŀǰ��PWM����ģʽ������ ��û�������������MSMλ����ûɶӰ�졣����������

    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

__IO uint16_t Frequency_temp = 0;
__IO uint16_t DutyCycle_temp = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;

void TIM1_CC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
        DutyCycle_temp = TIM_GetCapture1(TIM1);
        Frequency_temp = TIM_GetCapture2(TIM1);
    }
    if (DutyCycle_temp != 0)
    {
        /* ռ�ձȼ��� */
        DutyCycle = (float)((Frequency_temp + 1) * 100) / (DutyCycle_temp + 1);

        /* Ƶ�ʼ��� */
        Frequency = (72000000 / (72 - 1 + 1)) / (float)(DutyCycle_temp + 1);
    }
    else
    {
        DutyCycle = 0;
        Frequency = 0;
    }
}

void Brake_Complementary_Output_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_BDTRInitTypeDef TIM_BDTRInitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //���õ�ʱtDTS��ʱ�䣬��IC�˲��й�
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 10 - 1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;  //��������ļ�������Ҫ��ԭ����һ�£���ΪӲ���Ѿ������˲���Ҫ���������Ϊ�����ˡ�
    TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset; //����ʱ��״̬����ɲ����ĵ�ƽ��
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Set; // һ��Ҫ��TIM_OCIdleState�����෴�ļ���
    TIM_OCInitStruct.TIM_Pulse = 5;
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_1; //��ֹ���������ṩ���ձ������������ȼ�������BDTR�Ĵ�������Ķ�������
    TIM_BDTRInitStruct.TIM_DeadTime = 11;
    TIM_BDTRInitStruct.TIM_Break = TIM_Break_Enable;
    TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);

    TIM_Cmd(TIM1, ENABLE);

    TIM_CtrlPWMOutputs(TIM1, ENABLE); //���������ʹ�� ��BDTR�Ĵ�����MOEλ��1.�˺���ֻ���ڸ߼���ʱ��ʱ��Ҫ���ã���Ϊֻ�и߼���ʱ����������ɲ�����ܡ�
}
