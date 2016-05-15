/********************************************************************************
  * @file    spi2.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    25-12-2015
  * @brief   Header for spi2.c module
*******************************************************************************/ 
#ifndef _SPI2_H_
#define _SPI2_H_
#include "stm32f4xx.h"
#include "stdint.h"
//**********************************************************************************//
//Macro definition area:Define the parameters of the device
//**********************************************************************************//
/****************************** START parameters define *****************************/
#define SPI2_Direction_set       SPI_Direction_2Lines_FullDuplex 	/* SPI Direction set   									 */
#define SPI2_Mode_set       		 SPI_Mode_Master 									/* SPI	Mode	set      									 */
#define SPI2_Data_format       	 SPI_DataSize_8b 									/* SPI	Data frame format 							 */
#define SPI2_Unwanted_status   	 SPI_CPOL_Low 										/* SPI	Unwanted	status	of	Level 		 */
#define SPI2_first_state_level   SPI_CPHA_1Edge 									/* SPI	The first transition state level */

#define SPI2_control_Mode   		 SPI_NSS_Soft 										/* SPI	Internal NSS control 						 */
#define SPI2_Baud_value   		 	 SPI_BaudRatePrescaler_8 				/* SPI	Baud rate divider value 				 */
#define SPI2_Start_bit  		 	 	 SPI_FirstBit_MSB 								/* SPI	The first transmission of data 	 */
#define SPI2_CRC_Polynomial   	 7 																/* SPI	CRC computing polynomial 				 */
/******************************* END parameters define ******************************/
/************************************************************************************/
/***************************** START SPI Interface pins  ****************************/
#define SPI_number                    SPI2
#define SPI_CLK                   		RCC_APB1Periph_SPI2

#define SPI2_SCK_PIN               		GPIO_Pin_13                  /* PB.13 */
#define SPI2_SCK_GPIO_PORT         		GPIOB                        /* GPIOB */
#define SPI2_SCK_GPIO_CLK          		RCC_AHB1Periph_GPIOB
#define SPI2_SCK_SOURCE            		GPIO_PinSource13
#define SPI2_SCK_AF                		GPIO_AF_SPI2

#define SPI2_MISO_PIN              		GPIO_Pin_14                  /* PB.14 */
#define SPI2_MISO_GPIO_PORT        		GPIOB                        /* GPIOB */
#define SPI2_MISO_GPIO_CLK         		RCC_AHB1Periph_GPIOB
#define SPI2_MISO_SOURCE           		GPIO_PinSource14
#define SPI2_MISO_AF               		GPIO_AF_SPI2

#define SPI2_MOSI_PIN              		GPIO_Pin_15                  /* PB.15 */
#define SPI2_MOSI_GPIO_PORT        		GPIOB                        /* GPIOB */
#define SPI2_MOSI_GPIO_CLK         		RCC_AHB1Periph_GPIOB
#define SPI2_MOSI_SOURCE           		GPIO_PinSource15
#define SPI2_MOSI_AF               		GPIO_AF_SPI2
/****************************** END SPI Interface pins  *****************************/
//**********************************************************************************//
//**********************************************************************************//

//**********************************************************************************//
//Function definition area:All functions defined in the file
//**********************************************************************************//
/***************************** START Function definition ****************************/
void SPI2_Init(void);
void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI2_ReadWriteByte(SPI_TypeDef* SPIx, uint8_t Writedata);
/****************************** END Function definition *****************************/
//**********************************************************************************//

#endif /* _SPI2_H_ */

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
