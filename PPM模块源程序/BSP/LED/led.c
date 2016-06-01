#include "led.h"
#include "delay.h"

void LED_Init(void)
{
	/******************************************************************/	
	GPIO_InitTypeDef Led_GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	Led_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	Led_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	Led_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &Led_GPIO_InitStructure);						//LED∂Àø⁄≈‰÷√PA0
	/*************************************************************/	
	 GPIO_SetBits(GPIOA,GPIO_Pin_0);	 // turn off all led
}

void LED_Power(void)
{
	LED(ON);
	delay_ms(500);
	LED(OFF);
	delay_ms(500);
}

