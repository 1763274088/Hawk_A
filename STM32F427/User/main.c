#include "stm32f4xx.h"
#include "bsp.h"
#include "Math_Algorithm.h"
#include "Control_Algorithm.h"
void MyTimeDelay(__IO uint32_t nTime)
{
    uint32_t i = 0;
    uint32_t j = 0;
    for(i=0;i<nTime;i++)
    for(j=0;j<5000;j++);
}


extern uint32_t time_2_ms;
extern uint32_t time_3_ms;
extern float ROLL,YAW,PIT;

extern uint8_t give_buffer[16];
extern uint8_t give_buffer22222[16];

extern _REMOTE_RECEIVE	RC_bf; 

//extern 	uint8_t kkk;

extern uint8_t table_table;
extern uint8_t ggggg_table;

extern uint16_t Motor_channel[6];
// PID_InitTypeDef	Inside_link;
////定义一个外环结构体
// PID_InitTypeDef	Outside_link;
////定义一个目标结构体
// target_InitTypeDef	control_target;
uint32_t uu=0;
_MOTOR_CONTROL motor_tc;

//发送控制标志位
extern uint8_t motor_send_table;


extern float OutData[4]; 

unsigned char SendBuff1[16]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0xdd,0xab,0xee,0x1e,0x12,0x22};	//发送数据缓冲区
int main(void)
{
		//初始化所有外设
		BSP_initialization();
	
		//校准传感器的静态值
		Sensor_Calibration();
	
		//加载控制参数
		Parameter_loading();				
	
  while(1)		
	{


		if(time_3_ms>=10)
		{
		time_3_ms=0;

//		OutData[0]=Gyro_sensor.radian[2]*57.324841;
//		OutPut_Data();
//		OutData[1]=YAW;
//		OutPut_Data();
			
//		OutData[2]=Gyro_sensor.radian[1]*57.324841;
//		OutPut_Data();
//		OutData[3]=PIT;
//		OutPut_Data();
			
//		OutData[0]=Outside_link.Pitch_CtrOut;
//		OutPut_Data();
//		OutData[1]=Gyro_sensor.radian[1]*57.324841;
//		OutPut_Data();
//		OutData[2]=Inside_link.Pitch_CtrOut;
//		OutPut_Data();
			
		}
		
		if(RC_bf.control_target.mode==0)
		{
		Motor_Send_stop_task();
		}	
		else
		{	
		
		if(time_2_ms>=2)
		{
				time_2_ms=0;
			
				if(motor_send_table>=1)
				{
				//把控制值发送出去
				Motor_Send_task();
				motor_send_table=0;
				}
		}
	}

		
		
	}

	
}


		
		







