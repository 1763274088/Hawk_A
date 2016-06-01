#include "communication.h"
#include<string.h>

//接收来自F4控制电机标志位
uint8_t motor_receive_mark = 0;
uint8_t LED_give_table = 0;

uint8_t check_sum = 0;
//接收6个无刷电机的输出值
uint8_t usart2_rx_buffer[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t give_number = 0;
uint8_t receive_table_value = 0;

void USART2_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* config USART2 clock */
    RCC_APB1PeriphClockCmd(TO_Master_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(TO_Master_USART_GPIO_TX_CLK, ENABLE);
    USART_DeInit(TO_Master_Periph);

    GPIO_InitStructure.GPIO_Pin = TO_Master_USART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TO_Master_USART_GPIO_TX_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = TO_Master_USART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(TO_Master_USART_GPIO_RX_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = TO_Master_USART_BaudRate;
    USART_InitStructure.USART_WordLength = TO_Master_USART_WordLength;
    USART_InitStructure.USART_StopBits = TO_Master_USART_StopBits;
    USART_InitStructure.USART_Parity = TO_Master_USART_Parity;
    USART_InitStructure.USART_HardwareFlowControl = TO_Master_USART_HardwareFlowControl;
    USART_InitStructure.USART_Mode = TO_Master_USART_Mode_RX | TO_Master_USART_Mode_TX;

    USART_Init(TO_Master_Periph, &USART_InitStructure);
    USART_ITConfig(TO_Master_Periph, TO_Master_USART_Interrupt_RXMode, ENABLE);
    USART_ITConfig(TO_Master_Periph, TO_Master_USART_Interrupt_TXMode, ENABLE);
    USART_Cmd(TO_Master_Periph, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//Defined DMA interrupt channels
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//Define preemption priority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//Definition of response priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//Open interrupt enable
    NVIC_Init(&NVIC_InitStructure);
}

void USART2_IRQHandler(void) {			// UART接收中断
    uint8_t number;
    uint8_t receive_value;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); 		// 清除中断位
        receive_value = USART_ReceiveData(USART2);
		// 包头为0XCC且不处于接收过程中
        if (receive_value == 0xCC && receive_table_value == 0) {
            receive_table_value = 1;
        }
		// 不断接收数据
        if (receive_table_value == 1) {
            usart2_rx_buffer[give_number] = receive_value;
            give_number++;
        }
		// 接收完成
        if (give_number >= 14) {
            if (usart2_rx_buffer[0] == 0xCC) {
				// 包头正确，将所有数据加起来校验
                for (number = 0; number < 13; number++) {
                    check_sum += usart2_rx_buffer[number];
                }
				// 校验值也正确
                if (check_sum == usart2_rx_buffer[13]) {
                    for (number = 0; number < 14; number++)
                        motor_rc.RX_buffer[number] = usart2_rx_buffer[number];
					//接收到数据，更新其标志位
                    motor_receive_mark = 1;
                    LED_give_table = 1;
                }
                check_sum = 0;
                receive_table_value = 0;
            }
            give_number = 0;
            receive_table_value = 0;
        }
    }
}

void Usart2_SendData(USART_TypeDef *USARTx, unsigned char TempData) {
    while (!(USARTx->SR & 0x80));
    USARTx->DR = TempData;
}

void usart_send_data(unsigned char Send_number, unsigned char *Send_data) {
    unsigned char i;
    for (i = 0; i < Send_number; i++)
        Usart2_SendData(USART2, Send_data[i]);
}


