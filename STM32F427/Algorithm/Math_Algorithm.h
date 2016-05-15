/********************************************************************************
  * @file    Math_Algorithm.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    22-03-2016
  * @brief   Header for Math_Algorithm.c module
*******************************************************************************/ 
#ifndef _MATH_ALGORITHM_H_
#define _MATH_ALGORITHM_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//

#define square( number )     (((float)number)*((float)number))			//求平方
#define absolute(x) 				 ( (x)>0?(x):-(x) )											//求绝对值
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )


#define REAL   float
#define TAN_MAP_RES     0.003921569f     /* (smallest non-zero value in table) */
#define RAD_PER_DEG     0.017453293f
#define TAN_MAP_SIZE    256
#define MY_PPPIII   3.14159f
#define MY_PPPIII_HALF   1.570796f

//**********************************************************************************//
//**********************************************************************************//




//**********************************************************************************//
//Function definition area:All functions defined in the file
//**********************************************************************************//
/***************************** START Function definition ****************************/
float Rsqrt(float number);
float Rsqrt_reciprocal(float number);
float VariableParameter(float error);


float my_abs(float f);
REAL fast_atan2(REAL y, REAL x);
float my_pow(float a);
float my_sqrt(float number);
double mx_sin(double rad);
double my_sin(double rad);
float my_cos(double rad);
float my_deathzoom(float x,float zoom);
float To_180_degrees(float x);

/****************************** END Function definition *****************************/
//**********************************************************************************//


#endif /* _FILTERING_ALGORITHM_H_*/

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
