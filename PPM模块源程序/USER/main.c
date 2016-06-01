/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * 文件名  ：main.c
 * 描述    ：系统初始化
 * 实验平台：HT飞控
 * 库版本  ：ST3.5.0
 * 作者    ：Air Nano Team
**********************************************************************************/
#include "include.h"
#include "usart.h"
#include "key.h"
#include "page.h"

extern _F4_to_PPM_ F4_to_PPM_BUF;   //引入F4向PPM发送的数据联合体
extern _PPM_to_F4_ PPM_to_F4_BUF;   //引入PPM向F4发送的数据联合体
extern uint8_t USART1_Receive_Data; //导入USART1的完全接受完成标志

int main(void) {
    IAC_Init();     // 接口以及外设的初始化
    LED_Power();    // 指示灯
	PPM_to_F4_BUF.DATA.mode_Altiude = 2000; //初始油门为2000
    while (1) {
        OLED_Display();
		if (USART1_Receive_Data)        // 有数据接收亮点，无则不显示
			Dis_Char(7, 0, (USART1_Receive_Data = 0) + '.');
		else
			Dis_Char(7, 0, ' ');
    }
}
/******************* (C) COPYRIGHT 2014 BEYOND *****END OF FILE************/

