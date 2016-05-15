/********************************************************************************
  * @file    tim.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    5-2-2016
  * @brief   Header for communication.c module
*******************************************************************************/ 
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
#include "stm32f10x.h"
#include "stdint.h"
#include "delay.h"
#include "motor.h"
//**********************************************************************************//
//Macro definition area:Define the parameters of the device
/************************************************************************************/
/**************************** START USART Interface pins  ***************************/
#define TO_Master_Periph                   		USART2
#define TO_Master_CLK                   		 	RCC_APB1Periph_USART2

#define TO_Master_USART_TX_PIN               	GPIO_Pin_2                  /*  PA.2 */
#define TO_Master_USART_GPIO_TX_PORT         	GPIOA                        /* GPIOA */
#define TO_Master_USART_GPIO_TX_CLK          	RCC_APB2Periph_GPIOA
#define TO_Master_USART_TX_SOURCE            	GPIO_PinSource2


#define TO_Master_USART_RX_PIN               	GPIO_Pin_3                  /*  PA.3 */
#define TO_Master_USART_GPIO_RX_PORT         	GPIOA                        /* GPIOA */
#define TO_Master_USART_GPIO_RX_CLK          	RCC_APB2Periph_GPIOA
#define TO_Master_USART_RX_SOURCE            	GPIO_PinSource3


#define TO_Master_USART_BaudRate  						115200            	
#define TO_Master_USART_WordLength 						USART_WordLength_8b        		
#define TO_Master_USART_StopBits 							USART_StopBits_1         		
#define TO_Master_USART_Parity								USART_Parity_No      		
#define TO_Master_USART_HardwareFlowControl		USART_HardwareFlowControl_None         		
#define TO_Master_USART_Mode_RX								USART_Mode_Rx
#define TO_Master_USART_Mode_TX								USART_Mode_Tx

#define TO_Master_USART_Interrupt_RXMode			USART_IT_RXNE
#define TO_Master_USART_Interrupt_TXMode			USART_IT_TXE


#define TO_Master_NVIC_IRQChannel  						DMA1_Channel3_IRQn	         	
#define TO_Master_NVIC_PreemptionPriority 		0     		
#define TO_Master_NVIC_SubPriority						0       		
#define TO_Master_NVIC_Cmd										ENABLE



// #define TO_Master_DMA_CLK              				RCC_AHBPeriph_DMA1
// #define TO_Master_DMA_PeripheralBaseAddr      USART3->DR 
// #define TO_Master_DMA_DIR  										DMA_DIR_PeripheralSRC
// #define TO_Master_DMA_BufferSize							25   
// #define TO_Master_DMA_PeripheralInc           DMA_PeripheralInc_Disable
// #define TO_Master_DMA_MemoryInc								DMA_MemoryInc_Enable     
// #define TO_Master_DMA_PeripheralDataSize			DMA_PeripheralDataSize_Byte  									
// #define TO_Master_DMA_MemoryDataSize					DMA_MemoryDataSize_Byte							
// #define TO_Master_DMA_Mode          					DMA_Mode_Circular
// #define TO_Master_DMA_Priority								DMA_Priority_VeryHigh							     
// #define TO_Master_DMA_M2M											DMA_M2M_Disable
// #define TO_Master_DMA_Channel									DMA1_Channel3	
// #define TO_Master_DMA_IT_Mode									DMA_IT_TC					
/***************************** END USART Interface pins  ****************************/
/************************************************************************************/
/************************** START Communication Selection define  *************************/




/*************************** END Communication Selection define  **************************/
//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//

//接收F4电机控制结构体
#pragma pack (1)
typedef struct _CONTROL_Receive_Data_
{
    uint8_t     head;      										//包头：固定值为0xCC
    uint16_t    Control_value[6];      				//电机控制值数据包
		uint8_t			check;												//校验和：累加和
	
}_CONTROL_Receive_Data;
#pragma pack () 

//这里是小端模式
typedef   union _MOTOR_CONTROL_
{
    _CONTROL_Receive_Data 	info;
    uint8_t          				RX_buffer[sizeof(_CONTROL_Receive_Data)];
}_MOTOR_CONTROL;

extern _MOTOR_CONTROL motor_rc;

//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Function definition area:All functions defined in the file
//**********************************************************************************//
/***************************** START Function definition ****************************/
void USART2_Config(void);
void	Usart2_receive_dma_init(void);
void	Usart2_send_dma_init(void);
void Usart2_SendData(USART_TypeDef* USARTx, unsigned char TempData);
void usart_send_data(unsigned char	Send_number,unsigned char *Send_data);
/****************************** END Function definition *****************************/
//**********************************************************************************//




#endif /* _COMMUNICATION_ */

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
