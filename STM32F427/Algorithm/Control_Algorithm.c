#include "Control_Algorithm.h"
#include "Math_Algorithm.h"
#include "communication.h"
#include "uart.h"
#include<math.h>
#define RtA 		57.324841					//弧度转成角度系数

extern _sensor Gyro_sensor;				//陀螺仪结构体

//电机通道值
uint16_t Motor_channel[6]={0,0,0,0,0,0};

/*------------------电机最低数--------------------*/
#define Low_value 2000


#define Motor_Max_output 2800


//外环积分增量限幅
#define	pitch_increment_max			200
#define	roll_increment_max			200


//
extern _REMOTE_RECEIVE	RC_bf;

//起飞最低油门值
#define Altiude_min	330

//最低起飞油门标志位
uint8_t Altiude_min_table=0;


//定义一个目标结构体
target_InitTypeDef	control_target_value;

//===================================================================//
		//####################################################//
		/*
		根据接收F1传输回来的目标量，进行相对应的操作，优化目标
		控制量
		*/
		//####################################################//

		//输入：F1传输回来的pitch、roll、yaw、油门等等数值
		//输出：处理之后的目标值
		//注意：yaw轴要进行补全处理
void Calculate_target_amount(void)
{
		int16_t	yaw_difference;
	
		//保存遥控器处理之后的目标值
		control_target_value.target_Pitch=RC_bf.control_target.Pitch;
		control_target_value.target_Roll=RC_bf.control_target.Roll;
		control_target_value.target_Altitude=RC_bf.control_target.Altiude;	
		
		//======================================================================//
		//YAW轴的数据比较特殊，需要进行处理	
		//当油门低于最小起飞油门时，此时的航向作为目标航向
		if(control_target_value.target_Altitude>Altiude_min)
		{
		if(Altiude_min_table!=1)
		{
		Altiude_min_table=1;
		control_target_value.target_Yaw=YAW;	
		}
		}
		else
		{
		Altiude_min_table=0;
		control_target_value.target_Yaw=YAW;	
		}
		
		//航向设置一个死区
		if(RC_bf.control_target.Yaw>1100||RC_bf.control_target.Yaw<900)
		{
		yaw_difference=1000-RC_bf.control_target.Yaw;
		control_target_value.target_Yaw+=(yaw_difference/200.0f)*0.05f;	
		
		if(control_target_value.target_Yaw>180.0f)
		{
			control_target_value.target_Yaw-=360.0f;
		}
		else
			if(control_target_value.target_Yaw<-180.0f)
				control_target_value.target_Yaw+=360.0f;
		
		}
	
		//======================================================================//	
	
}
//===================================================================//






//===================================================================//
		//####################################################//
		/*
		控制算法主要采用串级控制，内环为角速度环，外环为角度环。
		内环的主要作用是：及时响应飞机的飞行变化，比如姿态偏移的
		回复速度，飞行状态的变化速度等等。
		外环的主要作用是：保证飞机的姿态稳定，控制飞机的飞行姿态，
		减小姿态误差。
		*/
		//####################################################//

//执行频率标志位
uint8_t execute_frequency=0;

//外环：角度环
//输入：遥控器等姿态控制
//反馈量：姿态解算的角度值
//输出量：需要的补偿值
//采用PID
void Angle_Control_loop(void)
{
	execute_frequency++;

	//4ms周期一次
	if(execute_frequency>= 2)
	{
		//************************PITCH部分控制************************//
		//误差部分：遥控器的目标值作为控制目标，姿态解算的角度值作为反馈目标
		Outside_link.Pitch_ErrNow=PIT-control_target_value.target_Pitch;
		
		//增量部分：对误差进行累加操作
		Outside_link.Pitch_increment+=Outside_link.Pitch_ErrNow;
		
		//比例部分:	系数乘以误差值，作为比例控制
		Outside_link.Pitch_ErrP_out=Outside_link.Pitch_kp*Outside_link.Pitch_ErrNow;
		
		//积分限幅部分：对增量进行限幅，抑制积分饱和
			if(Outside_link.Pitch_increment > Outside_link.Pitch_ErrILim)  	
				Outside_link.Pitch_increment = Outside_link.Pitch_ErrILim;
			else if(Outside_link.Pitch_increment < -(Outside_link.Pitch_ErrILim))		
				Outside_link.Pitch_increment = -(Outside_link.Pitch_ErrILim);
		
		//积分部分：系数乘以积分增量，作为积分
		Outside_link.Pitch_ErrI_out=Outside_link.Pitch_ki*Outside_link.Pitch_increment;	
		
		//微分部分:	用历史值作为控制
		Outside_link.Pitch_ErrD_out=Outside_link.Pitch_kd*(Outside_link.Pitch_ErrNow-Outside_link.Pitch_Errpast);
		
		//输出部分：将比例、积分、微分的输出值进行累加操作
		Outside_link.Pitch_CtrOut=Outside_link.Pitch_ErrP_out+Outside_link.Pitch_ErrI_out+Outside_link.Pitch_ErrD_out;
		Outside_link.Pitch_CtrOut=-Outside_link.Pitch_CtrOut;	
			
		//历史值部分：保存这时刻的误差			
		Outside_link.Pitch_Errpast=Outside_link.Pitch_ErrNow;	
		//*************************************************************//

			
		//************************Roll部分控制************************//
		//误差部分：遥控器的目标值作为控制目标，姿态解算的角度值作为反馈目标
		Outside_link.Roll_ErrNow=ROLL-control_target_value.target_Roll;
		//增量部分：对误差进行累加操作
		Outside_link.Roll_increment+=Outside_link.Roll_ErrNow;
		
		//比例部分:	系数乘以误差值，作为比例控制
		Outside_link.Roll_ErrP_out=Outside_link.Roll_kp*Outside_link.Roll_ErrNow;
		
		//积分限幅部分：对增量进行限幅，抑制积分饱和
			if(Outside_link.Roll_increment > Outside_link.Roll_ErrILim)  	
				Outside_link.Roll_increment = Outside_link.Roll_ErrILim;
			else if(Outside_link.Roll_increment < -(Outside_link.Roll_ErrILim))		
				Outside_link.Roll_increment = -(Outside_link.Roll_ErrILim);
		
		//积分部分：系数乘以积分增量，作为积分
		Outside_link.Roll_ErrI_out=Outside_link.Roll_ki*Outside_link.Roll_increment;	
		
		//微分部分:	用历史值作为控制
		Outside_link.Roll_ErrD_out=Outside_link.Roll_kd*(Outside_link.Roll_ErrNow-Outside_link.Roll_Errpast);
		
		//输出部分：将比例、积分、微分的输出值进行累加操作
		Outside_link.Roll_CtrOut=Outside_link.Roll_ErrP_out+Outside_link.Roll_ErrI_out+Outside_link.Roll_ErrD_out;
		Outside_link.Roll_CtrOut=-Outside_link.Roll_CtrOut;	
			
		//历史值部分：保存这时刻的误差			
		Outside_link.Roll_Errpast=Outside_link.Roll_ErrNow;	
		//*************************************************************//

				
		//************************Yaw部分控制************************//
		//先对Yaw轴做一些处理,在进行算法的运算
		 if((control_target_value.target_Yaw- YAW)>180 || (control_target_value.target_Yaw - YAW)<-180)
		{
       if(control_target_value.target_Yaw>0 && YAW<0)  Outside_link.Yaw_ErrNow= (-180 - YAW) +(control_target_value.target_Yaw - 180);
       if(control_target_value.target_Yaw<0 && YAW>0)  Outside_link.Yaw_ErrNow= (180 - YAW) +(control_target_value.target_Yaw+ 180);
    }
    else  Outside_link.Yaw_ErrNow = control_target_value.target_Yaw - YAW;
		
		//比例部分:	系数乘以误差值，作为比例控制
		Outside_link.Yaw_ErrP_out=Outside_link.Yaw_kp*Outside_link.Roll_ErrNow;
		
		//微分部分:	用历史值作为控制
		Outside_link.Yaw_ErrD_out=Outside_link.Yaw_kd*(Outside_link.Yaw_ErrNow-Outside_link.Yaw_Errpast);
		
		//输出部分：将比例、微分的输出值进行累加操作
		Outside_link.Yaw_CtrOut=Outside_link.Yaw_ErrP_out+Outside_link.Yaw_ErrD_out;
		
		//历史值部分：保存这时刻的误差	
		Outside_link.Yaw_Errpast=Outside_link.Yaw_ErrNow;
		//*************************************************************//
			
    execute_frequency = 0; 	
	}

}



//内环：角速度环
//输入：外环的输出
//反馈量：陀螺仪的角速度值
//输出量：无刷电机的控制量
//采用PD控制
void Angle_speed_control_loop(void)
{
		//************************PITCH部分控制************************//
		//误差部分：外环的输出作为目标量，陀螺仪的角速度作为反馈量
		Inside_link.Pitch_ErrNow=(Outside_link.Pitch_CtrOut-Gyro_sensor.radian[1]*RtA);
		//比例部分:	系数乘以误差值，作为比例控制
		Inside_link.Pitch_ErrP_out=Inside_link.Pitch_kp	*	Inside_link.Pitch_ErrNow;
		//微分部分:	用陀螺仪的角速度作为微分环节控制，响应更快，控制更平滑
		Inside_link.Pitch_ErrD_out=Inside_link.Pitch_kd*(Inside_link.Pitch_Errpast-	Gyro_sensor.Filter[1]);
		//*************************************************************//

		//************************ROLL部分控制*************************//
		//误差部分：外环的输出作为目标量，陀螺仪的角速度作为反馈量
		Inside_link.Roll_ErrNow=(Outside_link.Roll_CtrOut-Gyro_sensor.radian[0]*RtA);
		//比例部分:	系数乘以误差值，作为比例控制
		Inside_link.Roll_ErrP_out=Inside_link.Roll_kp	*	Inside_link.Roll_ErrNow;
		//微分部分:	用陀螺仪的角速度作为微分环节控制，响应更快，控制更平滑
		Inside_link.Roll_ErrD_out=Inside_link.Roll_kd*(Inside_link.Roll_Errpast-	Gyro_sensor.Filter[0]);
		//*************************************************************//
	
		//************************Yaw部分控制**************************//
		//误差部分：外环的输出作为目标量，陀螺仪的角速度作为反馈量
		Inside_link.Yaw_ErrNow=(Outside_link.Yaw_CtrOut-Gyro_sensor.radian[2]*RtA);
		//比例部分:	系数乘以误差值，作为比例控制
		Inside_link.Yaw_ErrP_out=Inside_link.Yaw_kp	*	Inside_link.Yaw_ErrNow;
		//微分部分:	用陀螺仪的角速度作为微分环节控制，响应更快，控制更平滑
		Inside_link.Yaw_ErrD_out=Inside_link.Yaw_kd*(Inside_link.Yaw_Errpast-	Gyro_sensor.Filter[2]);
		//*************************************************************//
	
		//************************输出控制部分*************************//	
		Inside_link.Pitch_CtrOut	=Inside_link.Pitch_ErrP_out	+	Inside_link.Pitch_ErrD_out;
		Inside_link.Roll_CtrOut		=Inside_link.Roll_ErrP_out	+	Inside_link.Roll_ErrD_out;
		Inside_link.Yaw_CtrOut		=Inside_link.Yaw_ErrP_out		+	Inside_link.Yaw_ErrD_out;
		//*************************************************************//
		
		//*********************保存上一次的角速度值********************//
		Inside_link.Pitch_Errpast	=	Gyro_sensor.Filter[1];	
		Inside_link.Roll_Errpast	=	Gyro_sensor.Filter[0];
		Inside_link.Yaw_Errpast		=	Gyro_sensor.Filter[2];			
		//*************************************************************//
}

//===================================================================//







//===================================================================//
		//####################################################//
		/*
			根据串级PID控制，计算出所需要的电机输出量，再根据对应的机型，设置
			不同的输出，然后每个电机根据各自的输出值进行动作，达到所需要的控制
			效果。
		*/
		//####################################################//

		//输入：计算出所需的油门、Pitch、Roll、Yaw的值
		//输出：无刷电机的PWM值
		//注意：油门进行相对应的补偿处理，还有平滑处理
		
		int16_t Pitch_output,Roll_output,Yaw_output,Throttle;
		int	Throttle_value;		
void Motor_Control(void)
{
		uint8_t count;
	
		//得到所需要的控制量
		Pitch_output	=Inside_link.Pitch_CtrOut;
		Roll_output		=Inside_link.Roll_CtrOut;
		Yaw_output		=Inside_link.Yaw_CtrOut;
		Throttle			=control_target_value.target_Altitude+1600;
		Throttle_value=Throttle;
		
//		Throttle=Throttle/cos(ROLL/RtA)/cos(PIT/RtA);
	
		//起飞油门处理：当油门大于起飞油门时，才进行对应的控制
		if(Throttle>2100)
		{
		 Motor_channel[0]=Throttle_value	+	Pitch_output	-	0.414*Roll_output			+	Yaw_output;
		 Motor_channel[1]=Throttle_value									-	Roll_output						-	Yaw_output;
		 Motor_channel[2]=Throttle_value	-	Pitch_output	-	0.414*Roll_output			+	Yaw_output;
		 Motor_channel[3]=Throttle_value	-	Pitch_output	+	0.414*Roll_output			-	Yaw_output;
		 Motor_channel[4]=Throttle_value									+	Roll_output						+	Yaw_output;
		 Motor_channel[5]=Throttle_value	+	Pitch_output	+	0.414*Roll_output			-	Yaw_output;		


		//==============限幅处理===============//
		for(count=0;count<6;count++)
		{
			if(Motor_channel[count]>=Motor_Max_output)
				Motor_channel[count]=Motor_Max_output;
		}			
		//=====================================//	
			
		}
		else
		{
		//当油门低于起飞油门时，清掉pitch和roll轴的角度环积分增量值，并且设置电机输出为最低输出
		Motor_channel[0]=Motor_channel[1]=Motor_channel[2]=Motor_channel[3]=Motor_channel[4]=Motor_channel[5]=Low_value;
		Outside_link.Pitch_increment	=0;
		Outside_link.Roll_increment		=0;
		}
		

		//油门上锁处理：电机输出为0.
//		Motor_channel[0]=Motor_channel[1]=Motor_channel[2]=Motor_channel[3]=Motor_channel[4]=Motor_channel[5]=0;
}
//===================================================================//



//===================================================================//
		//####################################################//
		/*
			把电机控制值，发送出去，包头为：0xCC，包尾是校验和
		*/
		//####################################################//

		//输入：计算出来的电机控制值
		//输出：无刷电机的PWM值
void Motor_Send_task(void)
{
		uint8_t check_sum;
		uint8_t seng_length;
	
		//配置发送结构体
		motor_tc.info.head=0xCC;
		motor_tc.info.Control_value[0]=Motor_channel[0];
		motor_tc.info.Control_value[1]=Motor_channel[1];	
		motor_tc.info.Control_value[2]=Motor_channel[2];	
		motor_tc.info.Control_value[3]=Motor_channel[3];	
		motor_tc.info.Control_value[4]=Motor_channel[4];	
		motor_tc.info.Control_value[5]=Motor_channel[5];	
		
		//求出发送数据包的校验和
		check_sum=0;
		for(seng_length=0;seng_length<13;seng_length++)
		check_sum+=motor_tc.TX_buffer[seng_length];
		
		motor_tc.info.check=check_sum;

		//把控制数据发送出去
		usart_send_data(14,	motor_tc.TX_buffer);
//		usart3_send_data(14,	motor_tc.TX_buffer);
	
}
//===================================================================//


void Motor_Send_stop_task(void)
{
		uint8_t check_sum;
		uint8_t seng_length;
	
		//配置发送结构体
		motor_tc.info.head=0xCC;
		motor_tc.info.Control_value[0]=2000;
		motor_tc.info.Control_value[1]=2000;	
		motor_tc.info.Control_value[2]=2000;	
		motor_tc.info.Control_value[3]=2000;	
		motor_tc.info.Control_value[4]=2000;	
		motor_tc.info.Control_value[5]=2000;	
		
		//求出发送数据包的校验和
		check_sum=0;
		for(seng_length=0;seng_length<13;seng_length++)
		check_sum+=motor_tc.TX_buffer[seng_length];
		
		motor_tc.info.check=check_sum;

		//把控制数据发送出去
		usart_send_data(14,	motor_tc.TX_buffer);
	
}

