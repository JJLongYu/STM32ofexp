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

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //设置的时tDTS的时间，于IC滤波有关
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 0xffff;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //使用直连时，通道1对应的是管脚1，使用非直连时，通道1对应的时管脚2.
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1; //设置多少个有效沿算一个真正的有效沿
    TIM_ICInitStruct.TIM_ICFilter = 0x0;               //设置一个电平信号持续多长时间才认为是一个有效的信号，时间计算  = （ 1/（72M/TIM_ClockDivision）/0x0所代表的值）*N
    TIM_PWMIConfig(TIM1, &TIM_ICInitStruct);           //其与TIM_ICInit()函数的区别在于：TIM_ICInit只使能一个通道，TIM_PWMIConfig使能两个通道，且第二个通道的极性会反转

    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); //设置哪一路为触发则对应的哪一路捕获的为周期，这个具体选择TIM_TS_TI1FP1还是TIM_TS_TI2FP2与直连、非直连以及通道1还是通道2有关
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable); // 目前对PWM捕获模式来看， 有没有这个函数设置MSM位好像没啥影响。。。。。。

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
        /* 占空比计算 */
        DutyCycle = (float)((Frequency_temp + 1) * 100) / (DutyCycle_temp + 1);

        /* 频率计算 */
        Frequency = (72000000 / (72 - 1 + 1)) / (float)(DutyCycle_temp + 1);
    }
    else
    {
        DutyCycle = 0;
        Frequency = 0;
    }
}
