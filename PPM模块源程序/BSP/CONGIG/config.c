
#include "config.h"

/******************************************************************************************
*	��������:void NVIC_Configuration(void)
*	�������ܣ�ϵͳ�жϳ�ʼ��
*	��������
*	����ֵ����
*	˵����
******************************************************************************************/
void NVIC_Configuration(void){
	
	NVIC_InitTypeDef NVIC_InitStructure;   
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 

  /* Enable the USART1 Interrupt */                             
  NVIC_InitStructure.NVIC_IRQChannel =   USART1_IRQn;   //ͨ������Ϊ����1�жϣ��ʺ���Ӧѡ���ڡ�void USART1_IRQHandler(void)�����жϣ�    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //�ж�ռ�ȵȼ�0   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //�ж���Ӧ���ȼ�0   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //���ж�   
	NVIC_Init(&NVIC_InitStructure);	
	
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	//-------------------------------------------------------------------------
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;           //EXTI��NVIC���� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //-------------------------------------------------------------------------
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;         //EXTI��NVIC���� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	 //-------------------------------------------------------------------------
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;       //EXTI��NVIC���� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}

/******************************************************************************************
*	��������:void RCC_Configuration(void)
*	�������ܣ�ϵͳʱ�ӳ�ʼ��
*	��������
*	����ֵ����
*	˵����
******************************************************************************************/
void RCC_Configuration(void){
	
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  RCC_PCLK2Config(RCC_HCLK_Div1);
  RCC_PCLK1Config(RCC_HCLK_Div2);
  FLASH_SetLatency(FLASH_Latency_2);
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	 //9��Ƶ 9x8MHz=72MHzʱ��
  RCC_PLLCmd(ENABLE); 
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){
       }
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
	while(RCC_GetSYSCLKSource() != 0x08){ 
       }
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 		   //USART2ʱ��ʹ��
}
