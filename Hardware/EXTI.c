#include "EXTI.h"
#include "JLY_LED.h"
/**
 * @brief 外部中断管脚初始化
 *
 */
static void EXTI_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(EXTI_GPIO_CLK, ENABLE);   //使用外部中断需要开启复用时钟
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = EXTI_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(EXTI_GPIO_PORT, &GPIO_InitStruct);
    GPIO_EXTILineConfig(EXTI_GPIO_PortSource_X,EXTI_GPIO_PinSource_X); //配置AFIO->EXTICR寄存器选择EXTIx外部中断的输入源
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_LINE_X;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //中断是普通的外部中断，而事件是给外设一个触发事件，比如按下一次后ADC转换一次。 其不需要进中断服务函数，为硬件操作
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);
}

/**
 * @brief 外部中断NVIC初始化
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
 * @brief 外部中断初始化
 *
 */
void JLY_EXTI_Init(void)
{
    EXTI_GPIO_Init();
    EXTI_NVIC_Init();
}

/*************************外部中断服务函数**************************************/

/**
 * @brief    外部中断服务函数
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
