#include "spi.h"

//SPI��ʼ������
//���룺void
//�����void
void Spi_Init(void)
{
		//Define the corresponding structure
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
	
		//Open the appropriate Clock Peripherals
    RCC_AHB1PeriphClockCmd(SPI1_GPIO_Peripherals, ENABLE);
    RCC_APB2PeriphClockCmd(SPI1_Peripherals, ENABLE);

	    //SPI GPIO Configuration
    GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_SCLK_SOURCE, SPI1_GPIO_AF);
    GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_MISO_SOURCE, SPI1_GPIO_AF);
    GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_MOSI_SOURCE, SPI1_GPIO_AF);
	
    GPIO_InitStructure.GPIO_Pin   = SPI1_SCLK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure);
		GPIO_SetBits(SPI1_GPIO_PORT,SPI1_SCLK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN);
	
		//SPI configuration
		SPI_I2S_DeInit(SPI1);		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//����Ϊ��SPI
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//SPI���ͽ���8λ֡�ṹ
		//MPU6000�ٷ��ĵ��д�ʵ������������		
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;												//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
		
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//�ڲ�NSS�ź���SSIλ����
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	//������Ԥ��ƵֵΪ16
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//���ݴ����MSBλ��ʼ
		SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRCֵ����Ķ���ʽ
		SPI_Init(SPI1, &SPI_InitStructure);  																//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
		SPI_Cmd(SPI1, ENABLE);
}



//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 


//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI1_ReadWriteByte(SPI_TypeDef* SPIx, uint8_t Writedata)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPIx, Writedata); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����	
 		    
}

