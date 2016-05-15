#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "motor.h"
#include "futaba_sbus.h"
#include "communication.h"
#include "tim.h"
#include "rc_process.h"

//����ִ��ʱ���־λ
uint32_t time_1_task_ms=0;
uint32_t time_2_task_ms=0;
uint32_t time_3_task_ms=0;

//ң��������
Target_Send Target;




//SBUS�źŴ�������
unsigned char sbus_rx_buffer[25];



int mode ;

uint32_t kkkk=0;


int main(void)
{	
	delay_init();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
	USART3_Config();
	USART2_Config();

	LED_Init();
	
	//��ʼ��PWM���
	motor_init();
	//����������źţ��Ա���ȷ����ʣ�µĲ���
	motor_start_output(4000);
	delay_ms(1000);
	motor_start_output(2000);
	delay_ms(2000);
	delay_ms(3000);	

	//�򿪶�ʱ���ж�
	TIM1_Configuration();

		
	while(1)
	{

		
		//==========================����ִ�д���==============================//
		
		//��ң���������ݷ��͸�F427
		if(time_2_task_ms==5)
		{
			time_2_task_ms=0;
			//��ң������ԭʼֵ������ת��ΪҪ���Ƶķɻ��ĽǶ�ֵ
			if(mode==1)
			{
			remote_process();
			}
			else
			{

			}
			
		}
		//====================================================================//
	}
	
}
