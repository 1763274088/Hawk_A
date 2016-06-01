#include "stm32f10x_gpio.h"
#include "key.h"
#include "led.h"
#include "delay.h"

//配置安全按钮PB5
void KEY_Init(void) {
    GPIO_InitTypeDef KEY_GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //使能GPIOB时钟

    KEY_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 			// PB5
    KEY_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    KEY_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		// 输出模式,内部电阻下拉
    GPIO_Init(GPIOB, &KEY_GPIO_InitStructure);
}

/*
* 函数名：Key_Scan
* 描述  ：检测是否有按键按下
* 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
* GPIO_Pin：待读取的端口位
* 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
*/
uint8_t Key_Scan(void) {
	uint16_t count;
    /*检测是否有按键按下 */
    if (GPIO_ReadInputDataBit(GPIOB, SAFETY) == SAFE) {
        delay_ms(20); 		// 20ms去抖
        if (GPIO_ReadInputDataBit(GPIOB, SAFETY) == SAFE) {
            while (GPIO_ReadInputDataBit(GPIOB, SAFETY) == SAFE) {
				if(count == 10000) 	// 按键很长时间
				{
					
				}
				else 				// 
					count++;
			}// 检测按键时间长短
            return SAFE;
		}
	}
    else 
		return UNSAFE;
	return UNSAFE;
}
