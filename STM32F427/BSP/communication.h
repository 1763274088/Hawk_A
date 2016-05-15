/********************************************************************************
  * @file    communication.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    5-2-2016
  * @brief   Header for communication.c module
*******************************************************************************/ 
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"

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
#define TO_Master_USART_Parity								USART_Parity_Even      		
#define TO_Master_USART_HardwareFlowControl		USART_HardwareFlowControl_None         		
#define TO_Master_USART_Mode_RX								USART_Mode_Rx
#define TO_Master_USART_Mode_TX								USART_Mode_Tx

#define TO_Master_USART_Interrupt_RXMode			USART_IT_RXNE
#define TO_Master_USART_Interrupt_TXMode			USART_IT_TXE

#define TO_Master_NVIC_IRQChannel  						DMA1_Channel3_IRQn	         	
#define TO_Master_NVIC_PreemptionPriority 		0     		
#define TO_Master_NVIC_SubPriority						0       		
#define TO_Master_NVIC_Cmd										ENABLE
	
/***************************** END USART Interface pins  ****************************/
/************************************************************************************/
/************************** START Communication Selection define  *************************/




/*************************** END Communication Selection define  **************************/
//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//
#pragma pack (1)
//Ŀ����ƽṹ��
typedef struct	_target
{
			uint8_t		 head;		 //���ݰ�ͷ���̶�Ϊ0xAA
			uint8_t		 head_1;	 //���ݰ�ͷ���̶�Ϊ0xBB
			uint8_t		 mode;		 //ģʽ����	
      uint16_t	 Altiude;	 //���ſ���ֵ	
      float 		 Pitch;    //pitch����ƽǶ�
	    float 		 Roll;  	 //roll����ƽǶ�
	    float 		 Yaw;   	 //yaw����ƽǶ�
			uint8_t		 check;		 //У��ͣ�������ֵ���ۼӺ�
}_target_result;
#pragma pack () 

//����ң������Ŀ��������
typedef   union _REMOTE_RECEIVE_
{
     _target_result control_target;
     uint8_t      receive_buffer[17];
}_REMOTE_RECEIVE;


//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Function definition area:All functions defined in the file
//**********************************************************************************//
/***************************** START Function definition ****************************/
void USART6_Config(void);
void Usart6_SendData(USART_TypeDef* USARTx, unsigned char TempData);
void usart_send_data(unsigned char	Send_number,unsigned char *Send_data);
//void DMA2_Rx_init();
/****************************** END Function definition *****************************/
//**********************************************************************************//




#endif /* _COMMUNICATION_ */

/******************* (C) COPYRIGHT 2016 Huangzhibin*****END OF FILE****/
