#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"
#include "stdint.h"

#define SAFETY		GPIO_Pin_5
   
 /*******
 *按键按下标置
 KEY_ON 0
 KEY_OFF 1
 ********/
 
#define SAFE		1
#define UNSAFE		0

void KEY_Init(void);
uint8_t Key_Scan(void);

#endif
