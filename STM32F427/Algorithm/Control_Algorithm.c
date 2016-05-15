#include "Control_Algorithm.h"
#include "Math_Algorithm.h"
#include "communication.h"
#include "uart.h"
#include<math.h>
#define RtA 		57.324841					//����ת�ɽǶ�ϵ��

extern _sensor Gyro_sensor;				//�����ǽṹ��

//���ͨ��ֵ
uint16_t Motor_channel[6]={0,0,0,0,0,0};

/*------------------��������--------------------*/
#define Low_value 2000


#define Motor_Max_output 2800


//�⻷���������޷�
#define	pitch_increment_max			200
#define	roll_increment_max			200


//
extern _REMOTE_RECEIVE	RC_bf;

//����������ֵ
#define Altiude_min	330

//���������ű�־λ
uint8_t Altiude_min_table=0;


//����һ��Ŀ��ṹ��
target_InitTypeDef	control_target_value;

//===================================================================//
		//####################################################//
		/*
		���ݽ���F1���������Ŀ�������������Ӧ�Ĳ������Ż�Ŀ��
		������
		*/
		//####################################################//

		//���룺F1���������pitch��roll��yaw�����ŵȵ���ֵ
		//���������֮���Ŀ��ֵ
		//ע�⣺yaw��Ҫ���в�ȫ����
void Calculate_target_amount(void)
{
		int16_t	yaw_difference;
	
		//����ң��������֮���Ŀ��ֵ
		control_target_value.target_Pitch=RC_bf.control_target.Pitch;
		control_target_value.target_Roll=RC_bf.control_target.Roll;
		control_target_value.target_Altitude=RC_bf.control_target.Altiude;	
		
		//======================================================================//
		//YAW������ݱȽ����⣬��Ҫ���д���	
		//�����ŵ�����С�������ʱ����ʱ�ĺ�����ΪĿ�꺽��
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
		
		//��������һ������
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
		�����㷨��Ҫ���ô������ƣ��ڻ�Ϊ���ٶȻ����⻷Ϊ�ǶȻ���
		�ڻ�����Ҫ�����ǣ���ʱ��Ӧ�ɻ��ķ��б仯��������̬ƫ�Ƶ�
		�ظ��ٶȣ�����״̬�ı仯�ٶȵȵȡ�
		�⻷����Ҫ�����ǣ���֤�ɻ�����̬�ȶ������Ʒɻ��ķ�����̬��
		��С��̬��
		*/
		//####################################################//

//ִ��Ƶ�ʱ�־λ
uint8_t execute_frequency=0;

//�⻷���ǶȻ�
//���룺ң��������̬����
//����������̬����ĽǶ�ֵ
//���������Ҫ�Ĳ���ֵ
//����PID
void Angle_Control_loop(void)
{
	execute_frequency++;

	//4ms����һ��
	if(execute_frequency>= 2)
	{
		//************************PITCH���ֿ���************************//
		//���֣�ң������Ŀ��ֵ��Ϊ����Ŀ�꣬��̬����ĽǶ�ֵ��Ϊ����Ŀ��
		Outside_link.Pitch_ErrNow=PIT-control_target_value.target_Pitch;
		
		//�������֣����������ۼӲ���
		Outside_link.Pitch_increment+=Outside_link.Pitch_ErrNow;
		
		//��������:	ϵ���������ֵ����Ϊ��������
		Outside_link.Pitch_ErrP_out=Outside_link.Pitch_kp*Outside_link.Pitch_ErrNow;
		
		//�����޷����֣������������޷������ƻ��ֱ���
			if(Outside_link.Pitch_increment > Outside_link.Pitch_ErrILim)  	
				Outside_link.Pitch_increment = Outside_link.Pitch_ErrILim;
			else if(Outside_link.Pitch_increment < -(Outside_link.Pitch_ErrILim))		
				Outside_link.Pitch_increment = -(Outside_link.Pitch_ErrILim);
		
		//���ֲ��֣�ϵ�����Ի�����������Ϊ����
		Outside_link.Pitch_ErrI_out=Outside_link.Pitch_ki*Outside_link.Pitch_increment;	
		
		//΢�ֲ���:	����ʷֵ��Ϊ����
		Outside_link.Pitch_ErrD_out=Outside_link.Pitch_kd*(Outside_link.Pitch_ErrNow-Outside_link.Pitch_Errpast);
		
		//������֣������������֡�΢�ֵ����ֵ�����ۼӲ���
		Outside_link.Pitch_CtrOut=Outside_link.Pitch_ErrP_out+Outside_link.Pitch_ErrI_out+Outside_link.Pitch_ErrD_out;
		Outside_link.Pitch_CtrOut=-Outside_link.Pitch_CtrOut;	
			
		//��ʷֵ���֣�������ʱ�̵����			
		Outside_link.Pitch_Errpast=Outside_link.Pitch_ErrNow;	
		//*************************************************************//

			
		//************************Roll���ֿ���************************//
		//���֣�ң������Ŀ��ֵ��Ϊ����Ŀ�꣬��̬����ĽǶ�ֵ��Ϊ����Ŀ��
		Outside_link.Roll_ErrNow=ROLL-control_target_value.target_Roll;
		//�������֣����������ۼӲ���
		Outside_link.Roll_increment+=Outside_link.Roll_ErrNow;
		
		//��������:	ϵ���������ֵ����Ϊ��������
		Outside_link.Roll_ErrP_out=Outside_link.Roll_kp*Outside_link.Roll_ErrNow;
		
		//�����޷����֣������������޷������ƻ��ֱ���
			if(Outside_link.Roll_increment > Outside_link.Roll_ErrILim)  	
				Outside_link.Roll_increment = Outside_link.Roll_ErrILim;
			else if(Outside_link.Roll_increment < -(Outside_link.Roll_ErrILim))		
				Outside_link.Roll_increment = -(Outside_link.Roll_ErrILim);
		
		//���ֲ��֣�ϵ�����Ի�����������Ϊ����
		Outside_link.Roll_ErrI_out=Outside_link.Roll_ki*Outside_link.Roll_increment;	
		
		//΢�ֲ���:	����ʷֵ��Ϊ����
		Outside_link.Roll_ErrD_out=Outside_link.Roll_kd*(Outside_link.Roll_ErrNow-Outside_link.Roll_Errpast);
		
		//������֣������������֡�΢�ֵ����ֵ�����ۼӲ���
		Outside_link.Roll_CtrOut=Outside_link.Roll_ErrP_out+Outside_link.Roll_ErrI_out+Outside_link.Roll_ErrD_out;
		Outside_link.Roll_CtrOut=-Outside_link.Roll_CtrOut;	
			
		//��ʷֵ���֣�������ʱ�̵����			
		Outside_link.Roll_Errpast=Outside_link.Roll_ErrNow;	
		//*************************************************************//

				
		//************************Yaw���ֿ���************************//
		//�ȶ�Yaw����һЩ����,�ڽ����㷨������
		 if((control_target_value.target_Yaw- YAW)>180 || (control_target_value.target_Yaw - YAW)<-180)
		{
       if(control_target_value.target_Yaw>0 && YAW<0)  Outside_link.Yaw_ErrNow= (-180 - YAW) +(control_target_value.target_Yaw - 180);
       if(control_target_value.target_Yaw<0 && YAW>0)  Outside_link.Yaw_ErrNow= (180 - YAW) +(control_target_value.target_Yaw+ 180);
    }
    else  Outside_link.Yaw_ErrNow = control_target_value.target_Yaw - YAW;
		
		//��������:	ϵ���������ֵ����Ϊ��������
		Outside_link.Yaw_ErrP_out=Outside_link.Yaw_kp*Outside_link.Roll_ErrNow;
		
		//΢�ֲ���:	����ʷֵ��Ϊ����
		Outside_link.Yaw_ErrD_out=Outside_link.Yaw_kd*(Outside_link.Yaw_ErrNow-Outside_link.Yaw_Errpast);
		
		//������֣���������΢�ֵ����ֵ�����ۼӲ���
		Outside_link.Yaw_CtrOut=Outside_link.Yaw_ErrP_out+Outside_link.Yaw_ErrD_out;
		
		//��ʷֵ���֣�������ʱ�̵����	
		Outside_link.Yaw_Errpast=Outside_link.Yaw_ErrNow;
		//*************************************************************//
			
    execute_frequency = 0; 	
	}

}



//�ڻ������ٶȻ�
//���룺�⻷�����
//�������������ǵĽ��ٶ�ֵ
//���������ˢ����Ŀ�����
//����PD����
void Angle_speed_control_loop(void)
{
		//************************PITCH���ֿ���************************//
		//���֣��⻷�������ΪĿ�����������ǵĽ��ٶ���Ϊ������
		Inside_link.Pitch_ErrNow=(Outside_link.Pitch_CtrOut-Gyro_sensor.radian[1]*RtA);
		//��������:	ϵ���������ֵ����Ϊ��������
		Inside_link.Pitch_ErrP_out=Inside_link.Pitch_kp	*	Inside_link.Pitch_ErrNow;
		//΢�ֲ���:	�������ǵĽ��ٶ���Ϊ΢�ֻ��ڿ��ƣ���Ӧ���죬���Ƹ�ƽ��
		Inside_link.Pitch_ErrD_out=Inside_link.Pitch_kd*(Inside_link.Pitch_Errpast-	Gyro_sensor.Filter[1]);
		//*************************************************************//

		//************************ROLL���ֿ���*************************//
		//���֣��⻷�������ΪĿ�����������ǵĽ��ٶ���Ϊ������
		Inside_link.Roll_ErrNow=(Outside_link.Roll_CtrOut-Gyro_sensor.radian[0]*RtA);
		//��������:	ϵ���������ֵ����Ϊ��������
		Inside_link.Roll_ErrP_out=Inside_link.Roll_kp	*	Inside_link.Roll_ErrNow;
		//΢�ֲ���:	�������ǵĽ��ٶ���Ϊ΢�ֻ��ڿ��ƣ���Ӧ���죬���Ƹ�ƽ��
		Inside_link.Roll_ErrD_out=Inside_link.Roll_kd*(Inside_link.Roll_Errpast-	Gyro_sensor.Filter[0]);
		//*************************************************************//
	
		//************************Yaw���ֿ���**************************//
		//���֣��⻷�������ΪĿ�����������ǵĽ��ٶ���Ϊ������
		Inside_link.Yaw_ErrNow=(Outside_link.Yaw_CtrOut-Gyro_sensor.radian[2]*RtA);
		//��������:	ϵ���������ֵ����Ϊ��������
		Inside_link.Yaw_ErrP_out=Inside_link.Yaw_kp	*	Inside_link.Yaw_ErrNow;
		//΢�ֲ���:	�������ǵĽ��ٶ���Ϊ΢�ֻ��ڿ��ƣ���Ӧ���죬���Ƹ�ƽ��
		Inside_link.Yaw_ErrD_out=Inside_link.Yaw_kd*(Inside_link.Yaw_Errpast-	Gyro_sensor.Filter[2]);
		//*************************************************************//
	
		//************************������Ʋ���*************************//	
		Inside_link.Pitch_CtrOut	=Inside_link.Pitch_ErrP_out	+	Inside_link.Pitch_ErrD_out;
		Inside_link.Roll_CtrOut		=Inside_link.Roll_ErrP_out	+	Inside_link.Roll_ErrD_out;
		Inside_link.Yaw_CtrOut		=Inside_link.Yaw_ErrP_out		+	Inside_link.Yaw_ErrD_out;
		//*************************************************************//
		
		//*********************������һ�εĽ��ٶ�ֵ********************//
		Inside_link.Pitch_Errpast	=	Gyro_sensor.Filter[1];	
		Inside_link.Roll_Errpast	=	Gyro_sensor.Filter[0];
		Inside_link.Yaw_Errpast		=	Gyro_sensor.Filter[2];			
		//*************************************************************//
}

//===================================================================//







//===================================================================//
		//####################################################//
		/*
			���ݴ���PID���ƣ����������Ҫ�ĵ����������ٸ��ݶ�Ӧ�Ļ��ͣ�����
			��ͬ�������Ȼ��ÿ��������ݸ��Ե����ֵ���ж������ﵽ����Ҫ�Ŀ���
			Ч����
		*/
		//####################################################//

		//���룺�������������š�Pitch��Roll��Yaw��ֵ
		//�������ˢ�����PWMֵ
		//ע�⣺���Ž������Ӧ�Ĳ�����������ƽ������
		
		int16_t Pitch_output,Roll_output,Yaw_output,Throttle;
		int	Throttle_value;		
void Motor_Control(void)
{
		uint8_t count;
	
		//�õ�����Ҫ�Ŀ�����
		Pitch_output	=Inside_link.Pitch_CtrOut;
		Roll_output		=Inside_link.Roll_CtrOut;
		Yaw_output		=Inside_link.Yaw_CtrOut;
		Throttle			=control_target_value.target_Altitude+1600;
		Throttle_value=Throttle;
		
//		Throttle=Throttle/cos(ROLL/RtA)/cos(PIT/RtA);
	
		//������Ŵ��������Ŵ����������ʱ���Ž��ж�Ӧ�Ŀ���
		if(Throttle>2100)
		{
		 Motor_channel[0]=Throttle_value	+	Pitch_output	-	0.414*Roll_output			+	Yaw_output;
		 Motor_channel[1]=Throttle_value									-	Roll_output						-	Yaw_output;
		 Motor_channel[2]=Throttle_value	-	Pitch_output	-	0.414*Roll_output			+	Yaw_output;
		 Motor_channel[3]=Throttle_value	-	Pitch_output	+	0.414*Roll_output			-	Yaw_output;
		 Motor_channel[4]=Throttle_value									+	Roll_output						+	Yaw_output;
		 Motor_channel[5]=Throttle_value	+	Pitch_output	+	0.414*Roll_output			-	Yaw_output;		


		//==============�޷�����===============//
		for(count=0;count<6;count++)
		{
			if(Motor_channel[count]>=Motor_Max_output)
				Motor_channel[count]=Motor_Max_output;
		}			
		//=====================================//	
			
		}
		else
		{
		//�����ŵ����������ʱ�����pitch��roll��ĽǶȻ���������ֵ���������õ�����Ϊ������
		Motor_channel[0]=Motor_channel[1]=Motor_channel[2]=Motor_channel[3]=Motor_channel[4]=Motor_channel[5]=Low_value;
		Outside_link.Pitch_increment	=0;
		Outside_link.Roll_increment		=0;
		}
		

		//������������������Ϊ0.
//		Motor_channel[0]=Motor_channel[1]=Motor_channel[2]=Motor_channel[3]=Motor_channel[4]=Motor_channel[5]=0;
}
//===================================================================//



//===================================================================//
		//####################################################//
		/*
			�ѵ������ֵ�����ͳ�ȥ����ͷΪ��0xCC����β��У���
		*/
		//####################################################//

		//���룺��������ĵ������ֵ
		//�������ˢ�����PWMֵ
void Motor_Send_task(void)
{
		uint8_t check_sum;
		uint8_t seng_length;
	
		//���÷��ͽṹ��
		motor_tc.info.head=0xCC;
		motor_tc.info.Control_value[0]=Motor_channel[0];
		motor_tc.info.Control_value[1]=Motor_channel[1];	
		motor_tc.info.Control_value[2]=Motor_channel[2];	
		motor_tc.info.Control_value[3]=Motor_channel[3];	
		motor_tc.info.Control_value[4]=Motor_channel[4];	
		motor_tc.info.Control_value[5]=Motor_channel[5];	
		
		//����������ݰ���У���
		check_sum=0;
		for(seng_length=0;seng_length<13;seng_length++)
		check_sum+=motor_tc.TX_buffer[seng_length];
		
		motor_tc.info.check=check_sum;

		//�ѿ������ݷ��ͳ�ȥ
		usart_send_data(14,	motor_tc.TX_buffer);
//		usart3_send_data(14,	motor_tc.TX_buffer);
	
}
//===================================================================//


void Motor_Send_stop_task(void)
{
		uint8_t check_sum;
		uint8_t seng_length;
	
		//���÷��ͽṹ��
		motor_tc.info.head=0xCC;
		motor_tc.info.Control_value[0]=2000;
		motor_tc.info.Control_value[1]=2000;	
		motor_tc.info.Control_value[2]=2000;	
		motor_tc.info.Control_value[3]=2000;	
		motor_tc.info.Control_value[4]=2000;	
		motor_tc.info.Control_value[5]=2000;	
		
		//����������ݰ���У���
		check_sum=0;
		for(seng_length=0;seng_length<13;seng_length++)
		check_sum+=motor_tc.TX_buffer[seng_length];
		
		motor_tc.info.check=check_sum;

		//�ѿ������ݷ��ͳ�ȥ
		usart_send_data(14,	motor_tc.TX_buffer);
	
}

