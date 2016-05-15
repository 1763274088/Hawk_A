/********************************************************************************
  * @file    spi.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    09-12-2015
  * @brief   Header for spi.c module
*******************************************************************************/ 
#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f4xx.h"
#include "stdint.h"

//***********************************//
/*****	SPI	Pin	definition	*****/
#define SPI1_Peripherals  				RCC_APB2Periph_SPI1
#define SPI1_GPIO_Peripherals			RCC_AHB1Periph_GPIOA
#define SPI1_GPIO_PORT						GPIOA
#define SPI1_GPIO_AF							GPIO_AF_SPI1

#define SPI1_SCLK_SOURCE          GPIO_PinSource5
#define SPI1_MISO_SOURCE     		  GPIO_PinSource6
#define SPI1_MOSI_SOURCE     		  GPIO_PinSource7

#define SPI1_SCLK_PIN       			GPIO_Pin_5
#define SPI1_MISO_PIN       			GPIO_Pin_6
#define SPI1_MOSI_PIN       			GPIO_Pin_7
//***********************************//











/*******************************************************************************
//SPI函数定义集合
*******************************************************************************/

void Spi_Init(void);
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI1_ReadWriteByte(SPI_TypeDef* SPIx, uint8_t Writedata);



#endif /* _SPI_H_ */

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
