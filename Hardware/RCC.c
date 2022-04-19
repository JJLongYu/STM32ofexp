#include "RCC.h"

/**
 * @brief 设置HSE为系统系统时钟 系统时钟：外部晶振*RCC_PLLMul_x
 *
 * @param RCC_PLLMul_x
 */
void HSE_SetSysclk(uint32_t RCC_PLLMul_x)
{
    ErrorStatus HSEstatus = ERROR;
    // 把RCC寄存器的清零
    RCC_DeInit();

    //使能HSE
    RCC_HSEConfig(RCC_HSE_ON);

    //等待HSE工作
    HSEstatus = RCC_WaitForHSEStartUp();

    if (HSEstatus == SUCCESS)
    {
        // 使能预取指
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // 为什么要使用Flash这要的操作：https://blog.csdn.net/sunjiajiang/article/details/7075061
        FLASH_SetLatency(FLASH_Latency_2);

        //设置AHB时钟
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        //设置APB2的时钟
        RCC_PCLK2Config(RCC_HCLK_Div1);

        //设置APB1的时钟
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //设置PLL锁相环
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);

        //打开PLL锁相环
        RCC_PLLCmd(ENABLE);

        //等待锁相环PLL工作
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == SET)
            ;

        //设置系统时钟
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        //判断系统时钟是否为PLL时钟
        while (RCC_GetSYSCLKSource() != 0x08)
            ;
    }
    else
    {
        //如果HSE启动失败，用户可以在这里添加处理错误的代码；
    }
}
/**
 * @brief 设置HSI为内部时钟 系统时钟：HSI*RCC_PLLMul_x
 *
 * @param RCC_PLLMul_x
 */
void HSI_SetSysClk(uint32_t RCC_PLLMul_x)
{
    __IO uint32_t HSIStatus = 0;

    // 把RCC 寄存器复位成复位值
    RCC_DeInit();

    // 使能 HSI
    RCC_HSICmd(ENABLE);

    HSIStatus = RCC->CR & RCC_CR_HSIRDY;

    if (HSIStatus == RCC_CR_HSIRDY)
    {
        // 使能预取指
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);

        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        RCC_PCLK2Config(RCC_HCLK_Div1);

        // 配置 PLLCLK = HSE * RCC_PLLMul_x
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_x);

        // 使能PLL
        RCC_PLLCmd(ENABLE);

        // 等待PLL稳定
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
            ;

        // 选择系统时钟
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        while (RCC_GetSYSCLKSource() != 0x08)
            ;
    }
    else
    {
        /* 如果HSI 启动失败，用户可以在这里添加处理错误的代码 */
    }
}

/**
 * @brief  输出时钟信号 GPIO：PA8
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
