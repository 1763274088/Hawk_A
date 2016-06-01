#include "bsp.h"
#include "Control_Algorithm.h"
#include "delay.h"
#include "lsm303d.h"

uint8_t F_byte;
uint8_t S_byte;

float Gyro_value[3];
float Accel_value[3];
float Temp_value;

_sensor Gyro_sensor;		//陀螺仪结构体
_sensor Acce_sensor;		//加速度计结构体
_sensor Maga_sensor;        //地磁传感器结构体

PID_InitTypeDef Inside_link;    //定义一个内环结构体
PID_InitTypeDef	Outside_link;   //定义一个外环结构体

void BSP_initialization(void) {
    //初始化延时函数，初始化系统时钟
	delay_init(180);

    //设置中断优先级，设置为第二组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//初始化串口6
    USART6_Config();

    //初始化串口3
    Uart3_init();

    //延时一下
	delay_ms(1000);

    //初始化MPU6000，初始化完成时点亮指示灯
    if (MPU6000_Init() == 0) {
        LED_Init();		//初始化指示灯，亮
		delay_us(2000);
	}

//    //初始化LSM303D，初始化完成时关闭指示灯
//    if(LSM303D_Init() == 0) {
        GPIO_SetBits(GPIOE,GPIO_Pin_12);// 灭指示灯
//        delay_us(2000);
//    }
}

void Sensor_Calibration(void) {
	// 装入初始协方差值
	Gyro_sensor.his_covariance[0] = 0.01;
	Gyro_sensor.his_covariance[1] = 0.01;
	Gyro_sensor.his_covariance[2] = 0.01;
	Acce_sensor.his_covariance[0] = 0.01;
	Acce_sensor.his_covariance[1] = 0.01;
	Acce_sensor.his_covariance[2] = 0.01;
    Maga_sensor.his_covariance[0] = 0.01;
	Maga_sensor.his_covariance[1] = 0.01;
	Maga_sensor.his_covariance[2] = 0.01;

    //校准地磁传感器
   // Magnetism_Bias(Maga_sensor.quiet);
    
    //校准陀螺仪
    Gyroscope_Bias(Gyro_sensor.quiet);

    //校准加速度计
    Accelerometer_Bias(Acce_sensor.quiet);
    Acce_sensor.quiet[2] = 65536 / 8 - (Acce_sensor.quiet[2]);

    //初始化定时器3，作为任务调度器基准,基准为1ms
	TIM3_Int_Init(2000, 45);
}

void Parameter_loading(void) {

    //===================外环参数===================//
    //pitch轴
    Outside_link.Pitch_kp = 0;
    Outside_link.Pitch_ki = 0;
    Outside_link.Pitch_kd = 0;
    //roll轴
    Outside_link.Roll_kp = 0;
    Outside_link.Roll_ki = 0;
    Outside_link.Roll_kd = 0;
    //yaw轴
    Outside_link.Yaw_kp = 0;
    Outside_link.Yaw_ki = 0;
    Outside_link.Yaw_kd = 0;
    //==============================================//

    //===================内环参数===================//
    //pitch轴
    Inside_link.Pitch_kp = 0.8;
    Inside_link.Pitch_ki = 0;
    Inside_link.Pitch_kd = 0;
    //roll轴
    Inside_link.Roll_kp = 0.8;
    Inside_link.Roll_ki = 0;
    Inside_link.Roll_kd = 0;
    //yaw轴
    Inside_link.Yaw_kp = 0;
    Inside_link.Yaw_ki = 0;
    Inside_link.Yaw_kd = 0;
    //==============================================//
}
