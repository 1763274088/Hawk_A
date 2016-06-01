#include "TIM_PWM_IN.h"
#include "PPM.h"

//任务管理定时器，定时周期为1ms
void TIM4_Int_Init(u16 arr, u16 psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			//使能TIM4时钟

    TIM_TimeBaseInitStructure.TIM_Period = arr - 1;					//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;				//定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);		        //初始化TIM4

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);				        //允许定时器3更新中断
    TIM_Cmd(TIM4, ENABLE);									        //使能定时器3

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;					//定时器3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;    //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

s8 	time_5_ms = 0;
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //溢出中断
    {
        time_5_ms++;
        //第1毫秒执行任务
        if (time_5_ms == 5) {
            //Send_PPM_Data();
        }
    }
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除中断标志位
}
