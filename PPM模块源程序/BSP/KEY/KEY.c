#include "stm32f10x_gpio.h"
#include "key.h"
#include "led.h"
#include "delay.h"

/*
* 函数名： Delay
* 描述  ：不精确的延时
* 输入  ：延时参数，
* 输出  ：无
*/
void Delay(__IO u32 nCount) {
    for (; nCount != 0; nCount--);
}

void KEY_Init(void) {
    GPIO_InitTypeDef KEY_GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //使能GPIOB时钟

    KEY_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
    KEY_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    KEY_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &KEY_GPIO_InitStructure);

    KEY_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    KEY_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    KEY_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &KEY_GPIO_InitStructure);
}

/*
* 函数名：Key_Scan
* 描述  ：检测是否有按键按下
* 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
* GPIO_Pin：待读取的端口位
* 输出  ：KEY_OFF(没按下按键)、1（按下按键）
*/
uint8_t Key_Scan(void) {
    /*检测是否有按键按下 */
    if (GPIO_ReadInputDataBit(GPIOA, KEY1) == 1) {
        delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOA, KEY1) == 1) {
			LED(ON);
            while (GPIO_ReadInputDataBit(GPIOA, KEY1) == 1);
			LED(OFF);
            return KEY_UP;
        }
    }
    else if (GPIO_ReadInputDataBit(GPIOB, KEY2) == 1) {
        delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOB, KEY2) == 1) {
			LED(ON);
            while (GPIO_ReadInputDataBit(GPIOB, KEY2) == 1);
			LED(OFF);
            return KEY_RIGHT;
        }
    }
    else if (GPIO_ReadInputDataBit(GPIOB, KEY3) == 1) {
        delay_ms(8000);
        if (GPIO_ReadInputDataBit(GPIOB, KEY3) == 1) {
			LED(ON);
            while (GPIO_ReadInputDataBit(GPIOB, KEY3) == 1);
			LED(OFF);
            return KEY_ENTER;
        }
    }
    else if (GPIO_ReadInputDataBit(GPIOA, KEY4) == 1) {
        delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOA, KEY4) == 1) {
			LED(ON);
            while (GPIO_ReadInputDataBit(GPIOA, KEY4) == 1);
			LED(OFF);
            return KEY_LEFT;
        }
    }
    else if (GPIO_ReadInputDataBit(GPIOA, KEY5) == 1) {
        delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOA, KEY5) == 1) {
			LED(ON);
            while (GPIO_ReadInputDataBit(GPIOA, KEY5) == 1);
			LED(OFF);
            return KEY_DOWN;
        }
    }
    else 
		return KEY_OFF;
	return 	0;
}
