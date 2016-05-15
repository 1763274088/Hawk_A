#include "Attitude_Algorithm.h"
#include "Math_Algorithm.h"
#include "uart.h"
#include<math.h>
//四元数单位向量定义
float Quaternion_q[4] = {1,0,0,0};

//等效重力分量
float gravity_X,gravity_Y,gravity_Z;

//加速度计补偿系数
#define KpDef 3.0f	//1.5f   3
#define KiDef 0.002//0.0005f

float norm_acce;
float norm_quaternion;

#define ANGLE_TO_RADIAN 0.01745329f //*0.01745 = /57.3	角度转弧度
#define IMU_INTEGRAL_LIM  ( 2.0f *ANGLE_TO_RADIAN )

			 float Err_X=0.0f,Err_Y=0.0f,Err_Z=0.0f;
			static float err_Int_x=0.0f,err_Int_y=0.0f,err_Int_z=0.0f;
			 float  GyrX=0.0f, GyrY=0.0f, GyrZ=0.0f;


void AHRS_Update(float Acc_X,float Acc_Y, float Acc_Z, float Gyr_X, float Gyr_Y, float Gyr_Z, float Half_time,float *Rol,float *Pit,float *Yaw )
{

			//计算等效重力的分量，提取重力的等效分量，用作补偿
			gravity_X = 2*(Quaternion_q[1]*Quaternion_q[3] - Quaternion_q[0]*Quaternion_q[2]);
			gravity_Y = 2*(Quaternion_q[0]*Quaternion_q[1] + Quaternion_q[2]*Quaternion_q[3]);
			gravity_Z = 1 - 2*(Quaternion_q[1]*Quaternion_q[1] + Quaternion_q[2]*Quaternion_q[2]);	
	
			//把加速度计做归一化处理
			norm_acce=Rsqrt((square(Acc_X)+square(Acc_Y)+square(Acc_Z)));	

			//判断是否在-90~90度
			if(absolute(Acc_X)<8290&&absolute(Acc_Y)<8290&&absolute(Acc_Z)<8290)
			{			
	
			Acc_X = Acc_X / norm_acce;
			Acc_Y = Acc_Y / norm_acce;
			Acc_Z = Acc_Z / norm_acce;
	
			//向量的差乘得到误差，用于补偿
			Err_X = (Acc_Y*gravity_Z - Acc_Z*gravity_Y);
			Err_Y = (Acc_Z*gravity_X - Acc_X*gravity_Z);
			Err_Z = (Acc_X*gravity_Y - Acc_Y*gravity_X);		
			}
			else
			{
			Err_X=0;
			Err_Y=0;
			
			}
			//误差积分
			err_Int_x = err_Int_x + Err_X * KiDef;
			err_Int_y = err_Int_y + Err_Y * KiDef;
			err_Int_z = err_Int_z + Err_Z * KiDef;
			
			//积分限幅
			err_Int_x = LIMIT(err_Int_x, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );;
			err_Int_y = LIMIT(err_Int_y, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );;
			err_Int_z = LIMIT(err_Int_z, - IMU_INTEGRAL_LIM ,IMU_INTEGRAL_LIM );;
			//得到补偿后的陀螺仪角速度值，用于更新四元数
//			GyrX = Gyr_X + KpDef * VariableParameter(Err_X) * Err_X  +  err_Int_x;
//			GyrY = Gyr_Y + KpDef * VariableParameter(Err_Y) * Err_Y  +  err_Int_y;
//			GyrZ = Gyr_Z + KpDef * VariableParameter(Err_Z) * Err_Z  +  err_Int_z;	
			GyrX = Gyr_X + KpDef  * Err_X  +  err_Int_x;
			GyrY = Gyr_Y + KpDef  * Err_Y  +  err_Int_y;
			GyrZ = Gyr_Z + KpDef  * Err_Z  +  err_Int_z;	
			
			
			//一阶龙格库塔法，更新四元数，得到最新的四元数
			Quaternion_q[0] = Quaternion_q[0] +(-Quaternion_q[1]*GyrX - Quaternion_q[2]*GyrY - Quaternion_q[3]*GyrZ)*Half_time;
			Quaternion_q[1] = Quaternion_q[1] + (Quaternion_q[0]*GyrX + Quaternion_q[2]*GyrZ - Quaternion_q[3]*GyrY)*Half_time;
			Quaternion_q[2] = Quaternion_q[2] + (Quaternion_q[0]*GyrY - Quaternion_q[1]*GyrZ + Quaternion_q[3]*GyrX)*Half_time;
			Quaternion_q[3] = Quaternion_q[3] + (Quaternion_q[0]*GyrZ + Quaternion_q[1]*GyrY - Quaternion_q[2]*GyrX)*Half_time; 

			//四元数归一化处理
			norm_quaternion = 	Rsqrt((square(Quaternion_q[0])+square(Quaternion_q[1])+square(Quaternion_q[2])+square(Quaternion_q[3])));			
			Quaternion_q[0] = Quaternion_q[0] / norm_quaternion;
			Quaternion_q[1] = Quaternion_q[1] / norm_quaternion;
			Quaternion_q[2] = Quaternion_q[2] / norm_quaternion;
			Quaternion_q[3] = Quaternion_q[3] / norm_quaternion;

			//四元数转换为欧拉角
			*Pit  = asin(-2 * Quaternion_q[1] * Quaternion_q[3] + 2 * Quaternion_q[0]* Quaternion_q[2])* 57.3; 					// pitch
			*Rol =  atan2(2 * Quaternion_q[2] * Quaternion_q[3] + 2 * Quaternion_q[0] * Quaternion_q[1], -2 * Quaternion_q[1] * Quaternion_q[1] - 2 * Quaternion_q[2]* Quaternion_q[2] + 1)* 57.3; 	// roll
			*Yaw = -atan2(2 * Quaternion_q[1] * Quaternion_q[2] + 2 * Quaternion_q[0] * Quaternion_q[3], -2 * Quaternion_q[2]*Quaternion_q[2] - 2 * Quaternion_q[3] * Quaternion_q[3] + 1)* 57.3; // yaw  


//			*Rol = fast_atan2(2*(Quaternion_q[0]*Quaternion_q[1] + Quaternion_q[2]*Quaternion_q[3]),1 - 2*(Quaternion_q[1]*Quaternion_q[1] + Quaternion_q[2]*Quaternion_q[2])) *57.3f;
//			*Pit = asin(2*(Quaternion_q[1]*Quaternion_q[3] - Quaternion_q[0]*Quaternion_q[2])) *57.3f;
//			*Yaw = fast_atan2(2*(Quaternion_q[1]*Quaternion_q[2] - Quaternion_q[0]*Quaternion_q[3]), 2*(Quaternion_q[0]*Quaternion_q[0] + Quaternion_q[1]*Quaternion_q[1]) - 1) *57.3f;	
//			*Yaw   = ( - fast_atan2(2*(Quaternion_q[1]*Quaternion_q[2] + Quaternion_q[0]*Quaternion_q[3]),Quaternion_q[0]*Quaternion_q[0] + Quaternion_q[1]*Quaternion_q[1] - Quaternion_q[2]*Quaternion_q[2] - Quaternion_q[3]*Quaternion_q[3]) )* 57.3;

			if(*Rol>90||*Rol<-90)
			{
			if(*Pit>0)
			*Pit=180-*Pit;
			if(*Pit<0)
			*Pit=-(180+*Pit);
			}
}
































