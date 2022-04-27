#include "EXTI.h"
#include "JLY_LED.h"
/**
 * @brief �ⲿ�жϹܽų�ʼ��
 *
 */
static void EXTI_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(EXTI_GPIO_CLK, ENABLE);   //ʹ���ⲿ�ж���Ҫ��������ʱ��
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = EXTI_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(EXTI_GPIO_PORT, &GPIO_InitStruct);
    GPIO_EXTILineConfig(EXTI_GPIO_PortSource_X,EXTI_GPIO_PinSource_X); //����AFIO->EXTICR�Ĵ���ѡ��EXTIx�ⲿ�жϵ�����Դ
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_LINE_X;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����ͨ���ⲿ�жϣ����¼��Ǹ�����һ�������¼������簴��һ�κ�ADCת��һ�Ρ� �䲻��Ҫ���жϷ�������ΪӲ������
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);
}

/**
 * @brief �ⲿ�ж�NVIC��ʼ��
 *
 */
static void EXTI_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStruct.NVIC_IRQChannel = EXTIx_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
}

/**
 * @brief �ⲿ�жϳ�ʼ��
 *
 */
void JLY_EXTI_Init(void)
{
    EXTI_GPIO_Init();
    EXTI_NVIC_Init();
}

/*************************�ⲿ�жϷ�����**************************************/

/**
 * @brief    �ⲿ�жϷ�����
 *
 */
void EXTI_IRQHandler(void)
{  
    if (EXTI_GetFlagStatus(EXTI_Line0)==SET)
    {
        LED1_TOGGLE;
    }
    EXTI_ClearFlag(EXTI_Line0);
}
