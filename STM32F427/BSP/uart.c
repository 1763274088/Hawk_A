#include "uart.h"
#include "PPM.h"
#include "communication.h"

//定义发送数据结构体
extern _STATE_DATA_ SD_bf;
extern _STATE_DATA_ RC_bf;

void Uart3_init(void) { 	 		// 串口通讯，到调试器
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//RX
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//TX
	GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);
    /* 使能串口3 */
    USART_Cmd(USART3, ENABLE);
	
	// 使能接收中断通道
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 接收中断开
}

void Usart3_SendData(USART_TypeDef *USARTx, unsigned char TempData) {
    while (!(USARTx->SR & 0x80));
    USARTx->DR = TempData;
}

void usart3_send_data(unsigned char Send_number, unsigned char *Send_data) {
    unsigned char i;
    for (i = 0; i < Send_number; i++)
        Usart3_SendData(USART3, Send_data[i]);
}

//定义UART3的接受缓冲器
uint8_t UART3_RX_buffer[sizeof(RC_bf.control_target)];

//定义UART3的接收第一字节完成标志
uint8_t UART3_receive_head;

//定义UART3的完全接受完成标志
uint8_t UART3_Receive_Data;

//定义接受位
uint8_t UART3_RX_number;

void USART3_IRQHandler(void) { 			// 串口USART1的中断句柄
    //保存串口接收到的数据
    uint8_t receive_value;
	uint8_t Count_number;
	uint8_t check_sum;
    //处理接收中断
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);	//清除中断标志
		// 接收数据
        receive_value = USART_ReceiveData(USART3);
		
		//判断包头
        if (UART3_receive_head) { 		//已经接受过包头
            UART3_RX_buffer[UART3_RX_number] = receive_value; 	//存到BUF里
            UART3_RX_number++;
        }
		else if (USART_ReceiveData(USART3) == 0xDD) { 			//检测包头
			UART3_receive_head = 1;

			UART3_RX_buffer[UART3_RX_number] = receive_value; 	//存到BUF里
		}
        //当接收的帧长度符合时，进行相对应的处理
        //*****************************************//
        if (UART3_RX_number > (sizeof(RC_bf.control_target))) {
            for (Count_number = 0; Count_number < sizeof(RC_bf.control_target)-1; Count_number++) {
                check_sum += UART3_RX_buffer[Count_number];
            }
            if (UART3_RX_buffer[0] == 0xDD && check_sum == UART3_RX_buffer[sizeof(RC_bf.control_target) - 1]) {
                for (Count_number = 0; Count_number < sizeof(RC_bf.control_target); Count_number++)
                    RC_bf.buffer[Count_number] = UART3_RX_buffer[Count_number];
                //接收到数据，更新其标志位
                UART3_Receive_Data = 1;
            }
			UART3_receive_head = 0;
            UART3_RX_number = 0;
        }
        //*****************************************//
    }
}
