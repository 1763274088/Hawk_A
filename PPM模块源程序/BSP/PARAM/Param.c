#include "include.h"


uint16_t VirtAddVarTab[NumbOfVar] = {0xAA00, 0xAA01, 0xAA02};

static void EE_READ_ACC_OFFSET(void)
{
	EE_ReadVariable(VirtAddVarTab[0], &sensor.acc.quiet.x);
	EE_ReadVariable(VirtAddVarTab[1], &sensor.acc.quiet.y);
	EE_ReadVariable(VirtAddVarTab[2], &sensor.acc.quiet.z);
}

void EE_SAVE_ACC_OFFSET(void)
{
  EE_WriteVariable(VirtAddVarTab[0],sensor.acc.quiet.x);
  EE_WriteVariable(VirtAddVarTab[1],sensor.acc.quiet.y);
	EE_WriteVariable(VirtAddVarTab[2],sensor.acc.quiet.z);
}	


//**************************************************************************
//��������
//**************************************************************************
void	paramLoad(void)
{
	EE_READ_ACC_OFFSET(); //��ȡ���ٶ���ƫ
	Gyro_OFFEST();  //�ɼ���������ƫ
	
	// The data of pitch
	ctrl.pitch.shell.kp = 5;    //5
	ctrl.pitch.shell.ki = 0.01;
	ctrl.pitch.shell.kd = 2;    //2
	
	ctrl.pitch.core.kp = 1.8;   //1.5
	ctrl.pitch.core.kd = 0.22;  //0.16
	
	//The data of roll
	ctrl.roll.shell.kp = 5;
	ctrl.roll.shell.ki = 0.01;
	ctrl.roll.shell.kd = 2;

	ctrl.roll.core.kp = 1.8;
	ctrl.roll.core.kd = 0.22;
	
	//The data of yaw
	ctrl.yaw.shell.kp = 4;
	ctrl.yaw.shell.kd = 0.13;
	
	ctrl.yaw.core.kp = 1.8;
	ctrl.yaw.core.kd = 0.1;
	//limit for the max increment
	ctrl.pitch.shell.increment_max = 200;
	ctrl.roll.shell.increment_max = 200;
	
	ctrl.ctrlRate = 0;

	Rc_Data.pitch_offset = 1530;
	Rc_Data.roll_offset = 1530;
	Rc_Data.yaw_offset = 1530;
}
