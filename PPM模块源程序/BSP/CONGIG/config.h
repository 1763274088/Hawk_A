#ifndef		__CONFIG_H_
#define		__CONFIG_H_
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"


void NVIC_Configuration(void);
void RCC_Configuration(void);

#if Print_f == 1
#include <stdio.h>
#endif

#define SYS_RUN_PWM		  1	//PWMģʽ
#define SYS_RUN_PPM		  2	//PPMģʽ
#define SYS_RUN_SBUS		3	//SBUSģʽ

#define OUT_PWM_MAX	2080	//������
#define OUT_PWM_MIN	880		//�����С

extern u8  sbusData[25] ;		//SBUS��������								
extern u16 channels[];			//sbusͨ��

#endif
