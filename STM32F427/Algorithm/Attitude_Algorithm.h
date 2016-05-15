/********************************************************************************
  * @file    Attitude_Algorithm.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    22-03-2016
  * @brief   Header for Attitude_Algorithm.c module
*******************************************************************************/ 
#ifndef _ATTITUDE_ALGORITHM_H_
#define _ATTITUDE_ALGORITHM_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"
#include "Math_Algorithm.h"

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
void AHRS_Update(float Acc_X,float Acc_Y, float Acc_Z, float Gyr_X, float Gyr_Y, float Gyr_Z, float Half_time,float *Rol,float *Pit,float *Yaw );

/****************************** END Function definition *****************************/
//**********************************************************************************//


#endif /* _FILTERING_ALGORITHM_H_*/

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
