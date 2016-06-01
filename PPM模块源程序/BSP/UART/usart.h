#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"
#include <stdio.h>
#include "stdint.h"


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//
//监控数据结构体
#pragma pack (1)
typedef struct _DATA_ {
    uint8_t		head;				//数据包头，固定为0xDD
	uint16_t 	mode_Altiude;		//模式_油门(模式为高4位)
    s16 		Pitch;        		//pitch轴数据
    s16 		Roll;				//roll轴数据
    s16 		Yaw;				//yaw轴数据
	uint16_t 	Control_value[4];	//电机控制值数据包
    uint8_t 	check;				//校验和
} _DATA_;
#pragma pack ()
//监控数据联合体
typedef union _STATE_DATA {
    _DATA_	control_target;
    uint8_t	buffer[sizeof(_DATA_)];
} _STATE_DATA_;

//**********************************************************************************//
//**********************************************************************************//
void usart1_config(void);

void USART1_IRQHandler(void);

void usart1_send_data(unsigned char Send_number, unsigned char *Send_data);

#endif
