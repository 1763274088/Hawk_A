#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"

#define KEY1        GPIO_Pin_7     
#define KEY2        GPIO_Pin_1    
#define KEY3        GPIO_Pin_0      
#define KEY4        GPIO_Pin_1      
#define KEY5        GPIO_Pin_6       

#define KEY_OFF		0
#define KEY_UP		1
#define KEY_DOWN 	2
#define KEY_LEFT	3
#define KEY_RIGHT	4
#define KEY_ENTER	5

void KEY_Init(void);
uint8_t Key_Scan(void);

#endif
