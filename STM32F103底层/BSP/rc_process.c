#include "rc_process.h"
#include "futaba_sbus.h"
#include "communication.h"

_SBUS_RECEIVE_DATA	Sbus_Receive_Data;

//���ܿ��Ƶ����Ƕ�ֵ
#define Max_angle			25
#define Zoom					10
#define Send_length		sizeof(_target_result)



//ԭʼֵ��������
int16_t deathzoom(int16_t x,int16_t zoom)
{
	int16_t result;
	if(x>0)
	{
		result = x - zoom;
		if(result<0)
		{
			result = 0;
		}
	}
	else
	{
		result = x + zoom;
		if(result>0)
		{
			result = 0;
		}
	}
  return (result);
}

void remote_process(void)
{		
		uint8_t Data_length;
		int16_t	pitch_difference;
		int16_t	roll_difference;		
		
		//����ǰ���ȶ�У��ͽ������㴦��
		Target.control_target.check=0;
	
		//���͵İ�ͷ���̶�Ϊ0xAA
		Target.control_target.head=0xAA;
	
		//���͵İ�ͷ���̶�Ϊ0xBB
		Target.control_target.head_1=0xBB;
	
		//���Ƶ�ģʽ������ģʽ���ֱ�Ϊ��̬���ֶ���������ֹͣģʽ
		if(Sbus_Receive_Data.Channel_received_value[4]==306)
		Target.control_target.mode=0;
		
		if(Sbus_Receive_Data.Channel_received_value[4]==1000)
			Target.control_target.mode=1;
		
		if(Sbus_Receive_Data.Channel_received_value[4]==1694)
			Target.control_target.mode=2;	
	
		//����ֵ��ң��������֮�������ֵ��ԭʼ��ֵ��
		Target.control_target.Altiude=2000-Sbus_Receive_Data.Channel_received_value[2];					//���ſ���ͨ��ֵ;

		//==================================================//
		//���ö�Ӧ�ı�������
		//���Ƶ�Pitch��Ƕ�ֵ�������ڿ��Ƶķ�Χ��
		pitch_difference=(Sbus_Receive_Data.Channel_received_value[1]-1000);								//pitch�����ͨ��ֵ
		Target.control_target.Pitch=Max_angle*(	deathzoom(pitch_difference,Zoom) /690.0f);
		//==================================================//
	
		//==================================================//
		//���ö�Ӧ�ı�������
		//���Ƶ�Roll��Ƕ�ֵ�������ڿ��Ƶķ�Χ��		
		roll_difference=(1000-Sbus_Receive_Data.Channel_received_value[0]);									//roll�����ͨ��ֵ
		Target.control_target.Roll=Max_angle*(	deathzoom(roll_difference,Zoom) /690.0f);
		//==================================================//

		//==================================================//	
		//����YAW��Ĵ���Ƚ����⣬�������ﲻ�����崦��
		//���Ƶ�Yaw��Ƕ�ֵ�������ڿ��Ƶķ�Χ��
		Target.control_target.Yaw=Sbus_Receive_Data.Channel_received_value[3];								//yaw�����ͨ��ֵ
		//==================================================//	
			
		//==================================================//	
		//*****�ر�ע�⣺���ͺ�Ҫ���㣬���߼���ǰҪ���㣬����У������ݽ��Ǵ��!*****//
		//���ݰ���У��ͣ��������ݵ��ۼӺ�	
		for(Data_length=0;Data_length<(Send_length-1);Data_length++)	
		Target.control_target.check+=Target.Send_buffer[Data_length];
		//==================================================//	

		//�Ѵ�����֮���ֵ�����ͳ�ȥ
		usart_send_data(Send_length,Target.Send_buffer);	
		
}



