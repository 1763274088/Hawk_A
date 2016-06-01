#ifndef		__PPM_H_
#define		__PPM_H_

#include "stm32f10x.h"

//**********************************************��������****************************************************************************
#define PPM_HEART  		12000			  //ͬ��ͷ
#define PPM_NO_Ch   	500					//���ڴ�ֵΪ��Ч����
#define PPM_Ch        1200				//��Ч������Ĭ�����
#define PPM_P					400         //ÿ����

#define  PPM_1      GPIO_SetBits(GPIOA, GPIO_Pin_12)													
#define	 PPM_0	    GPIO_ResetBits(GPIOA, GPIO_Pin_12)	
#define  PPM_W			GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_12)
//**************************************************************************************************************************************


void  PPM_OUT_CH (u8 ch,u16 pwm);
void PPM_OUTPUT(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void TIM3_IRQHandler(void);


#endif
