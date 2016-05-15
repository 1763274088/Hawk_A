#include "motor.h"


///////////////////��ʱ�����ܳ�ʼ��

static void TIM2_Configuration(void)//TIM2��ͨ���ֱ����PWM1/PWM2,��Ӧ����ΪPA0/PA1
{
		GPIO_InitTypeDef GPIO_InitStructure; 
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		//����ʱ��Ҫ����ʵ�����ı䣬����TIM2����APB1��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;    
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�������������Ϊ3.3V      
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		TIM_TimeBaseStructure.TIM_Prescaler = 36;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = PWMPeriod;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

		TIM_OC1Init(TIM2, &TIM_OCInitStructure);
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	// 	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	// 	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
		
		TIM2->CCR1 = 0; 
		TIM2->CCR2 = 0;  
	// 	TIM1->CCR3 = 0;  
	// 	TIM1->CCR4 = 0; 

		TIM_Cmd(TIM2, ENABLE);
		TIM_CtrlPWMOutputs(TIM2, ENABLE);	
}

////////////////////////////

static void TIM4_Configuration(void)//TIM2��ͨ���ֱ����PWM3/PWM4,��Ӧ����ΪPB8/PB9
{
		GPIO_InitTypeDef GPIO_InitStructure; 
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		//����ʱ��Ҫ����ʵ�����ı䣬����TIM4����APB1��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;    
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
		GPIO_Init(GPIOB, &GPIO_InitStructure); 

		TIM_TimeBaseStructure.TIM_Prescaler = 36;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = PWMPeriod;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	// 	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	// 	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);
		
	// 	TIM4->CCR1 = 0;    
	// 	TIM4->CCR2 = 0; 
		TIM4->CCR3 = 0; 
		TIM4->CCR4 = 0; 

		TIM_Cmd(TIM4, ENABLE);
		TIM_CtrlPWMOutputs(TIM4, ENABLE);	
}
////////////////////////////

static void TIM3_Configuration(void)//TIM3��ͨ���ֱ����PWM1/PWM2/PWM3/PWM4,��Ӧ����ΪPA6/PA7/PB0/PB1
{
		GPIO_InitTypeDef GPIO_InitStructure; 
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		//����ʱ��Ҫ����ʵ�����ı䣬����TIM3����APB1��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;    
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
		GPIO_Init(GPIOA, &GPIO_InitStructure); 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;    
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		TIM_TimeBaseStructure.TIM_Prescaler = 36;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = PWMPeriod;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);
		
		TIM3->CCR1 = 0;    
		TIM3->CCR2 = 0; 
		TIM3->CCR3 = 0; 
		TIM3->CCR4 = 0; 

		TIM_Cmd(TIM3, ENABLE);
		TIM_CtrlPWMOutputs(TIM3, ENABLE);	
}


void motor_init(void)
{
		//��ʱ����ʼ���������壬����PWM
		TIM2_Configuration();
		TIM4_Configuration();
		TIM3_Configuration();
	
		//����PWM���ֵΪ��
		motor_Stop_output();
	
}


void motor_set_pwm_value(uint16_t *motor_head_address)
{
		unsigned char count;
		//���ֵ����޷���������ΪPWM_Upper_limit
		for(count=0;count<6;count++)
		{
		  if(*(motor_head_address+count) > PWM_Max-PWM_Upper_limit)  
					*(motor_head_address+count) = PWM_Max-PWM_Upper_limit;	
		}
		//����ֵ����޷���������ΪPWM_Lower_limit
		for(count=0;count<6;count++)
		{
		  if(*(motor_head_address+count) < PWM_Min-PWM_Lower_limit)  
					*(motor_head_address+count) = PWM_Min-PWM_Lower_limit;	
		}
		//��ͳһ���PWM��������ˢ���
		TIM2->CCR1 = *(motor_head_address++); 
		TIM2->CCR2 = *(motor_head_address++); 	
		TIM4->CCR3 = *(motor_head_address++); 
		TIM4->CCR4 = *(motor_head_address++); 	
		TIM3->CCR1 = *(motor_head_address++);     
		TIM3->CCR2 = * motor_head_address;
// �����ʱ����õ�		
// 		TIM3->CCR2 = *(motor_head_address++); 
// 		TIM3->CCR3 = *(motor_head_address++); 
// 		TIM3->CCR4 = * motor_head_address;		

}

//�˺���ÿ���ϵ��ʼ����Ҫ����һ�Σ����ڵ��ʶ����ʼ�ź�
void motor_start_output(uint16_t m)
{
		TIM2->CCR1 = m; 
		TIM2->CCR2 = m; 	
		TIM4->CCR3 = m; 
		TIM4->CCR4 = m; 	
		TIM3->CCR1 = m;    
		TIM3->CCR2 = m; 
// 	  �����ʱ����õ�	
// 		TIM3->CCR3 = 2000; 
// 		TIM3->CCR4 = 2000; 
}

void motor_Stop_output(void)
{
		TIM2->CCR1 = 0; 
		TIM2->CCR2 = 0; 	
		TIM4->CCR3 = 0; 
		TIM4->CCR4 = 0; 	
		TIM3->CCR1 = 0;    
		TIM3->CCR2 = 0; 
// 	  �����ʱ����õ�	
// 		TIM3->CCR3 = 0; 
// 		TIM3->CCR4 = 0; 
}

