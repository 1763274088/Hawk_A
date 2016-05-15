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
//全部统一PWM的配置，操作简便
#define PWMPeriod      				 40000			//PWM的周期			50 HZ   
#define PWM_TIM_Prescaler      36					//PWM的系统时钟分频系数
#define PWM_TIM_ClockDivision  0 					//PWM的时钟分频系数
#define PWM_Max								 4000				//占空比为10%
#define PWM_Min								 2000				//占空比为5%
#define PWM_Upper_limit			   1200				//PWM上限余量		目前预留10%的余量
#define PWM_Lower_limit				 0					//PWM下限余量
//******************************************************//

//******************************************************//
/*****	PWM	Pin	definition	*****/
//M1到M8的对应管脚分别为：PA0/PA1/PB8/PB9/PA6/PA7/PB0/PB1
//分别对应的定时器为：TIM2_CH1/TIM2_CH2/TIM4_CH3/TIM4_CH4
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
//PWM函数定义集合
*******************************************************************************/
static void TIM2_Configuration(void);//TIM2两通道分别输出PWM1/PWM2,对应引脚为PA0/PA1		对应M1/M2
static void TIM4_Configuration(void);//TIM2两通道分别输出PWM3/PWM4,对应引脚为PB8/PB9		对应M3/M4
static void TIM3_Configuration(void);//TIM3四通道分别输出PWM1/PWM2/PWM3/PWM4,对应引脚为PA6/PA7/PB0/PB1		对应M5/M6/M7/M8

void motor_init(void);							 //无刷电机的初始化函数
void motor_set_pwm_value(uint16_t *motor_head_address); //无刷电机输出函数
void motor_start_output(uint16_t m);			 //此函数每次上电初始化都要调用一次，用于电调识别起始信号
void motor_Stop_output(void);				 //设置无刷电机输出为零

#endif /* _MOTOR_H_ */

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
