#include "tim.h"
#include "mpu6000.h"
#include "uart.h"
#include "Filtering_Algorithm.h"
#include "Attitude_Algorithm.h"
#include "Control_Algorithm.h"
#include "lsm303d.h"

extern _sensor Gyro_sensor;		//陀螺仪结构体
extern _sensor Acce_sensor;		//加速度计结构体
extern _sensor Maga_sensor;     //地磁传感器结构体

char table = 0;
#define Gyro_Gr		0.0005426f
#define Gyro_G		0.030517578125f

//姿态解算变量
float ROLL = 0, YAW = 0, PIT = 0, HALF_T = 0.001;

//保存陀螺仪和加速度计的偏差值（原始值和静态值的差值）
int16_t GYRO_transition[3];
int16_t ACCE_transition[3];
int16_t MAGA_transition[3];

int16_t 	time_1_ms = 0;
uint32_t 	time_2_ms = 0;
uint32_t 	time_10_ms = 0;

extern uint8_t UART3_Receive_Data;  //定义UART3的完全接受完成标志

//发送控制标志位
uint8_t motor_send_table = 0;

//任务管理定时器，定时周期为1ms
void TIM3_Int_Init(u16 arr, u16 psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//使能TIM3时钟

    TIM_TimeBaseInitStructure.TIM_Period = arr - 1;					//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;				//定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);		//初始化TIM3

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);				//允许定时器3更新中断
    TIM_Cmd(TIM3, ENABLE);									//使能定时器3

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;					//定时器3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;    //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////

void time_1ms_task_loop(void) {
//读取遥控器的数值
    if (UART3_Receive_Data == 1) {
		Calculate_target_amount();
        UART3_Receive_Data = 0;
    }

//读取MPU6000的陀螺仪和加速度计的原始数值
    MPU6000_Get_Original_Value(Gyro_sensor.origin, Acce_sensor.origin, 0);

//读取LSM303D的地磁计和加速度计的原始值
    //LSM303D_DataPrepare(Maga_sensor.origin, 0);

//读取地磁感应器电子罗盘复合模块
//    MAGA_transition[0] = Maga_sensor.origin[0] - Maga_sensor.quiet[0];
//    MAGA_transition[1] = Maga_sensor.origin[1] - Maga_sensor.quiet[1];
//    MAGA_transition[2] = Maga_sensor.origin[2] - Maga_sensor.quiet[2];

//计算陀螺仪和加速度计的静态偏差
    GYRO_transition[0] = (-(Gyro_sensor.origin[0])) - Gyro_sensor.quiet[0];
    GYRO_transition[1] = (+(Gyro_sensor.origin[1])) - Gyro_sensor.quiet[1];
    GYRO_transition[2] = (-(Gyro_sensor.origin[2])) - Gyro_sensor.quiet[2];
    ACCE_transition[0] = (-(Acce_sensor.origin[0])) - Acce_sensor.quiet[0];
    ACCE_transition[1] = (+(Acce_sensor.origin[1])) - Acce_sensor.quiet[1];
    ACCE_transition[2] = (-(Acce_sensor.origin[2])) - Acce_sensor.quiet[2];

//对原始数值进行滤波,采用先卡尔曼，后滑动滤波
	Data_KalmanFilter(Gyro_sensor.his_covariance,Gyro_sensor.histor,GYRO_transition,Gyro_sensor.Filter);
	Data_KalmanFilter(Acce_sensor.his_covariance,Acce_sensor.histor,ACCE_transition,Acce_sensor.Filter);
    Data_KalmanFilter(Maga_sensor.his_covariance,Maga_sensor.histor,MAGA_transition,Maga_sensor.Filter);
//    Moving_Average_Filter(GYRO_transition, ACCE_transition, Gyro_sensor.Filter, Acce_sensor.Filter);

//把陀螺仪的值转化为弧度值
    Gyro_sensor.radian[0] = Gyro_sensor.Filter[0] * Gyro_Gr;
    Gyro_sensor.radian[1] = Gyro_sensor.Filter[1] * Gyro_Gr;
    Gyro_sensor.radian[2] = Gyro_sensor.Filter[2] * Gyro_Gr;
}
////////////////////////////////

void time_2ms_task_loop(void) {

    //姿态解算，算出ROLL、PIT、YAW三个角度值
    AHRS_Update(Acce_sensor.Filter[0], Acce_sensor.Filter[1], Acce_sensor.Filter[2], Gyro_sensor.radian[0],
                Gyro_sensor.radian[1], Gyro_sensor.radian[2], HALF_T, &ROLL, &PIT, &YAW);

    //角度环：4ms的执行周期
//		Angle_Control_loop();

    Outside_link.Pitch_CtrOut = 0;
    Outside_link.Roll_CtrOut = 0;
    Outside_link.Yaw_CtrOut = 0;
    //角速度环：2ms的执行周期
    Angle_speed_control_loop();

    //计算电机输出量
    Motor_Control();

    //发送标志位置一
    motor_send_table = 1;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/*
任务调度处理
循环间隔调度不同的函数，处理每个任务
执行周期：1MS中断
*/
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//定时器3中断服务函数

void TIM3_IRQHandler(void) {

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
    {
        time_2_ms++;
        time_1_ms++;
        time_10_ms++;

        //第1毫秒执行任务
        if (time_1_ms == 1) {
            time_1ms_task_loop();
        }
        //第2毫秒执行任务
        if (time_1_ms == 2) {
            time_2ms_task_loop();
            time_1_ms = 0;
        }
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除中断标志位
}


