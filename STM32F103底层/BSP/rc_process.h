/********************************************************************************
  * @file    rc_process.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    8-4-2016
  * @brief   Header for rc_process.c module
*******************************************************************************/ 
#ifndef _RC_PROCESS_H_
#define _RC_PROCESS_H_
#include "stm32f10x.h"
#include "stdint.h"

//******************************************************//
//******************************************************//
#pragma pack (1)
//Ŀ����ƽṹ��
typedef struct	_target
{
			uint8_t		 head;		 //���ݰ�ͷ���̶�Ϊ0xAA
			uint8_t		 head_1;	 //���ݰ�ͷ���̶�Ϊ0xBB
			uint8_t		 mode;		 //ģʽ����	
      uint16_t	 Altiude;	 //���ſ���ֵ	
      float 		 Pitch;    //pitch����ƽǶ�
	    float 		 Roll;  	 //roll����ƽǶ�
	    float 		 Yaw;   	 //yaw����ƽǶ�
			uint8_t		 check;		 //У��ͣ�������ֵ���ۼӺ�
}_target_result;
#pragma pack () 

//******************************************************//
typedef   union _TARGET_SEND_
{
    _target_result control_target;
    uint8_t        Send_buffer[sizeof(_target_result)];
}Target_Send;

extern  Target_Send Target;

/*******************************************************************************
//�������弯��
*******************************************************************************/
int16_t deathzoom(int16_t x,int16_t zoom);
void remote_process(void);

#endif /* _RC_PROCESS_H_ */

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
