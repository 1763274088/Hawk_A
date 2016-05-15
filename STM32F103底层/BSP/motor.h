/********************************************************************************
  * @file    motor.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    14-12-2015
  * @brief   Header for motor.c module
*******************************************************************************/ 
#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "stm32f10x.h"
#include "stdint.h"

//******************************************************//
/*****	PWM	time Configuration	*****/
//ȫ��ͳһPWM�����ã��������
#define PWMPeriod      				 40000			//PWM������			50 HZ   
#define PWM_TIM_Prescaler      36					//PWM��ϵͳʱ�ӷ�Ƶϵ��
#define PWM_TIM_ClockDivision  0 					//PWM��ʱ�ӷ�Ƶϵ��
#define PWM_Max								 4000				//ռ�ձ�Ϊ10%
#define PWM_Min								 2000				//ռ�ձ�Ϊ5%
#define PWM_Upper_limit			   1200				//PWM��������		ĿǰԤ��10%������
#define PWM_Lower_limit				 0					//PWM��������
//******************************************************//

//******************************************************//
/*****	PWM	Pin	definition	*****/
//M1��M8�Ķ�Ӧ�ܽŷֱ�Ϊ��PA0/PA1/PB8/PB9/PA6/PA7/PB0/PB1
//�ֱ��Ӧ�Ķ�ʱ��Ϊ��TIM2_CH1/TIM2_CH2/TIM4_CH3/TIM4_CH4
//										TIM3_CH1/TIM3_CH2/TIM3_CH3/TIM3_CH4
#define PWM_M1_pin     GPIO_Pin_0
#define PWM_M2_pin     GPIO_Pin_1
#define PWM_M3_pin     GPIO_Pin_8
#define PWM_M4_pin     GPIO_Pin_9
#define PWM_M5_pin     GPIO_Pin_6
#define PWM_M6_pin     GPIO_Pin_7
#define PWM_M7_pin     GPIO_Pin_0
#define PWM_M8_pin     GPIO_Pin_1
//******************************************************//



/*******************************************************************************
//PWM�������弯��
*******************************************************************************/
static void TIM2_Configuration(void);//TIM2��ͨ���ֱ����PWM1/PWM2,��Ӧ����ΪPA0/PA1		��ӦM1/M2
static void TIM4_Configuration(void);//TIM2��ͨ���ֱ����PWM3/PWM4,��Ӧ����ΪPB8/PB9		��ӦM3/M4
static void TIM3_Configuration(void);//TIM3��ͨ���ֱ����PWM1/PWM2/PWM3/PWM4,��Ӧ����ΪPA6/PA7/PB0/PB1		��ӦM5/M6/M7/M8

void motor_init(void);							 //��ˢ����ĳ�ʼ������
void motor_set_pwm_value(uint16_t *motor_head_address); //��ˢ����������
void motor_start_output(uint16_t m);			 //�˺���ÿ���ϵ��ʼ����Ҫ����һ�Σ����ڵ��ʶ����ʼ�ź�
void motor_Stop_output(void);				 //������ˢ������Ϊ��

#endif /* _MOTOR_H_ */

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
