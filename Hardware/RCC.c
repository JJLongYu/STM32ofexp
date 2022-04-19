#include "RCC.h"

/**
 * @brief ����HSEΪϵͳϵͳʱ�� ϵͳʱ�ӣ��ⲿ����*RCC_PLLMul_x
 *
 * @param RCC_PLLMul_x
 */
void HSE_SetSysclk(uint32_t RCC_PLLMul_x)
{
    ErrorStatus HSEstatus = ERROR;
    // ��RCC�Ĵ���������
    RCC_DeInit();

    //ʹ��HSE
    RCC_HSEConfig(RCC_HSE_ON);

    //�ȴ�HSE����
    HSEstatus = RCC_WaitForHSEStartUp();

    if (HSEstatus == SUCCESS)
    {
        // ʹ��Ԥȡָ
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // ΪʲôҪʹ��Flash��Ҫ�Ĳ�����https://blog.csdn.net/sunjiajiang/article/details/7075061
        FLASH_SetLatency(FLASH_Latency_2);

        //����AHBʱ��
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        //����APB2��ʱ��
        RCC_PCLK2Config(RCC_HCLK_Div1);

        //����APB1��ʱ��
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //����PLL���໷
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);

        //��PLL���໷
        RCC_PLLCmd(ENABLE);

        //�ȴ����໷PLL����
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == SET)
            ;

        //����ϵͳʱ��
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        //�ж�ϵͳʱ���Ƿ�ΪPLLʱ��
        while (RCC_GetSYSCLKSource() != 0x08)
            ;
    }
    else
    {
        //���HSE����ʧ�ܣ��û�������������Ӵ������Ĵ��룻
    }
}
/**
 * @brief ����HSIΪ�ڲ�ʱ�� ϵͳʱ�ӣ�HSI*RCC_PLLMul_x
 *
 * @param RCC_PLLMul_x
 */
void HSI_SetSysClk(uint32_t RCC_PLLMul_x)
{
    __IO uint32_t HSIStatus = 0;

    // ��RCC �Ĵ�����λ�ɸ�λֵ
    RCC_DeInit();

    // ʹ�� HSI
    RCC_HSICmd(ENABLE);

    HSIStatus = RCC->CR & RCC_CR_HSIRDY;

    if (HSIStatus == RCC_CR_HSIRDY)
    {
        // ʹ��Ԥȡָ
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);

        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        RCC_PCLK2Config(RCC_HCLK_Div1);

        // ���� PLLCLK = HSE * RCC_PLLMul_x
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_x);

        // ʹ��PLL
        RCC_PLLCmd(ENABLE);

        // �ȴ�PLL�ȶ�
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
            ;

        // ѡ��ϵͳʱ��
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        while (RCC_GetSYSCLKSource() != 0x08)
            ;
    }
    else
    {
        /* ���HSI ����ʧ�ܣ��û�������������Ӵ������Ĵ��� */
    }
}

/**
 * @brief  ���ʱ���ź� GPIO��PA8
 * @param  RCC_MCO: specifies the clock source to output.
 *
 *   For @b STM32_Connectivity_line_devices, this parameter can be one of the
 *   following values:
 *     @arg RCC_MCO_NoClock: No clock selected
 *     @arg RCC_MCO_SYSCLK: System clock selected
 *     @arg RCC_MCO_HSI: HSI oscillator clock selected
 *     @arg RCC_MCO_HSE: HSE oscillator clock selected
 *     @arg RCC_MCO_PLLCLK_Div2: PLL clock divided by 2 selected
 *     @arg RCC_MCO_PLL2CLK: PLL2 clock selected
 *     @arg RCC_MCO_PLL3CLK_Div2: PLL3 clock divided by 2 selected
 *     @arg RCC_MCO_XT1: External 3-25 MHz oscillator clock selected
 *     @arg RCC_MCO_PLL3CLK: PLL3 clock selected
 *
 *   For  @b other_STM32_devices, this parameter can be one of the following values:
 *     @arg RCC_MCO_NoClock: No clock selected
 *     @arg RCC_MCO_SYSCLK: System clock selected
 *     @arg RCC_MCO_HSI: HSI oscillator clock selected
 *     @arg RCC_MCO_HSE: HSE oscillator clock selected
 *     @arg RCC_MCO_PLLCLK_Div2: PLL clock divided by 2 selected
 *
 * @retval None
 */
void MCO_GPIO_Config(uint8_t RCC_MCO)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);

    RCC_MCOConfig(RCC_MCO);
}
