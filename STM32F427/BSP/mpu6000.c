#include "mpu6000.h"

//MPU6000Ƭѡ�ܽų�ʼ������
//���룺void
//�����void
void MPU6000_CS_init(void)
{
		//Define the corresponding structure
    GPIO_InitTypeDef GPIO_InitStructure;
		//Open the appropriate Clock Peripherals
    RCC_AHB1PeriphClockCmd(MPU6000_GPIO_Periphera, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = MPU6000_CS_PIN;//PC2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(MPU6000_CS_PORT, &GPIO_InitStructure);//��ʼ��
		GPIO_SetBits(MPU6000_CS_PORT,MPU6000_CS_PIN);
}


static uint8_t MPU6000_ReadData(uint8_t address)
{
		uint8_t receive;
    MPU6000_ENABLE;
	  SPI1_ReadWriteByte(SPI1,(address|0x80));
		receive =  SPI1_ReadWriteByte(SPI1,0xff);		
		MPU6000_DISABLE ;
		return (receive);
}


static void MPU6000_WriteData(uint8_t address,uint8_t data)
{
		MPU6000_ENABLE;
		SPI1_ReadWriteByte(SPI1,(address&0x7f));
		SPI1_ReadWriteByte(SPI1,data);
		MPU6000_DISABLE;
}

static void MPU6000_BrustReadData(uint8_t address,uint8_t *data,uint8_t length)
{
		MPU6000_ENABLE  ; 
	  SPI1_ReadWriteByte(SPI1,(address|0x80));
		while(length--)
		{	
			*data++ =  SPI1_ReadWriteByte(SPI1,0xff);
		}
		MPU6000_DISABLE  ;
}


uint8_t MPU6000_Init(void)
{
	  uint8_t test[10] = { 0 };
		Spi_Init();
		MPU6000_CS_init();
		//����ΪSPIģʽ
		MPU6000_WriteData(MPU_USER_CTRL_REG, 0x10);//resets the signal paths for all sensors, bit0 autumaticaly clear to 0
	  delay_ms(10);
		
		//����Ϊ����ģʽ
	  MPU6000_WriteData(MPU_PWR_MGMT1_REG,0x00);
		delay_ms(10);
    test[0] = MPU6000_ReadData(MPU_PWR_MGMT1_REG);	
//		printf("%d\r\n",test[0]);		
				delay_ms(10);
		if(test[0] != 0x00)
		{
			delay_ms(1);
			return 1;
		}	
		delay_ms(10);
		
		//����ʱ��ԴΪ�����ǵ�Z��
	  MPU6000_WriteData(MPU_PWR_MGMT1_REG,0x03);
		delay_ms(10);
    test[0] = MPU6000_ReadData(MPU_PWR_MGMT1_REG);
//		printf("%d\r\n",test[0]);		
				delay_ms(10);
		if(test[0] != 0x03)
		{
			delay_ms(1);
			return 1;
		}	
		delay_ms(10);		
		
		//��ȡ6000��ID
		test[1] = MPU6000_ReadData(MPU_DEVICE_ID_REG);
//		printf("%d\r\n",test[1]);
				delay_ms(10);
		if(test[1] != 0x68) //? infinite loop;
		{
			delay_ms(1);
			return 1;
		}
	
		//����Ƶ��
		MPU6000_WriteData(MPU_SAMPLE_RATE_REG, 0x07);
		delay_ms(10);
		test[2] = MPU6000_ReadData(MPU_SAMPLE_RATE_REG);
//				printf("%d\r\n",test[2]);
				delay_ms(10);
		if(test[2] != 0x07) //? infinite loop;
		{
			delay_ms(1);
			return 1;
		}
		delay_ms(10);

		//��ͨ�˲���
		MPU6000_WriteData(MPU_CFG_REG, 0x03);
		delay_ms(10);
		test[3] = MPU6000_ReadData(MPU_CFG_REG);
//		printf("%d\r\n",test[3]);
				delay_ms(10);
		if(test[3] != 0x03) //? infinite loop;
		{
			delay_ms(1);
			return 1;
		}

		delay_ms(10);
		/*****************************************************************/
		//�����ǵ���������
		//250d/s  131LSB/d gain = 131*180/pi = 7505.7471162137840348605582806478
		//500d/s  65.5LSB/d 3752.8735581068920174302791403239
		//1000d/s 32.8LSB/d 1876.4367790534460087151395701619
		//2000d/s 16.4LSB/d 938.21838952672300435756978508097
		//0x00 250d/s 0x08 500d/s 0x10 1000d/s 0x11 2000d/s 
		/*****************************************************************/
		MPU6000_WriteData(MPU_GYRO_CFG_REG, 0x10);
		delay_ms(10);
		test[4] = MPU6000_ReadData(MPU_GYRO_CFG_REG);
//		printf("%d\r\n",test[4]);
				delay_ms(10);
		if(test[4] != 0x10) //? infinite loop;
		{
			delay_ms(1);
			return 1;
		}
		delay_ms(10);
		/*****************************************************************/
		//���ٶȼƵ���������
		//2g  8192LSB/g
		//4g  4096LSB/g
		//8g  2048LSB/g
		//16g 1024LSB/g
		//0x00 2g 0x08 4g 0x10 8g 0x11 16g 
		/*****************************************************************/
		MPU6000_WriteData(MPU_ACCEL_CFG_REG, 0x08);
		delay_ms(10);
		test[5] = MPU6000_ReadData(MPU_ACCEL_CFG_REG);
//			printf("%d\r\n",test[5]);
		delay_ms(10);
		if(test[5] != 0x08) //? infinite loop;
		{
			delay_ms(1);
			return 1;
		}
		
		return 0;	
}

static uint8_t MPU6000_GetGyroRaw(int16_t *adc)
{
		u8 buffer[6] = {0};
		MPU6000_BrustReadData(MPU_GYRO_XOUTH_REG,buffer,6);
		//������ȡ�����ǵ�ֵ����ȡ�ķ����ɾ���λ�þ���
		adc[0] = ((((int16_t)buffer[0])<<8) | buffer[1]);
		adc[1] = ((((int16_t)buffer[2])<<8) | buffer[3]);	
		adc[2] = ((((int16_t)buffer[4])<<8) | buffer[5]);
		return 0;	
}
static uint8_t MPU6000_GetAccelRaw(int16_t *adc)
{
		u8 buffer[6] = {0};
		MPU6000_BrustReadData(MPU_ACCEL_XOUTH_REG,buffer,6);
		//������ȡ���ٶȼƵ�ֵ����ȡ�ķ����ɾ���λ�þ���		
		adc[0] = ((((int16_t)buffer[0])<<8) | buffer[1]);	
		adc[1] = ((((int16_t)buffer[2])<<8) | buffer[3]);	
		adc[2] = ((((int16_t)buffer[4])<<8) | buffer[5]);
		return 0;
}

static uint8_t MPU6000_GetTempRaw(int16_t *adc)
{
		u8 buffer[2] = {0};
		MPU6000_BrustReadData(MPU_TEMP_OUTH_REG,buffer,2);
		//��ȡ�¶ȵ�ֵ	
		*adc = ((((int16_t)buffer[0])<<8) | buffer[1]);	
		return 0;
}


//-218.6   9.9   -69.8
//-396   33    -7915
void	MPU6000_Get_Original_Value(int16_t *Gyro_adc,int16_t *Acce_adc,int16_t *Temp_adc)
{
		char i;
		//��ȡ�����ǵ�ADֵ
		MPU6000_GetGyroRaw(Gyro_adc);
		for(i=0;i<40;i++)
		__NOP();
	
		//��ȡ���ٶȼƵ�ADֵ
		MPU6000_GetAccelRaw(Acce_adc);
		for(i=0;i<40;i++)
		__NOP();	
	
		//��ȡ�¶ȴ�������ADֵ
		MPU6000_GetTempRaw(Temp_adc);
		for(i=0;i<40;i++)
		__NOP();	
}

void Gyroscope_Bias(int16_t *Gyro_temp_value)
{
		int32_t Gyro_temporary_X=100,Gyro_temporary_Y=100,Gyro_temporary_Z=100;
		int16_t Gyro_temporary_X_Last=0,Gyro_temporary_Y_Last=0,Gyro_temporary_Z_Last=0;
		int16_t	Gyro_temporary[3]={0};
		uint16_t Count=0;
		char i=0;
		
		
		//��֤��ʱ���ᴦ�ھ�ֹ״̬����ֹ״̬�£�������ÿ�ε�ƫ��ᳬ��100
		while(Gyro_temporary_X>=100 || Gyro_temporary_Y>=100 || Gyro_temporary_Z>=100)
		{
		 Gyro_temporary_X=0;Gyro_temporary_Y=0;Gyro_temporary_Z=0;Count=30;
		 while(Count--)
		 {
				//��ȡ�����ǵ�ֵ
				MPU6000_GetGyroRaw(Gyro_temporary);
			 	for(i=0;i<30;i++)
				__NOP();
		
	      Gyro_temporary_X += absu16(Gyro_temporary[0] - Gyro_temporary_X_Last);
			  Gyro_temporary_Y += absu16(Gyro_temporary[1] - Gyro_temporary_Y_Last);
			  Gyro_temporary_Z += absu16(Gyro_temporary[2] - Gyro_temporary_Z_Last);

			  Gyro_temporary_X_Last = Gyro_temporary[0];
			  Gyro_temporary_Y_Last = Gyro_temporary[1];
			  Gyro_temporary_Z_Last =	Gyro_temporary[2];
	   }	 
		}
	 Gyro_temporary_X=0;Gyro_temporary_X=0;Gyro_temporary_X=0;Count=2000;		

 	 while(Count--)	 //��ѭ�����������Ǳ궨��ǰ���������Ѿ�������ȫ��ֹ״̬
	 {
				//��ȡ�����ǵ�ֵ
				MPU6000_GetGyroRaw(Gyro_temporary);		 
	
				for(i=0;i<30;i++)
				__NOP();
				Gyro_temporary_X += Gyro_temporary[0];
				Gyro_temporary_Y += Gyro_temporary[1];
				Gyro_temporary_Z += Gyro_temporary[2];
     }

		Gyro_temp_value[0] = -(Gyro_temporary_X/2000);
		Gyro_temp_value[1] = (Gyro_temporary_Y/2000);
		Gyro_temp_value[2] = -(Gyro_temporary_Z/2000);			
}


void Accelerometer_Bias(int16_t *Acce_temp_value)
{
		int32_t 	Acce_temporary_X=900,Acce_temporary_Y=900,Acce_temporary_Z=900;
		int16_t 	Acce_reference_X=-200,Acce_reference_Y=33,Acce_reference_Z=-8000;
		int16_t		Acce_temporary[3]={0};
		uint16_t 	Count=0;
		char 			i=0;
		
		
//		//��֤��ʱ���ᴦ�ھ�ֹ״̬����ֹ״̬�£����ٶȼƵ�ƫ��ᳬ���м�λ��ƫ��200
//		while(Acce_temporary_X>=900 || Acce_temporary_Y>=900 || Acce_temporary_Z>=900)
//		{
//		 Acce_temporary_X=0;Acce_temporary_Y=0;Acce_temporary_Z=0;Count=10;
//		 while(Count--)
//		 {
//				//��ȡ���ٶȵ�ֵ
//				MPU6000_GetAccelRaw(Acce_temporary);
//			 	for(i=0;i<30;i++)
//				__NOP();
//		
//	      Acce_temporary_X += absu16(Acce_temporary[0] - Acce_reference_X);
//			  Acce_temporary_Y += absu16(Acce_temporary[1] - Acce_reference_Y);
//			  Acce_temporary_Z += absu16(Acce_temporary[2] - Acce_reference_Z);
//	   }	 
//		}
	 Acce_temporary_X=0;Acce_temporary_Y=0;Acce_temporary_Z=0;Count=2000;		

		while(Count--)	 //��ѭ�����������Ǳ궨��ǰ���������Ѿ�������ȫ��ֹ״̬
		{
				//��ȡ���ٶȵ�ֵ
				MPU6000_GetAccelRaw(Acce_temporary);	 
				for(i=0;i<30;i++)
				__NOP();
		 
				Acce_temporary_X += Acce_temporary[0];
				Acce_temporary_Y += Acce_temporary[1];
				Acce_temporary_Z += Acce_temporary[2];
     }

		Acce_temp_value[0] = -(Acce_temporary_X/2000);
		Acce_temp_value[1] = (Acce_temporary_Y/2000);
		Acce_temp_value[2] = -(Acce_temporary_Z/2000);		
}







