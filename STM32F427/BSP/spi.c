#include "spi.h"

//SPI初始化函数
//输入：void
//输出：void
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
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//设置为主SPI
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//SPI发送接收8位帧结构
		//MPU6000官方文档有错，实际是如下配置		
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													//串行同步时钟的空闲状态为低电平
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;												//串行同步时钟的第一个跳变沿（上升或下降）数据被采样
		
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//内部NSS信号有SSI位控制
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	//波特率预分频值为16
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//数据传输从MSB位开始
		SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRC值计算的多项式
		SPI_Init(SPI1, &SPI_InitStructure);  																//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
		SPI_Cmd(SPI1, ENABLE);
}



//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 


//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI1_ReadWriteByte(SPI_TypeDef* SPIx, uint8_t Writedata)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPIx, Writedata); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据	
 		    
}

