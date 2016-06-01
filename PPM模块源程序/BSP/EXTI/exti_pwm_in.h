
#ifndef		__EXTI_PWM_IN_H_
#define		__EXTI_PWM_IN_H_

#include "config.h"

#define waring_out	900

void EXTI_Configuration(void);	//外部中断初始化
void TIM2_Configuration(void);	//TIM2定时器初始化
void TIM3_Configuration(void);
void TIM4_Configuration(void);

extern void waring_ouput(void);//紧急输出
extern void PWMIN_Stop(u8 data);//禁用无效端口

#endif
