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
/*����ʽPID�㷨���ӿڲ����ṹ����*/
typedef struct 
{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//pitch��Ŀ��ƽṹ��
		float Pitch_kp;     								//����ϵ��
		float Pitch_ki;     								//����ϵ��
		float Pitch_kd;     								//΢��ϵ��
		float Pitch_ErrILim;								//����������	 
		float Pitch_ErrNow;									//��ǰ�����
		float Pitch_Errpast;								//��ʷֵ	
		float Pitch_CtrOut;									//��������� 
		float Pitch_ErrP_out;								//�������
		float Pitch_ErrI_out;								//�������
		float Pitch_ErrD_out;								//΢�����
		float	Pitch_increment;							//��������
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~// 

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//Roll��Ŀ��ƽṹ��
		float Roll_kp;     									//����ϵ��
		float Roll_ki;     									//����ϵ��
		float Roll_kd;     									//΢��ϵ��
		float Roll_ErrILim;									//����������	 
		float Roll_ErrNow;									//��ǰ�����
		float Roll_Errpast;									//��ʷֵ	
		float Roll_CtrOut;									//��������� 
		float Roll_ErrP_out;								//�������
		float Roll_ErrI_out;								//�������
		float Roll_ErrD_out;								//΢�����
		float	Roll_increment;								//��������
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~// 

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//Yaw��Ŀ��ƽṹ��
		float Yaw_kp;     									//����ϵ��
		float Yaw_ki;     									//����ϵ��
		float Yaw_kd;     									//΢��ϵ��
		float Yaw_ErrILim;									//����������	 
		float Yaw_ErrNow;										//��ǰ�����
		float Yaw_Errpast;									//��ʷֵ	
		float Yaw_CtrOut;										//��������� 
		float Yaw_ErrP_out;									//�������
		float Yaw_ErrI_out;									//�������
		float Yaw_ErrD_out;									//΢�����
		float	Yaw_increment;								//��������
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~// 
}PID_InitTypeDef;

/*������Ŀ������ṹ������*/
typedef struct 
{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//Ŀ������ṹ�嶨��
		uint16_t target_Altitude;						//Ŀ��߶�
    float 	target_Pitch; 							//Ŀ�긩��   
	  float 	target_Roll;								//Ŀ����  
	  float 	target_Yaw;									//Ŀ��ƫ��   

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~// 
}target_InitTypeDef;

//����һ���ڻ��ṹ��
extern PID_InitTypeDef	Inside_link;
//����һ���⻷�ṹ��
extern PID_InitTypeDef	Outside_link;




//���Ƶ�����
#pragma pack (1)
typedef struct _CONTROL_Receive_Data_
{
    uint8_t     head;      										//��ͷ���̶�ֵΪ0xCC
    uint16_t    Control_value[6];      				//�������ֵ���ݰ�
		uint8_t			check;												//У��ͣ��ۼӺ�
	
}_CONTROL_Receive_Data;
#pragma pack () 

//������С��ģʽ
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
