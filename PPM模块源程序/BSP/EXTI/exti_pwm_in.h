
#ifndef		__EXTI_PWM_IN_H_
#define		__EXTI_PWM_IN_H_

#include "config.h"

#define waring_out	900

void EXTI_Configuration(void);	//�ⲿ�жϳ�ʼ��
void TIM2_Configuration(void);	//TIM2��ʱ����ʼ��
void TIM3_Configuration(void);
void TIM4_Configuration(void);

extern void waring_ouput(void);//�������
extern void PWMIN_Stop(u8 data);//������Ч�˿�

#endif
