#include "basic_TIM.h"

/** ��ʹ�÷���ʱ����ʹ����û�����У����Ƕ�ʱ����Ȼ���������������Ҫ����
 * DBGMCU_Config(DBGMCU_TIM6_STOP,ENABLE);�������رշ�����ͣʱ�����Ĺ���
 **/

/**
 * @brief ������ʱ��NVIC��ʼ������TIM6Ϊ��
 *
 */
static void Basic_TIM_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}
/**
 * @brief ������ʱ������,��TIM6Ϊ��
 *
 */
static void Basic_TIM_Config_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 10000;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM6, ENABLE);
}
/**
 * @brief ������ʱ����ʼ������TIM6Ϊ��
 *
 */
void Basci_TIM_Init(void)
{
    Basic_TIM_NVIC_Init();
    Basic_TIM_Config_Init();
}

/**
 * @brief ������ʱ���жϷ���������TIM6Ϊ��
 *
 */
void TIM6_IRQHandler()
{
    if (TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == SET)
    {
        TIM_ClearFlag(TIM6, TIM_FLAG_Update);
    }
}
