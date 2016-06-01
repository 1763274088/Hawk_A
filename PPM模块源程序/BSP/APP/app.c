#include "app.h"

void IAC_Init(void) {
    delay_init(72);         //延时函数初始化
    LED_Init();             //led初始化
    OLED_Init();            //oled初始化
    KEY_Init();             //按键初始化
    USART1_Init();          //串口1初始化
    RCC_Configuration();    //系统时钟初始化
    TIM4_Int_Init(2000,45); //定时器
}
