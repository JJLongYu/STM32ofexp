#ifndef __GENERAL_TIM_H_
#define __GENERAL_TIM_H_

#include "stm32f10x.h"

void External_Clock_Mode_One_Init(void);
void External_Clock_Mode_Two_Init(void);
void Pulse_Width_Measurement(void);

typedef struct capture_struct
{
    uint8_t Capture_FinishFlag; // 捕获结束标志位
    uint8_t Capture_StartFlag;  // 捕获开始标志位
    uint16_t Capture_CcrValue;  // 捕获寄存器的值
    uint16_t Capture_Period;    // 自动重装载寄存器更新标志
}TIM_ICUserValueTypeDef;

#endif /* __GENERAL_TIM_H_ */
