#ifndef		__PPM_H_
#define		__PPM_H_

#include "stm32f10x.h"

//**********************************************参数设置****************************************************************************
#define PPM_HEART  		12000			  //同步头
#define PPM_NO_Ch   	500					//低于此值为无效输入
#define PPM_Ch        1200				//无效输入做默认输出
#define PPM_P					400         //每组间隔

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
