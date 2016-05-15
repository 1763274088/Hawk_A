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


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//



///* L3GD20 struct */
//typedef struct
//{
//  uint8_t Power_Mode;                         /* Power-down/Sleep/Normal Mode */
//  uint8_t Output_DataRate;                    /* OUT data rate */
//  uint8_t Axes_Enable;                        /* Axes enable */
//  uint8_t Band_Width;                         /* Bandwidth selection */
//  uint8_t BlockData_Update;                   /* Block Data Update */
//  uint8_t Endianness;                         /* Endian Data selection */
//  uint8_t Full_Scale;                         /* Full Scale selection */
//}L3GD20_InitTypeDef;

//**********************************************************************************//
//**********************************************************************************//




//**********************************************************************************//
//Function definition area:All functions defined in the file
//**********************************************************************************//
/***************************** START Function definition ****************************/
void Moving_Average_Filter(s16 *buffer_1_Before,s16 *buffer_2_Before,float *buffer_1_after,float *buffer_2_after);


/****************************** END Function definition *****************************/
//**********************************************************************************//


#endif /* _FILTERING_ALGORITHM_H_*/

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
