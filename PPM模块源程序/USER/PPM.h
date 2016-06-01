#ifndef _PPM_H_
#define _PPM_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "include.h"

#pragma pack (1)
typedef struct _F4_to_PPM {     //F4发送给PPM的数据结构体
    uint8_t		head;				//数据包头，固定为0xDD
    s16 		Pit;        		//pitch轴数据
    s16 		Rol;				//roll轴数据
    s16 		Yaw;				//yaw轴数据
	uint16_t 	motor[4]; 			//4电机的值
    uint8_t 	check;				//校验和
} _F4_to_PPM;
#pragma pack ()
////////////////////////////////////////////////////////////////////
typedef union _F4_to_PPM_ {     //F4发送给PPM的数据联合体
    _F4_to_PPM	DATA;
    uint8_t		buffer[sizeof(_F4_to_PPM)];
} _F4_to_PPM_;
////////////////////////////////////////////////////////////////////
#pragma pack (1)
typedef struct _PPM_to_F4 {     //PPM发送给F4的数据结构体
    uint8_t		head;				//数据包头，固定为0xDD
	uint16_t 	mode_Altiude;		//模式_油门(模式为高4位)
    s8 		    Pit;        		//pitch轴数据
    s8 		    Rol;				//roll轴数据
    s8 		    Yaw;				//yaw轴数据
    uint8_t 	check;				//校验和
} _PPM_to_F4;
#pragma pack ()
////////////////////////////////////////////////////////////////////
typedef union _PPM_to_F4_ {     //PPM发送给F4的数据联合体
    _PPM_to_F4	DATA;
    uint8_t		buffer[sizeof(_PPM_to_F4)];
} _PPM_to_F4_;
////////////////////////////////////////////////////////////////////

void Get_PPM_Data(void); 		// 获取需要的数据
void Send_PPM_Data(void); 		// 发送监控数据
void USART1_Init(void);         // 初始化与PPM通讯的串口
void USART1_Send_Data(uint8_t Send_number, uint8_t *Send_data);  	//Uart1发送N个字节

#endif /* _PPM_H_ */
////////////////////////////////////////////////////////////////////
