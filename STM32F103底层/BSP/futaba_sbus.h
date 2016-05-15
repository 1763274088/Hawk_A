/********************************************************************************
  * @file    futaba_sbus.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    27-12-2015
  * @brief   Header for futaba_sbus.c module
*******************************************************************************/ 
#ifndef _FUTABA_SBUS_H_
#define _FUTABA_SBUS_H_
#include "stm32f10x.h"
#include "stdint.h"
#include "delay.h"

//**********************************************************************************//
//Macro definition area:Define the parameters of the device
/************************************************************************************/
/**************************** START USART Interface pins  ***************************/
#define SBUS_Periph                   	USART3
#define SBUS_CLK                   		 	RCC_APB1Periph_USART3

#define SBUS_USART_PIN               		GPIO_Pin_11                  /* PB.11 */
#define SBUS_USART_GPIO_PORT         		GPIOB                        /* GPIOB */
#define SBUS_USART_GPIO_CLK          		RCC_APB2Periph_GPIOB
#define SBUS_USART_SOURCE            		GPIO_PinSource11

#define SBUS_USART_BaudRate  						100000            	
#define SBUS_USART_WordLength 					USART_WordLength_8b        		
#define SBUS_USART_StopBits 						USART_StopBits_2         		
#define SBUS_USART_Parity								USART_Parity_Even      		
#define SBUS_USART_HardwareFlowControl	USART_HardwareFlowControl_None         		
#define SBUS_USART_Mode									USART_Mode_Rx

#define SBUS_NVIC_IRQChannel  					DMA1_Channel3_IRQn	         	
#define SBUS_NVIC_PreemptionPriority 		0     		
#define SBUS_NVIC_SubPriority						0       		
#define SBUS_NVIC_Cmd										ENABLE



#define SBUS_DMA_CLK              			RCC_AHBPeriph_DMA1
#define SBUS_DMA_PeripheralBaseAddr     USART3->DR 
#define SBUS_DMA_DIR  									DMA_DIR_PeripheralSRC
#define SBUS_DMA_BufferSize							25   
#define SBUS_DMA_PeripheralInc          DMA_PeripheralInc_Disable
#define SBUS_DMA_MemoryInc							DMA_MemoryInc_Enable     
#define SBUS_DMA_PeripheralDataSize			DMA_PeripheralDataSize_Byte  									
#define SBUS_DMA_MemoryDataSize					DMA_MemoryDataSize_Byte							
#define SBUS_DMA_Mode          					DMA_Mode_Circular
#define SBUS_DMA_Priority								DMA_Priority_VeryHigh							     
#define SBUS_DMA_M2M										DMA_M2M_Disable
#define SBUS_DMA_Channel								DMA1_Channel3	
#define SBUS_DMA_IT_Mode								DMA_IT_TC					
/***************************** END USART Interface pins  ****************************/
/************************************************************************************/
/************************** START FM25V01 Selection define  *************************/




/*************************** END FM25V01 Selection define  **************************/
//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//

#pragma pack (1)
typedef struct _SBUS_Receive_Data
{
    uint8_t     startbyte;      										//Start value
    uint16_t    Channel_received_value[16];      		//Channel received value:Only 11 bit
		uint8_t			flags;															//Flag value
		uint8_t			endbyte;														//End value
	
}_SBUS_RECEIVE_DATA,*P_SBUS_RECEIVE_DATA;

#pragma pack () 
extern _SBUS_RECEIVE_DATA	Sbus_Receive_Data;

// //这里是小端模式
// typedef   union _FM25V01_Manufacturer_Product_ID_
// {
//     _FM25V01_MANUFACTURER_PRODUCT_ID FM25V01_Product_ID;
//     uint8_t           ID_buffer[sizeof(_FM25V01_MANUFACTURER_PRODUCT_ID)];
// }FM25V01_Manufacturer_Product_ID;

// extern FM25V01_Manufacturer_Product_ID FM25V01_ID;



extern unsigned char sbus_rx_buffer[25];


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
void USART3_Config(void);



/****************************** END Function definition *****************************/
//**********************************************************************************//




#endif /* _FUTABA_SBUS_H_ */

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
