#include "TCA62724.h"


static void TCA62724_IIC_Init(void)
{
		//Define the corresponding structure
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Enable INT1 GPIO clock */	
		RCC_AHB1PeriphClockCmd(TCA62724_IIC_SCK_GPIO_CLK, ENABLE);										//Enable the peripheral clock

		/* Configure GPIO PIN for Lis Chip select */
		GPIO_InitStructure.GPIO_Pin = TCA62724_IIC_SCK_PIN | TCA62724_IIC_SDA_PIN;		//Enable the corresponding pin
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																	//Pin mode
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;																//Pin Output Type
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;														//Pin Speed
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;																	//Level mode																	
		GPIO_Init(TCA62724_IIC_SCK_GPIO_PORT, &GPIO_InitStructure);

		/* Deselect : The clock and data lines pulled */
		TCA62724_IIC_SCL=1;
		TCA62724_IIC_SDA=1;
}  



//����IIC��ʼ�ź�
static void TCA62724_IIC_Start(void)
{
		TCA62724_IIC_SDA_OUT();    
		TCA62724_IIC_SDA=1;	  	  
		TCA62724_IIC_SCL=1;
		delay_us(4);
		TCA62724_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
		delay_us(4);
		TCA62724_IIC_SCL=0;
}


//����IICֹͣ�ź�
static void TCA62724_IIC_Stop(void)
{
		TCA62724_IIC_SDA_OUT();
		TCA62724_IIC_SCL=0;
		TCA62724_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
		delay_us(4);
		TCA62724_IIC_SCL=1; 
		TCA62724_IIC_SDA=1;
		delay_us(4);							   	
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 TCA62724_IIC_Wait_Ack(void)
{
		u8 ucErrTime=0;
		TCA62724_IIC_SDA_IN(); 
		TCA62724_IIC_SDA=1;delay_us(1);	   
		TCA62724_IIC_SCL=1;delay_us(1);	 
	while(TCA62724_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			TCA62724_IIC_Stop();
			return 1;
		}
	}
	TCA62724_IIC_SCL=0;	   
	return 0;  
}



//����ACKӦ��
static void TCA62724_IIC_Ack(void)
{
		TCA62724_IIC_SCL=0;
		TCA62724_IIC_SDA_OUT();
		TCA62724_IIC_SDA=0;
		delay_us(2);
		TCA62724_IIC_SCL=1;
		delay_us(2);
		TCA62724_IIC_SCL=0;
}


//������ACKӦ��		    
static void TCA62724_IIC_NAck(void)
{
		TCA62724_IIC_SCL=0;
		TCA62724_IIC_SDA_OUT();
		TCA62724_IIC_SDA=1;
		delay_us(2);
		TCA62724_IIC_SCL=1;
		delay_us(2);
		TCA62724_IIC_SCL=0;
}


//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void TCA62724_IIC_Send_Byte(u8 Send_data)
{                        
    u8 t;   
	  TCA62724_IIC_SDA_OUT(); 	    
    TCA62724_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
    TCA62724_IIC_SDA=(Send_data&0x80)>>7;
    Send_data<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		TCA62724_IIC_SCL=1;
		delay_us(2); 
		TCA62724_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t TCA62724_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	TCA62724_IIC_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
    TCA62724_IIC_SCL=0; 
    delay_us(2);
		TCA62724_IIC_SCL=1;
    receive<<=1;
    if(TCA62724_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        TCA62724_IIC_NAck();//����nACK
    else
        TCA62724_IIC_Ack(); //����ACK   
    return receive;
}




void TCA62724_Init(void)
{
TCA62724_IIC_Init();	
} 


void TCA62724_Write_Pwm_Data(uint8_t mode,uint8_t PWM0_Data,uint8_t PWM1_Data,uint8_t PWM2_Data)
{
		//Auto-increment OFF
		if(mode==0x00)
		{
		TCA62724_IIC_Start();																		//Send a start signal
			
		TCA62724_IIC_Send_Byte(TCA62724_IIC_write);							//Send write command and address
		TCA62724_IIC_Wait_Ack();																//Wait for Ack
			
		TCA62724_IIC_Send_Byte(TCA62724_IIC_PWM0);							//Send PWM0 address			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack		
		TCA62724_IIC_Send_Byte(PWM0_Data);											//Send PWM data			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack	

		TCA62724_IIC_Send_Byte(TCA62724_IIC_PWM1);							//Send PWM1 address
		TCA62724_IIC_Wait_Ack();																//Wait for Ack
		TCA62724_IIC_Send_Byte(PWM1_Data);											//Send PWM data			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack

		TCA62724_IIC_Send_Byte(TCA62724_IIC_PWM2);							//Send PWM2 address		
		TCA62724_IIC_Wait_Ack();																//Wait for Ack	
		TCA62724_IIC_Send_Byte(PWM2_Data);											//Send PWM data		
		TCA62724_IIC_Wait_Ack();																//Wait for Ack

		TCA62724_IIC_Send_Byte(TCA62724_IIC_ENABLE_SHDN);				//Send ENABLE/SHDN Address			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack	
		TCA62724_IIC_Send_Byte(TCA62724_IIC_ENABLE_SHDN_Open);	//ENABLE_SHDN			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack
		
		TCA62724_IIC_Stop();																		//Sending a stop signal
		delay_ms(10);	
		}
		//Auto-increment ON 
		else
		{
		TCA62724_IIC_Start();																		//Send a start signal
			
		TCA62724_IIC_Send_Byte(TCA62724_IIC_write);							//Send write command and address
		TCA62724_IIC_Wait_Ack();																//Wait for Ack
			
		TCA62724_IIC_Send_Byte(TCA62724_IIC_PWM0_auto);							//Send PWM0 address			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack
			
		TCA62724_IIC_Send_Byte(PWM0_Data);											//Send PWM data			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack	

		TCA62724_IIC_Send_Byte(PWM1_Data);											//Send PWM data			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack

		TCA62724_IIC_Send_Byte(PWM2_Data);											//Send PWM data		
		TCA62724_IIC_Wait_Ack();																//Wait for Ack

		TCA62724_IIC_Send_Byte(TCA62724_IIC_ENABLE_SHDN_Open);	//ENABLE_SHDN			
		TCA62724_IIC_Wait_Ack();																//Wait for Ack			
		
		TCA62724_IIC_Stop();
		delay_ms(10);					
		}

}





void TCA62724_Read_staus_Data(uint8_t* First_byte,uint8_t* Second_byte )
{
		TCA62724_IIC_Start();																		//Send a start signal
			
		TCA62724_IIC_Send_Byte(TCA62724_IIC_read);							//Send read command and address
		TCA62724_IIC_Wait_Ack();																//Wait for Ack
		
		*First_byte=TCA62724_IIC_Read_Byte(0);
		TCA62724_IIC_Wait_Ack();																//Wait for Ack
		*Second_byte=TCA62724_IIC_Read_Byte(0);

	
		TCA62724_IIC_Stop();																		//Sending a stop signal
		delay_ms(10);	
}







