
#include "config.h"		//Ó²¼þÅäÖÃÎÄ¼þ
#include "exti_pwm_in.h"
#include "ppm.h"


u16 pwm_start0,pwm_end0,pwm_start1,pwm_end1,pwm_start2,pwm_end2,pwm_start3,pwm_end3;
u16 pwm_start4,pwm_end4,pwm_start5,pwm_end5,pwm_start6,pwm_end6,pwm_start7,pwm_end7;

u32 time1=PPM_HEART ,time2=PPM_P;

u32 PPM[18]={12000,400,900,400,1000,400,1100,400,1200,400,1300,400,1400,400,1500,400,1600,400}, ppm_c=0;

void  PPM_OUT_CH (u8 ch,u16 pwm)
{

    if((pwm<PPM_NO_Ch )||(pwm>2500)){
			PPM[ch]=PPM_Ch-PPM_P;
			}else {
			PPM[ch]=pwm-PPM_P;
		}
}

void PPM_OUTPUT(void)
{
  PPM_OUT_CH(2,pwm_end0);
	PPM_OUT_CH(4,pwm_end1);
	PPM_OUT_CH(6,pwm_end2);
	PPM_OUT_CH(8,pwm_end3);
	PPM_OUT_CH(10,pwm_end4);
	PPM_OUT_CH(12,pwm_end5);
	PPM_OUT_CH(14,pwm_end6);
	PPM_OUT_CH(16,pwm_end7);
}

void EXTI4_IRQHandler(void)
{	
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){ 
	  EXTI_ClearITPendingBit(EXTI_Line4);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)){
			 pwm_start0=TIM_GetCounter(TIM2);
			}
		else{
				pwm_end0=TIM_GetCounter(TIM2)-pwm_start0;
			}
	}
}

void EXTI9_5_IRQHandler(void)
{	
	if(EXTI_GetITStatus(EXTI_Line5) != RESET){ 
	  EXTI_ClearITPendingBit(EXTI_Line5);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)){
			 pwm_start1=TIM_GetCounter(TIM2);
			}
		else{
				pwm_end1=TIM_GetCounter(TIM2)-pwm_start1;
			}
	}
	
	if(EXTI_GetITStatus(EXTI_Line8) != RESET){ 
	  EXTI_ClearITPendingBit(EXTI_Line8);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)){
			 pwm_start7=TIM_GetCounter(TIM2);
			}
		else{			
				pwm_end7=TIM_GetCounter(TIM2)-pwm_start7;			
			}
	}			
	
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET){ 
	  EXTI_ClearITPendingBit(EXTI_Line10);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)){
			 pwm_start2=TIM_GetCounter(TIM2);
			}
		else{
				pwm_end2=TIM_GetCounter(TIM2)-pwm_start2;
			}
	}
	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET){ 
	  EXTI_ClearITPendingBit(EXTI_Line11);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)){
			 pwm_start3=TIM_GetCounter(TIM2);
			}
		else{
				pwm_end3=TIM_GetCounter(TIM2)-pwm_start3;
			}
	}
	
	if(EXTI_GetITStatus(EXTI_Line12) != RESET){ 
	  EXTI_ClearITPendingBit(EXTI_Line12);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
			 pwm_start4=TIM_GetCounter(TIM2);
			}
		else{
				pwm_end4=TIM_GetCounter(TIM2)-pwm_start4;
			}
	}
	
	if(EXTI_GetITStatus(EXTI_Line13) != RESET){ 
	  EXTI_ClearITPendingBit(EXTI_Line13);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)){
			 pwm_start5=TIM_GetCounter(TIM2);
			}
		else{
				pwm_end5=TIM_GetCounter(TIM2)-pwm_start5;
			}
	}	

	
	if(EXTI_GetITStatus(EXTI_Line14) != RESET){ 
	  EXTI_ClearITPendingBit(EXTI_Line14);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)){
			 pwm_start6=TIM_GetCounter(TIM2);
			}
		else{
				pwm_end6=TIM_GetCounter(TIM2)-pwm_start6;
			}
	}
}

void TIM3_IRQHandler(void)
{
	u8 ReadValue;
       if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {

              TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
              ReadValue = PPM_W;
				 	
		          if(ppm_c>17)ppm_c=0;
              TIM3->ARR=PPM[ppm_c]-1;
							ppm_c++;
				 
              if(ReadValue == 0)  {
                 PPM_1;
              }    
              else{
                  PPM_0;    
              }
       }
}



