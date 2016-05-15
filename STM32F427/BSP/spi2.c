#include "spi2.h"

//SPI��ʼ������
//���룺void
//�����void
void SPI2_Init(void)
{
		//Define the corresponding structure
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
	
		//Open the appropriate Clock Peripherals
    RCC_AHB1PeriphClockCmd(SPI2_SCK_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(SPI_CLK, ENABLE);

	  //SPI GPIO Configuration
    GPIO_PinAFConfig(SPI2_SCK_GPIO_PORT,  SPI2_SCK_SOURCE,   SPI2_SCK_AF);
    GPIO_PinAFConfig(SPI2_MISO_GPIO_PORT, SPI2_MISO_SOURCE, SPI2_MISO_AF);
    GPIO_PinAFConfig(SPI2_MOSI_GPIO_PORT, SPI2_MOSI_SOURCE, SPI2_MOSI_AF);
	
    GPIO_InitStructure.GPIO_Pin   = SPI2_SCK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStructure);
		GPIO_SetBits(SPI2_SCK_GPIO_PORT,SPI2_SCK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN);
	
		//SPI configuration
		SPI_I2S_DeInit(SPI_number);		
		SPI_InitStructure.SPI_Direction = SPI2_Direction_set;  				//SPI Direction set
		SPI_InitStructure.SPI_Mode      = SPI2_Mode_set;							//SPI	Mode	set
		SPI_InitStructure.SPI_DataSize  = SPI2_Data_format;						//SPI	Data frame format
		SPI_InitStructure.SPI_CPOL      = SPI2_Unwanted_status;				//SPI	Unwanted	status	of	Level
		SPI_InitStructure.SPI_CPHA      = SPI2_first_state_level;			//SPI	The first transition state level 
		
		SPI_InitStructure.SPI_NSS 			= SPI2_control_Mode;					//SPI	Internal NSS control
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI2_Baud_value;		//SPI	Baud rate divider value
		SPI_InitStructure.SPI_FirstBit 	= SPI2_Start_bit;							//SPI	The first transmission of data
		SPI_InitStructure.SPI_CRCPolynomial = SPI2_CRC_Polynomial;		//SPI	CRC computing polynomial
		SPI_Init(SPI_number, &SPI_InitStructure);  										//SPI	Initialization parameter structure
		SPI_Cmd(SPI_number, ENABLE);																	//SPI	Open the SPI peripherals


//		//Define the corresponding structure
//    GPIO_InitTypeDef GPIO_InitStructure;
//    SPI_InitTypeDef  SPI_InitStructure;
//	
//		//Open the appropriate Clock Peripherals
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

//	    //SPI GPIO Configuration
//    GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_SCLK_SOURCE, SPI1_GPIO_AF);
//    GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_MISO_SOURCE, SPI1_GPIO_AF);
//    GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_MOSI_SOURCE, SPI1_GPIO_AF);
//	
//    GPIO_InitStructure.GPIO_Pin   = SPI1_SCLK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
//    GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure);
//		GPIO_SetBits(SPI1_GPIO_PORT,SPI1_SCLK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN);
//	
//		//SPI configuration
//		SPI_I2S_DeInit(SPI2);		
//		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
//		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//����Ϊ��SPI
//		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//SPI���ͽ���8λ֡�ṹ
//		//MPU6000�ٷ��ĵ��д�ʵ������������		
//		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
//		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;												//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
//		
//		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//�ڲ�NSS�ź���SSIλ����
//		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//������Ԥ��ƵֵΪ16
//		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//���ݴ����MSBλ��ʼ
//		SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRCֵ����Ķ���ʽ
//		SPI_Init(SPI2, &SPI_InitStructure);  																//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
//		SPI_Cmd(SPI2, ENABLE);




}



//SPI2�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//Judge of the effectiveness
	SPI2->CR1&=0XFFC7;																							//Set the baud rate
	SPI2->CR1|=SPI_BaudRatePrescaler;																//Set SPI speed
	SPI_Cmd(SPI_number,ENABLE); 																		//Enable SPI
} 


//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI2_ReadWriteByte(SPI_TypeDef* SPIx, uint8_t Writedata)
{		 			 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//Waiting to send area is empty
	
	SPI_I2S_SendData(SPIx, Writedata); 															//By SPI peripheral transmit data
		
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){}//After waiting to receive a data
 
	return SPI_I2S_ReceiveData(SPIx); 															//Return to SPI the most recently received data	 		    
}

