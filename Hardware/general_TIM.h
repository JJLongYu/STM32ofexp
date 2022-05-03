#ifndef __GENERAL_TIM_H_
#define __GENERAL_TIM_H_

#include "stm32f10x.h"

void External_Clock_Mode_One_Init(void);
void External_Clock_Mode_Two_Init(void);
void Pulse_Width_Measurement(void);

typedef struct capture_struct
{
    uint8_t Capture_FinishFlag; // ���������־λ
    uint8_t Capture_StartFlag;  // ����ʼ��־λ
    uint16_t Capture_CcrValue;  // ����Ĵ�����ֵ
    uint16_t Capture_Period;    // �Զ���װ�ؼĴ������±�־
}TIM_ICUserValueTypeDef;

#endif /* __GENERAL_TIM_H_ */
