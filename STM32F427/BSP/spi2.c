#include "spi2.h"

//SPI初始化函数
//输入：void
//输出：void
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
//		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
//		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//设置为主SPI
//		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//SPI发送接收8位帧结构
//		//MPU6000官方文档有错，实际是如下配置		
//		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													//串行同步时钟的空闲状态为低电平
//		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;												//串行同步时钟的第一个跳变沿（上升或下降）数据被采样
//		
//		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//内部NSS信号有SSI位控制
//		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//波特率预分频值为16
//		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//数据传输从MSB位开始
//		SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRC值计算的多项式
//		SPI_Init(SPI2, &SPI_InitStructure);  																//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
//		SPI_Cmd(SPI2, ENABLE);




}



//SPI2速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//Judge of the effectiveness
	SPI2->CR1&=0XFFC7;																							//Set the baud rate
	SPI2->CR1|=SPI_BaudRatePrescaler;																//Set SPI speed
	SPI_Cmd(SPI_number,ENABLE); 																		//Enable SPI
} 


//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI2_ReadWriteByte(SPI_TypeDef* SPIx, uint8_t Writedata)
{		 			 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//Waiting to send area is empty
	
	SPI_I2S_SendData(SPIx, Writedata); 															//By SPI peripheral transmit data
		
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){}//After waiting to receive a data
 
	return SPI_I2S_ReceiveData(SPIx); 															//Return to SPI the most recently received data	 		    
}

