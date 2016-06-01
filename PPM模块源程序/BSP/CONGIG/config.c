
#include "config.h"

/******************************************************************************************
*	函数名称:void NVIC_Configuration(void)
*	函数功能：系统中断初始化
*	参数：无
*	返回值：无
*	说明：
******************************************************************************************/
void NVIC_Configuration(void){
	
	NVIC_InitTypeDef NVIC_InitStructure;   
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 

  /* Enable the USART1 Interrupt */                             
  NVIC_InitStructure.NVIC_IRQChannel =   USART1_IRQn;   //通道设置为串口1中断（故后面应选择在“void USART1_IRQHandler(void)”开中断）    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //中断占先等级0   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //中断响应优先级0   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //打开中断   
	NVIC_Init(&NVIC_InitStructure);	
	
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	//-------------------------------------------------------------------------
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;           //EXTI的NVIC配置 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //-------------------------------------------------------------------------
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;         //EXTI的NVIC配置 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	 //-------------------------------------------------------------------------
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;       //EXTI的NVIC配置 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}

/******************************************************************************************
*	函数名称:void RCC_Configuration(void)
*	函数功能：系统时钟初始化
*	参数：无
*	返回值：无
*	说明：
******************************************************************************************/
void RCC_Configuration(void){
	
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  RCC_PCLK2Config(RCC_HCLK_Div1);
  RCC_PCLK1Config(RCC_HCLK_Div2);
  FLASH_SetLatency(FLASH_Latency_2);
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	 //9倍频 9x8MHz=72MHz时钟
  RCC_PLLCmd(ENABLE); 
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){
       }
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
	while(RCC_GetSYSCLKSource() != 0x08){ 
       }
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 		   //USART2时钟使能
}
