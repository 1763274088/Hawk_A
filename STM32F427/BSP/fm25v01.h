/********************************************************************************
  * @file    fm25v01.h
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    25-12-2015
  * @brief   Header for fm25v01.c module
  *******************************************************************************/
#ifndef _FM25V01_H_
#define _FM25V01_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "spi2.h"
#include "delay.h"

//**********************************************************************************//
//Macro definition area:Define the parameters of the device
//**********************************************************************************//
/****************************** START REGISTER MAPPING  *****************************/
#define FM25V01_WREN         			((uint8_t)0x06)  /* Set Write Enable Latch	*/
#define FM25V01_WRDI         			((uint8_t)0x04)  /* Write Disable			*/
#define FM25V01_RDSR         			((uint8_t)0x05)  /* Read Status Register	*/
#define FM25V01_WRSR         			((uint8_t)0x01)  /* Write Status Register	*/
#define FM25V01_READ         			((uint8_t)0x03)  /* Read Memory Data		*/
#define FM25V01_FSTRD         			((uint8_t)0x0B)  /* Fast Read Memory Data	*/
#define FM25V01_WRITE					((uint8_t)0x02)  /* Write Memory Data		*/
#define FM25V01_SLEEP          			((uint8_t)0xB9)  /* Enter Sleep Mode		*/
#define FM25V01_RDID         			((uint8_t)0x9F)  /* Read Device ID			*/
#define FM25V01_empty           		((uint8_t)0x00)  /*							*/

/******************************* END REGISTER MAPPING  ******************************/
/************************************************************************************/
/***************************** START SPI Interface pins  ****************************/
#define FM25V01_SPI                       	SPI2
#define FM25V01_SPI_CLK                   	RCC_APB1Periph_SPI2

#define FM25V01_SPI_SCK_PIN               	GPIO_Pin_13                  /* PB.13 */
#define FM25V01_SPI_SCK_GPIO_PORT         	GPIOB                        /* GPIOB */
#define FM25V01_SPI_SCK_GPIO_CLK          	RCC_AHB1Periph_GPIOB
#define FM25V01_SPI_SCK_SOURCE            	GPIO_PinSource13
#define FM25V01_SPI_SCK_AF                	GPIO_AF_SPI2

#define FM25V01_SPI_MISO_PIN              	GPIO_Pin_14                  /* PB.14 */
#define FM25V01_SPI_MISO_GPIO_PORT        	GPIOB                        /* GPIOB */
#define FM25V01_SPI_MISO_GPIO_CLK         	RCC_AHB1Periph_GPIOB
#define FM25V01_SPI_MISO_SOURCE           	GPIO_PinSource14
#define FM25V01_SPI_MISO_AF               	GPIO_AF_SPI2

#define FM25V01_SPI_MOSI_PIN              	GPIO_Pin_15                  /* PB.15 */
#define FM25V01_SPI_MOSI_GPIO_PORT        	GPIOB                        /* GPIOB */
#define FM25V01_SPI_MOSI_GPIO_CLK         	RCC_AHB1Periph_GPIOB
#define FM25V01_SPI_MOSI_SOURCE           	GPIO_PinSource15
#define FM25V01_SPI_MOSI_AF               	GPIO_AF_SPI2

#define FM25V01_SPI_CS_PIN					GPIO_Pin_10                  /* PD.10 */
#define FM25V01_SPI_CS_GPIO_PORT          	GPIOD                        /* GPIOD */
#define FM25V01_SPI_CS_GPIO_CLK           	RCC_AHB1Periph_GPIOD
#define FM25V01_SPI_CS_SOURCE           	GPIO_PinSource10

//FM25V01_CS_PIN
#define FM25V01_CS_LOW()					GPIO_WriteBit(FM25V01_SPI_CS_GPIO_PORT,FM25V01_SPI_CS_PIN,Bit_RESET);
#define FM25V01_CS_HIGH()					GPIO_WriteBit(FM25V01_SPI_CS_GPIO_PORT,FM25V01_SPI_CS_PIN,Bit_SET);
/****************************** END SPI Interface pins  *****************************/
/************************************************************************************/
/************************** START FM25V01 Selection define  *************************/
//Block Memory Write Protection
#define FM25V01_Protect_Memory_None			((uint8_t)0x00)
#define FM25V01_Protect_Memory_Quarter		((uint8_t)0x04)
#define FM25V01_Protect_Memory_Half        	((uint8_t)0x08)
#define FM25V01_Protect_Memory_All        	((uint8_t)0x0C)

//
#define FM25V01_WPEN_ON						((uint8_t)0x80)
#define FM25V01_WPEN_OFF					((uint8_t)0x00)
#define FM25V01_WEL_BIT						((uint8_t)0x02)
/*************************** END FM25V01 Selection define  **************************/
//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//
typedef	union _FM25V01_WRITE_ADDRESS
{
	uint16_t	write_adderss;
	uint8_t  	adderss_area[sizeof(uint16_t)];
} FM25V01_WRITE_ADDRESS;
extern FM25V01_WRITE_ADDRESS FM25V01_Write;

typedef union _FM25V01_READ_ADDRESS
{
	uint16_t 	read_adderss;
	uint8_t  	adderss_area_read[sizeof(uint16_t)];
}FM25V01_READ_ADDRESS;
extern FM25V01_READ_ADDRESS FM25V01_Read;

typedef struct _FM25V01_Manufacturer_Product_ID
{
	uint8_t    	Continuation_code_1;		//Continuation code  0x7F
	uint8_t    	Continuation_code_2;		//Continuation code  0x7F
	uint8_t    	Continuation_code_3;		//Continuation code  0x7F
	uint8_t    	Continuation_code_4;      	//Continuation code  0x7F
	uint8_t    	Continuation_code_5;      	//Continuation code  0x7F
	uint8_t    	Continuation_code_6;		//Continuation code  0x7F
	uint8_t    	JEDEC_assigned_Ramtron;     //JEDEC assigned Ramtron C2h in bank 7 0xC2
	uint8_t    	Device_ID_1;				//Density: 01h=128K, 02h=256K, 03h=512K, 04=1M
	uint8_t    	Device_ID_2;				//00h=FM25V01
} _FM25V01_MANUFACTURER_PRODUCT_ID, *P_FM25V01_MANUFACTURER_PRODUCT_ID;
//这里是小端模式
typedef union _FM25V01_Manufacturer_Product_ID_
{
	_FM25V01_MANUFACTURER_PRODUCT_ID	FM25V01_Product_ID;
	uint8_t           					ID_buffer[sizeof(_FM25V01_MANUFACTURER_PRODUCT_ID)];
}FM25V01_Manufacturer_Product_ID;
extern FM25V01_Manufacturer_Product_ID FM25V01_ID;

///* L3GD20 struct */
//typedef struct
//{
//  uint8_t Power_Mode;                         /* Power-down/Sleep/Normal Mode */
//  uint8_t Output_DataRate;                    /* OUT data rate */
//  uint8_t Axes_Enable;                        /* Axes enable */
//  uint8_t Band_Width;                         /* Bandwidth selection */
//  uint8_t BlockData_Update;                   /* Block Data Update */
//  uint8_t Endianness;                         /* Endian Data selection */
//  uint8_t Full_Scale;                         /* Full Scale selection */
//}L3GD20_InitTypeDef;

//**********************************************************************************//
//**********************************************************************************//

//**********************************************************************************//
//Function definition area:All functions defined in the file
//**********************************************************************************//
/***************************** START Function definition ****************************/
void FM25V01_CS_Init(void);
void FM25V01_Write_Byte(uint8_t Write_data);
uint8_t FM25V01_Read_Byte(void);
void FM25V01_Init(void);
void FM25V01_Write_Memory(uint16_t Write_address, uint8_t Write_Data);
void FM25V01_multiple_Write_Memory(uint16_t Write_address, uint8_t *Write_Data, uint8_t length);
uint8_t FM25V01_Read_Memory(uint16_t Read_address);
void FM25V01_multiple_Read_Memory(uint16_t Read_address, uint8_t *Read_buffer, uint8_t length);
void FM25V01_Fast_Read_Memory(uint16_t Read_address, uint8_t* Read_Data);
void FM25V01_Fast_multiple_Read_Memory(uint16_t Read_address, uint8_t *Read_buffer, uint8_t length);
void FM25V01_Sleep_Mode_Entry(void);
void FM25V01_Read_Status_Register(uint8_t* Read_Status_Data);
void FM25V01_Read_Device_ID_mask(void);
/****************************** END Function definition *****************************/
//**********************************************************************************//

#endif /* _FM25V01_H_*/
/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
