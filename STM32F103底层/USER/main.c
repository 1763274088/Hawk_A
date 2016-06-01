#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "motor.h"
#include "KEY.h"
#include "futaba_sbus.h"
#include "communication.h"
#include "tim.h"
#include "rc_process.h"

//任务执行时间标志位
uint32_t time_1_task_ms = 0;
uint32_t time_2_task_ms = 0;
uint32_t time_3_task_ms = 0;

//遥控器处理
Target_Send Target;

//SBUS信号处理数组
unsigned char sbus_rx_buffer[25];

int mode;

uint32_t kkkk = 0;


int main(void) {
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    USART3_Config();
    USART2_Config();

    LED_Init();

    //初始化PWM输出
    motor_init();

    // 复位电调
    motor_start_output(4000);        // 油门最大
    delay_ms(1000);                    // 3S内油门调至最小
    motor_start_output(2000);        // 油门最小
    delay_ms(2000);

    //打开定时器中断
    TIM1_Configuration();
    while (1) {
        //==========================任务执行处理==============================//
        //把遥控器的数据发送给F427
        if (time_2_task_ms == 5) {
            time_2_task_ms = 0;
            //把遥控器的原始值，处理，转变为要控制的飞机的角度值
            if (mode == 1) {
                remote_process();
            }
            else {

            }
        }
        //====================================================================//
    }
}
