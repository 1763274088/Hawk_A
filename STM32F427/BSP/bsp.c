#include "bsp.h"
#include "Control_Algorithm.h"

uint8_t F_byte;
uint8_t S_byte;


float Gyro_value[3];
float Accel_value[3];
float Temp_value;

_MS5611_PROM_DATA_SHARED MS5611_PROM_DATA_SHARED;
_MS5611_NORMAL_DATA_SHARED MS5611_NORMAL_DATA_SHARED;
FM25V01_WRITE_ADDRESS FM25V01_Write;
FM25V01_READ_ADDRESS FM25V01_Read;
FM25V01_Manufacturer_Product_ID FM25V01_ID;


_sensor Gyro_sensor;				//陀螺仪结构体
_sensor Acce_sensor;				//加速度计结构体


 PID_InitTypeDef	Inside_link;
//定义一个外环结构体
 PID_InitTypeDef	Outside_link;


extern char table;

void BSP_initialization(void)
{
	uint8_t table;
		//初始化延时函数，初始化系统时钟
		delay_init(180);
		//设置中断优先级，设置为第二组
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		USART6_Config();
		//初始化串口3
    Uart_init();
		//初始化MPU6000
		table=MPU6000_Init();
		if(table==0)
		//初始化指示灯
		LED_Init();	
				//初始化串口6

		//初始化TCA62724
//		TCA62724_Init();

}



void Sensor_Calibration(void)
{	
		//校准陀螺仪
		Gyroscope_Bias(Gyro_sensor.quiet);
		//校准加速度计	
		Accelerometer_Bias(Acce_sensor.quiet);
		Acce_sensor.quiet[2]=65536/8-(Acce_sensor.quiet[2]);
		table=1;
		if(table==1)
		//初始化定时器3，作为任务调度器基准,基准为1ms
		TIM3_Int_Init(2000,45);				
}


void Parameter_loading(void)
{
		//===================外环参数===================//
			//pitch轴
		Outside_link.Pitch_kp=0;
		Outside_link.Pitch_ki=0;
		Outside_link.Pitch_kd=0;
	
		//roll轴
		Outside_link.Roll_kp=0;
		Outside_link.Roll_ki=0;
		Outside_link.Roll_kd=0;
	
		//yaw轴
		Outside_link.Yaw_kp=0;
		Outside_link.Yaw_ki=0;
		Outside_link.Yaw_kd=0;
		//==============================================//


		//===================内环参数===================//
		//pitch轴
		Inside_link.Pitch_kp=0.8;	
		Inside_link.Pitch_ki=0;		
		Inside_link.Pitch_kd=0;	

		//roll轴
		Inside_link.Roll_kp=0.8;
		Inside_link.Roll_ki=0;
		Inside_link.Roll_kd=0;
	
		//yaw轴
		Inside_link.Yaw_kp=0;
		Inside_link.Yaw_ki=0;
		Inside_link.Yaw_kd=0;
		//==============================================//

}

//		TCA62724_Write_Pwm_Data(0x00,TCA62724_IIC_PWM_Duty_1,TCA62724_IIC_PWM_Duty_1,TCA62724_IIC_PWM_Duty_1);	
//	
//		TCA62724_Read_staus_Data(&F_byte,&S_byte );
















