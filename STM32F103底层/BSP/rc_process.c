#include "rc_process.h"
#include "futaba_sbus.h"
#include "communication.h"

_SBUS_RECEIVE_DATA Sbus_Receive_Data;

//所能控制的最大角度值
#define Max_angle		25
#define Zoom			10
#define Send_length		sizeof(_target_result)


//原始值死区处理
int16_t deathzoom(int16_t x, int16_t zoom) {
    int16_t result;
    if (x > 0) {
        result = x - zoom;
        if (result < 0) {
            result = 0;
        }
    }
    else {
        result = x + zoom;
        if (result > 0) {
            result = 0;
        }
    }
    return (result);
}

void remote_process(void) {
    uint8_t Data_length;
    int16_t pitch_difference;
    int16_t roll_difference;

    //处理前，先对校验和进行清零处理
    Target.control_target.check = 0;

    //发送的包头：固定为0xAA
    Target.control_target.head = 0xAA;

    //发送的包头：固定为0xBB
    Target.control_target.head_1 = 0xBB;

    //控制的模式：三种模式，分别为姿态、手动、导航、停止模式
    if (Sbus_Receive_Data.Channel_received_value[4] == 306)
        Target.control_target.mode = 0;

    if (Sbus_Receive_Data.Channel_received_value[4] == 1000)
        Target.control_target.mode = 1;

    if (Sbus_Receive_Data.Channel_received_value[4] == 1694)
        Target.control_target.mode = 2;

    //油门值：遥控器处理之后的油门值（原始数值）
    Target.control_target.Altiude = 2000 - Sbus_Receive_Data.Channel_received_value[2];                    //油门控制通道值;

    //==================================================//
    //采用对应的比例处理
    //控制的Pitch轴角度值，限制在控制的范围内
    pitch_difference = (Sbus_Receive_Data.Channel_received_value[1] - 1000);	//pitch轴控制通道值
    Target.control_target.Pitch = Max_angle * (deathzoom(pitch_difference, Zoom) / 690.0f);
    //==================================================//

    //==================================================//
    //采用对应的比例处理
    //控制的Roll轴角度值，限制在控制的范围内
    roll_difference = (1000 - Sbus_Receive_Data.Channel_received_value[0]);		//roll轴控制通道值
    Target.control_target.Roll = Max_angle * (deathzoom(roll_difference, Zoom) / 690.0f);
    //==================================================//

    //==================================================//
    //由于YAW轴的处理比较特殊，所以这里不做具体处理
    //控制的Yaw轴角度值，限制在控制的范围内
    Target.control_target.Yaw = Sbus_Receive_Data.Channel_received_value[3];	//yaw轴控制通道值
    //==================================================//

    //==================================================//
    //*****特别注意：发送后要清零，或者计算前要清零，否则，校验的数据将是错的!*****//
    //数据包的校验和：所有数据的累加和
    for (Data_length = 0; Data_length < (Send_length - 1); Data_length++)
        Target.control_target.check += Target.Send_buffer[Data_length];
    //==================================================//

    //把处理完之后的值，发送出去
    usart_send_data(Send_length, Target.Send_buffer);

}



