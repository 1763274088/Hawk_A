#include "stm32f4xx.h"
#include "bsp.h"
#include "Math_Algorithm.h"
#include "Control_Algorithm.h"
#include "communication.h"
#include "PPM.h"
#include "fm25v01.h"

//应用其他文件中的计时器
extern uint32_t time_2_ms;
extern uint32_t time_10_ms;

//引用其他文件中的三轴值
extern float ROLL, YAW, PIT;

extern _F4_to_PPM_ F4_to_PPM_BUF;       //引入F4向PPM发送的数据联合体
extern _PPM_to_F4_ PPM_to_F4_BUF;       //引入PPM向F4发送的数据联合体
extern uint8_t UART3_Receive_Data;      //定义UART3的完全接受完成标志

// 定义控制电机信号结构体
_MOTOR_CONTROL motor_tc;

// 引用其他文件中的Motor_channel
extern uint16_t Motor_channel[6];

//发送控制标志位
extern uint8_t motor_send_table;

//开机次数,先从eeprom中读取值，如果为0，则系统首次运行，需要重置，不为0，则系统已重启过一次，将其归0，不重置。
//s8 start_time;


int main(void) {

    //初始化所有外设
    BSP_initialization();

    //校准传感器的静态值
    Sensor_Calibration();

    //加载控制参数
    Parameter_loading();

    while (1) {
        if (time_10_ms >= 10) {
            time_10_ms = 0;
			Send_PPM_Data();        // 向调试器发送数据
        }
        else if (UART3_Receive_Data && (((PPM_to_F4_BUF.DATA.mode_Altiude > 12)) & 0x000F) == 1) { 	// 自稳模式
            if (time_2_ms >= 2) {
                time_2_ms = 0;
                if (motor_send_table == 1) {
                    //把控制值发送出去
                    Motor_Send_task();
                    motor_send_table = 0;
                }
            }
        }
    }
}
