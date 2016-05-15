/********************************************************************************
  * @file    Control_Algorithm.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    1-04-2016
  * @brief   Header for Control_Algorithm.c module
*******************************************************************************/ 
#ifndef _CONTROL_ALGORITHM_H_
#define _CONTROL_ALGORITHM_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"
#include "Math_Algorithm.h"
#include "mpu6000.h"
//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//
/*绝对式PID算法，接口参数结构类型*/
typedef struct 
{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//pitch轴的控制结构体
		float Pitch_kp;     								//比例系数
		float Pitch_ki;     								//积分系数
		float Pitch_kd;     								//微分系数
		float Pitch_ErrILim;								//误差积分上限	 
		float Pitch_ErrNow;									//当前的误差
		float Pitch_Errpast;								//历史值	
		float Pitch_CtrOut;									//控制量输出 
		float Pitch_ErrP_out;								//比例输出
		float Pitch_ErrI_out;								//积分输出
		float Pitch_ErrD_out;								//微分输出
		float	Pitch_increment;							//误差的增量
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~// 

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//Roll轴的控制结构体
		float Roll_kp;     									//比例系数
		float Roll_ki;     									//积分系数
		float Roll_kd;     									//微分系数
		float Roll_ErrILim;									//误差积分上限	 
		float Roll_ErrNow;									//当前的误差
		float Roll_Errpast;									//历史值	
		float Roll_CtrOut;									//控制量输出 
		float Roll_ErrP_out;								//比例输出
		float Roll_ErrI_out;								//积分输出
		float Roll_ErrD_out;								//微分输出
		float	Roll_increment;								//误差的增量
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~// 

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//Yaw轴的控制结构体
		float Yaw_kp;     									//比例系数
		float Yaw_ki;     									//积分系数
		float Yaw_kd;     									//微分系数
		float Yaw_ErrILim;									//误差积分上限	 
		float Yaw_ErrNow;										//当前的误差
		float Yaw_Errpast;									//历史值	
		float Yaw_CtrOut;										//控制量输出 
		float Yaw_ErrP_out;									//比例输出
		float Yaw_ErrI_out;									//积分输出
		float Yaw_ErrD_out;									//微分输出
		float	Yaw_increment;								//误差的增量
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~// 
}PID_InitTypeDef;

/*多旋翼目标输出结构体类型*/
typedef struct 
{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//目标输出结构体定义
		uint16_t target_Altitude;						//目标高度
    float 	target_Pitch; 							//目标俯仰   
	  float 	target_Roll;								//目标横滚  
	  float 	target_Yaw;									//目标偏航   

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~// 
}target_InitTypeDef;

//定义一个内环结构体
extern PID_InitTypeDef	Inside_link;
//定义一个外环结构体
extern PID_InitTypeDef	Outside_link;




//控制电机输出
#pragma pack (1)
typedef struct _CONTROL_Receive_Data_
{
    uint8_t     head;      										//包头：固定值为0xCC
    uint16_t    Control_value[6];      				//电机控制值数据包
		uint8_t			check;												//校验和：累加和
	
}_CONTROL_Receive_Data;
#pragma pack () 

//这里是小端模式
typedef   union _MOTOR_CONTROL_
{
    _CONTROL_Receive_Data 	info;
    uint8_t          				TX_buffer[sizeof(_CONTROL_Receive_Data)];
}_MOTOR_CONTROL;

extern  _MOTOR_CONTROL motor_tc;


extern float ROLL,YAW,PIT,HALF_T;

//**********************************************************************************//
//**********************************************************************************//




//**********************************************************************************//
//Function definition area:All functions defined in the file
//**********************************************************************************//
/***************************** START Function definition ****************************/
void Calculate_target_amount(void);
void Angle_Control_loop(void);
void Angle_speed_control_loop(void);
void Motor_Control(void);
void Motor_Send_task(void);
void Motor_Send_stop_task(void);
/****************************** END Function definition *****************************/
//**********************************************************************************//


#endif /* _FILTERING_ALGORITHM_H_*/

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
