#include "general_TIM.h"

/** 在使用仿真时，即使仿真没有运行，但是定时器仍然会继续计数，所以要调用
 * DBGMCU_Config(DBGMCU_TIM6_STOP,ENABLE);函数来关闭仿真暂停时计数的功能
 **/

/**
 * @brief 使用外部时钟模式1充当时钟源，可以充当计数器使用，每有一次边缘跳变就可以计数一次。
 *   此函数设置的是TIM2的CH1的TI1FP1。
 *  可以改成任意捕获定时器的CH1和CH2的TI1FP1和TI1FP2
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
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0; //时钟的分频系数不要太高，最好为1分频。因为本函数是使用外部时钟源充当触发，如果分频太多会造成触发很多次才会计数一次
    TIM_TimeBaseInitStruct.TIM_Period = 10000;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //在本函数中可以写可以不写，这在PWM输入捕获中会使用
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //在本函数中可以写可以不写，这在PWM输入捕获中会使用
    TIM_ICInitStruct.TIM_ICFilter = 0x3;                         //相当于几个沿采样一次，具体看手册，一般为0x00
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
#if 0 //使用外部时钟模式1的ETR，使用时记得把IC的结构体配置之类的删除。
    TIM_ETRClockMode1Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0);
    TIM_SelectInputTrigger(TIM2, TIM_TS_ETRF);
#endif
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_External1);

    TIM_Cmd(TIM2, ENABLE);
}
/**
 * @brief 使用外部时钟模式2充当时钟源，可以充当计数器使用，每有一次边缘跳变就可以计数一次。
 * 其拥有单独的函数去设置。一个函数就搞定了。
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
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0; //时钟的分频系数不要太高，最好为1分频。因为本函数是使用外部时钟源充当触发，如果分频太多会造成触发很多次才会计数一次
    TIM_TimeBaseInitStruct.TIM_Period = 10000;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    //设置外部时钟模式2，其设置TIM_ClockDivision和滤波系数不是和捕获的一起的，所以不能用IC的结构体去设置ETR的。
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0);

    TIM_Cmd(TIM2, ENABLE);
}

void Pulse_Width_Measurement(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStruct.TIM_ICFilter = 0x0;
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInit(TIM2, &TIM_ICInitStruct);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update | TIM_IT_CC1);
    TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}

TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0, 0, 0, 0};
uint32_t TIMx_IC_Per = 0;
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_FLAG_Update) == SET)
    {
        TIM_ICUserValueStructure.Capture_Period++;
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
    }
    if (TIM_GetITStatus(TIM2, TIM_FLAG_CC1) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_CC1);
        if (TIM_ICUserValueStructure.Capture_StartFlag == 0)
        {
            TIM_SetCounter(TIM2, 0);
            TIM_ICUserValueStructure.Capture_StartFlag = 1;
            TIM_ICUserValueStructure.Capture_Period = 0;
            TIM_ICUserValueStructure.Capture_FinishFlag = 0;
            TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_Low);
        }
        else
        {
            TIM_ICUserValueStructure.Capture_CcrValue = TIM_GetCapture1(TIM2);
            TIM_ICUserValueStructure.Capture_StartFlag = 0;
            TIM_ICUserValueStructure.Capture_FinishFlag = 1;
            TIM_ICUserValueStructure.Capture_Period = 0;
            TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_High);
            TIMx_IC_Per = TIM_ICUserValueStructure.Capture_CcrValue + 1 + (TIM_ICUserValueStructure.Capture_Period * 10000);
        }
    }
}
