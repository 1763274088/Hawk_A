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
//目标控制结构体
typedef struct	_target
{
			uint8_t		 head;		 //数据包头，固定为0xAA
			uint8_t		 head_1;	 //数据包头，固定为0xBB
			uint8_t		 mode;		 //模式控制	
      uint16_t	 Altiude;	 //油门控制值	
      float 		 Pitch;    //pitch轴控制角度
	    float 		 Roll;  	 //roll轴控制角度
	    float 		 Yaw;   	 //yaw轴控制角度
			uint8_t		 check;		 //校验和，所有数值的累加和
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
//函数定义集合
*******************************************************************************/
int16_t deathzoom(int16_t x,int16_t zoom);
void remote_process(void);

#endif /* _RC_PROCESS_H_ */

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
