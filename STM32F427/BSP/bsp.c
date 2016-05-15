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


_sensor Gyro_sensor;				//�����ǽṹ��
_sensor Acce_sensor;				//���ٶȼƽṹ��


 PID_InitTypeDef	Inside_link;
//����һ���⻷�ṹ��
 PID_InitTypeDef	Outside_link;


extern char table;

void BSP_initialization(void)
{
	uint8_t table;
		//��ʼ����ʱ��������ʼ��ϵͳʱ��
		delay_init(180);
		//�����ж����ȼ�������Ϊ�ڶ���
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		USART6_Config();
		//��ʼ������3
    Uart_init();
		//��ʼ��MPU6000
		table=MPU6000_Init();
		if(table==0)
		//��ʼ��ָʾ��
		LED_Init();	
				//��ʼ������6

		//��ʼ��TCA62724
//		TCA62724_Init();

}



void Sensor_Calibration(void)
{	
		//У׼������
		Gyroscope_Bias(Gyro_sensor.quiet);
		//У׼���ٶȼ�	
		Accelerometer_Bias(Acce_sensor.quiet);
		Acce_sensor.quiet[2]=65536/8-(Acce_sensor.quiet[2]);
		table=1;
		if(table==1)
		//��ʼ����ʱ��3����Ϊ�����������׼,��׼Ϊ1ms
		TIM3_Int_Init(2000,45);				
}


void Parameter_loading(void)
{
		//===================�⻷����===================//
			//pitch��
		Outside_link.Pitch_kp=0;
		Outside_link.Pitch_ki=0;
		Outside_link.Pitch_kd=0;
	
		//roll��
		Outside_link.Roll_kp=0;
		Outside_link.Roll_ki=0;
		Outside_link.Roll_kd=0;
	
		//yaw��
		Outside_link.Yaw_kp=0;
		Outside_link.Yaw_ki=0;
		Outside_link.Yaw_kd=0;
		//==============================================//


		//===================�ڻ�����===================//
		//pitch��
		Inside_link.Pitch_kp=0.8;	
		Inside_link.Pitch_ki=0;		
		Inside_link.Pitch_kd=0;	

		//roll��
		Inside_link.Roll_kp=0.8;
		Inside_link.Roll_ki=0;
		Inside_link.Roll_kd=0;
	
		//yaw��
		Inside_link.Yaw_kp=0;
		Inside_link.Yaw_ki=0;
		Inside_link.Yaw_kd=0;
		//==============================================//

}

//		TCA62724_Write_Pwm_Data(0x00,TCA62724_IIC_PWM_Duty_1,TCA62724_IIC_PWM_Duty_1,TCA62724_IIC_PWM_Duty_1);	
//	
//		TCA62724_Read_staus_Data(&F_byte,&S_byte );
















