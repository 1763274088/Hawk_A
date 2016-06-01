/********************************************************************************
  * @file    Filtering_Algorithm.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    01-03-2016
  * @brief   Header for Filtering_Algorithm.c module
*******************************************************************************/ 
#ifndef _FILTERING_ALGORITHM_H_
#define _FILTERING_ALGORITHM_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"

/***************************** START Function definition ****************************/
void Moving_Average_Filter(s16 *buffer_1_Before,s16 *buffer_2_Before,float *buffer_1_after,float *buffer_2_after);
void Data_KalmanFilter(float *cov_history, float *Data_history, s16 *Data_now, float *Data_after);

/****************************** END Function definition *****************************/
//**********************************************************************************//


#endif /* _FILTERING_ALGORITHM_H_*/

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
