#include "tim.h"
#include "motor.h"
#include "communication.h"
#include "led.h"
#include "futaba_sbus.h"


//��������F4���Ƶ����־λ
extern uint8_t motor_receive_mark;

//������ƽṹ��
_MOTOR_CONTROL motor_rc;

extern uint8_t LED_give_table;

extern uint8_t usart2_rx_buffer[14];

extern int mode ;

//���ö�ʱ��1Ϊ1ms�ж�ģʽ
void TIM1_Configuration(void)  
{  
    NVIC_InitTypeDef NVIC_InitStructure;  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
  
    TIM_DeInit(TIM1);                                          
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);       
    TIM_TimeBaseStructure.TIM_Period = 100;                         
    TIM_TimeBaseStructure.TIM_Prescaler = 720-1;                     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;             
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  
      

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);                 
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;         
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);   
		TIM_Cmd(TIM1, ENABLE);   	
}  


void TIM1_UP_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) 
	{	
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);		
		time_1_task_ms++;
		time_2_task_ms++;
		time_3_task_ms++;
		
		if(time_1_task_ms==2)
		{
			time_1_task_ms=0;
			
			if(motor_receive_mark==1)
			{
			//�Ѵ���֮�����ˢ���PWMֵ���Ƶ��
			motor_set_pwm_value(motor_rc.info.Control_value);
			//������ձ�־λ
			motor_receive_mark=0;
			}
			if(LED_give_table==1)
			{
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);						 //PB.13 �����
			}
			
			if(mode==1)
			{
			GPIO_ResetBits(GPIOB,GPIO_Pin_15);						 //PB.15 �����
			}
				
		}
		
	}
	
}





