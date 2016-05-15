#include "tim.h"
#include "mpu6000.h"
#include "uart.h"
#include "Filtering_Algorithm.h"
#include "Attitude_Algorithm.h"
#include "Control_Algorithm.h"

char table=0;
#define Gyro_Gr	0.0005426
#define Gyro_G	0.030517578125

//��̬�������
float ROLL=0,YAW=0,PIT=0,HALF_T=0.001;

//���������Ǻͼ��ٶȼƵ�ƫ��ֵ��ԭʼֵ�;�ֵ̬�Ĳ�ֵ��
int16_t GYRO_transition[3];
int16_t ACCE_transition[3];

uint32_t time_2_ms=0;
uint32_t time_3_ms=0;
//������յ����ݱ�־λ,��communication�ļ������д���
extern uint8_t	give_data_table;

//���Ϳ��Ʊ�־λ
uint8_t motor_send_table=0;


//�������ʱ������ʱ����Ϊ1ms
void TIM3_Int_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
		
		TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	//�Զ���װ��ֵ
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;  //��ʱ����Ƶ
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
		
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
		TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
		
		NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

void time_1ms_task_loop(void)
{
		//��ȡң��������ֵ
		if(give_data_table==1)
		{
		Calculate_target_amount();
		give_data_table=0;
		}
			
		//��ȡMPU6000�������Ǻͼ��ٶȼƵ�ԭʼ��ֵ
		MPU6000_Get_Original_Value(Gyro_sensor.origin,	Acce_sensor.origin,0);
	
		//��������Ǻͼ��ٶȼƵľ�̬ƫ��
		GYRO_transition[0]=(-(Gyro_sensor.origin[0]))-Gyro_sensor.quiet[0];
		GYRO_transition[1]=(Gyro_sensor.origin[1])-Gyro_sensor.quiet[1];
		GYRO_transition[2]=(-(Gyro_sensor.origin[2]))-Gyro_sensor.quiet[2];
	
		ACCE_transition[0]=(-(Acce_sensor.origin[0]))-Acce_sensor.quiet[0];
		ACCE_transition[1]=(Acce_sensor.origin[1])-Acce_sensor.quiet[1];
		ACCE_transition[2]=(-(Acce_sensor.origin[2]))-Acce_sensor.quiet[2];		

		//��ԭʼ��ֵ�����˲�,���û����˲�
		Moving_Average_Filter(GYRO_transition,ACCE_transition,Gyro_sensor.Filter	,Acce_sensor.Filter);
	
		//�������ǵ�ֵת��Ϊ����ֵ
		Gyro_sensor.radian[0]=Gyro_sensor.Filter[0]*Gyro_Gr;
		Gyro_sensor.radian[1]=Gyro_sensor.Filter[1]*Gyro_Gr;
		Gyro_sensor.radian[2]=Gyro_sensor.Filter[2]*Gyro_Gr;
	
}


void time_2ms_task_loop(void)
{
			
//	printf("%f,%f,%f,%f,%f,%f\r\n",Gyro_sensor.radian[0],Gyro_sensor.radian[1],Gyro_sensor.radian[2],
//																Acce_sensor.Filter[0],	Acce_sensor.Filter[1],	Acce_sensor.Filter[2]);
		//��̬���㣬���ROLL��PIT��YAW�����Ƕ�ֵ
		AHRS_Update(Acce_sensor.Filter[0],Acce_sensor.Filter[1], Acce_sensor.Filter[2], Gyro_sensor.radian[0], Gyro_sensor.radian[1], Gyro_sensor.radian[2],HALF_T, &ROLL,&PIT,&YAW );
		
		//�ǶȻ���4ms��ִ������
//		Angle_Control_loop();
	
	Outside_link.Pitch_CtrOut=0;
	Outside_link.Roll_CtrOut=0;
	Outside_link.Yaw_CtrOut=0;
		//���ٶȻ���2ms��ִ������
		Angle_speed_control_loop();
	
		//�����������
		Motor_Control();
		
		//���ͱ�־λ��һ
		motor_send_table=1;
		
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/*
������ȴ���
ѭ��������Ȳ�ͬ�ĺ���������ÿ������
ִ�����ڣ�1MS�ж�
*/
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//��ʱ��3�жϷ�����

void TIM3_IRQHandler(void)
{
		static int16_t time_1_ms=0;
//		static int16_t time_2_ms=0;
//		static int16_t time_5_ms=0;
	
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
		{
		
		time_2_ms++;
		time_1_ms++;
		time_3_ms++;

				
			
		//��1����ִ������	
		if(time_1_ms==1)
		{	
		time_1ms_task_loop();
		}
		//��2����ִ������
		if(time_1_ms==2)
		{
			time_2ms_task_loop();
			GPIO_ToggleBits( GPIOA,GPIO_Pin_1);				
  		time_1_ms=0;	
		}

		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}


