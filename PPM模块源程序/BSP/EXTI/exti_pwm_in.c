
/*****************************************************************************************
				输入				 引脚
				PWM1---------PA4
				PWM2---------PA5
				PWM3---------PB10
				PWM4---------PB11
				PWM5---------PB12
				PWM6---------PB13
				PWM7---------PB14
				PWM8---------PA8
				
				PA12----------PPM
*****************************************************************************************/

#include "exti_pwm_in.h"

vu16 CCR1_Val	=	waring_out;
vu16 CCR2_Val	=	waring_out;
vu16 CCR3_Val	=	waring_out;
vu16 CCR4_Val	=	waring_out;

void EXTI_Configuration(void){
	
		EXTI_InitTypeDef EXTI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;	
	/*--------------开启时钟---------------*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能GPIOB复用时钟	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOB时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能GPIOB复用时钟
	/* ---------- 配置外部中断IO,3-7路PWM输入-------   */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11 |
																	GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //浮空输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);  
		/* ---------- 配置外部中断IO,1-2路 8 PWM输入-------   */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ---------- 8路PWM输入时钟线-------   */
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);   //pwm1
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);		//pwm2
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);	//pwm3
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);	//pwm4
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);	//pwm5
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);	//pwm6
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);	//pwm7
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);		//pwm8
		/* ---------- 配置外部中断IO,1路PPM输入-------   */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource9);	//ppm
	
		EXTI_DeInit();		//将EXTI寄存器重置为缺省值

		EXTI_InitStructure.EXTI_Line = EXTI_Line4; //中断线
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
		EXTI_Init(&EXTI_InitStructure);	//初始化
		EXTI_ClearITPendingBit(EXTI_Line4);	//清除下标志位
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line5; //中断线
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
		EXTI_Init(&EXTI_InitStructure);	//初始化
		EXTI_ClearITPendingBit(EXTI_Line5);	//清除下标志位
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line10; //中断线
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
		EXTI_Init(&EXTI_InitStructure);	//初始化
		EXTI_ClearITPendingBit(EXTI_Line10);	//清除下标志位
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line11; //中断线
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
		EXTI_Init(&EXTI_InitStructure);	//初始化
		EXTI_ClearITPendingBit(EXTI_Line11);	//清除下标志位

		EXTI_InitStructure.EXTI_Line = EXTI_Line12; //中断线
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
		EXTI_Init(&EXTI_InitStructure);	//初始化
		EXTI_ClearITPendingBit(EXTI_Line12);	//清除下标志位
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line13; //中断线
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
		EXTI_Init(&EXTI_InitStructure);	//初始化
		EXTI_ClearITPendingBit(EXTI_Line13);	//清除下标志位
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line14; //中断线
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
		EXTI_Init(&EXTI_InitStructure);	//初始化
		EXTI_ClearITPendingBit(EXTI_Line14);	//清除下标志位
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line8; //中断线
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //双边沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
		EXTI_Init(&EXTI_InitStructure);	//初始化
		EXTI_ClearITPendingBit(EXTI_Line8);	//清除下标志位
		
	
}

void TIM2_Configuration(void){
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 				//TIM2时钟使能
	
		TIM_DeInit(TIM2);
		TIM_TimeBaseStructure.TIM_Period = 0xffff;        					// 范围
		TIM_TimeBaseStructure.TIM_Prescaler = 71;    								// 分频71+1 对应精度为1us
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  		//时钟分频
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //想上计数
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);												//清除TIM2溢出中断标志
	  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
		TIM_Cmd(TIM2, ENABLE);  
	    
}

void TIM3_Configuration(void){
		
       TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//	TIM_OCInitTypeDef TIM_OCInitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//TIM2时钟使能
       TIM_DeInit(TIM3);
       TIM_InternalClockConfig(TIM3);

       TIM_TimeBaseStructure.TIM_Prescaler = 71;
       TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
       TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

       TIM_ClearFlag(TIM3, TIM_FLAG_Update);

       TIM_ARRPreloadConfig(TIM3, DISABLE);

       TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	     TIM_Cmd(TIM3,ENABLE);
}

void TIM4_Configuration(void){
		
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 		   //TIM4时钟使能
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseStructure.TIM_Period = 0x09C4;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val; 
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val; 
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val; 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val; 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4,ENABLE);
	
}
