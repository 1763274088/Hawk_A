#include "usart.h"
#include <stdarg.h>
#include "app.h"

// 定义发送数据结构体
_STATE_DATA_ SD_bf;

// 定义接收数据结构体
_STATE_DATA_ RC_bf;

// 保存校验值
uint8_t check_sum = 0;

//保存F4传输过来的数据
uint8_t RX_buffer[sizeof(SD_bf.control_target)];

//接收的个数
uint16_t RX_number = 0;

//处理计数值
uint16_t Count_number = 0;

//接收的标志位处理
uint8_t receive_table_value = 0;

//保存接收到数据标志位
uint8_t give_data_table = 0;

void usart1_config(void) { 					// 串口通讯，到F4
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;        // TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;        // RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;						/*设置波特率为57600*/
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;        	/*设置数据位为8位*/
    USART_InitStructure.USART_StopBits = USART_StopBits_1;            	/*设置停止位为1位*/
    USART_InitStructure.USART_Parity = USART_Parity_No;                	/*无奇偶校验*/
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;        /*没有硬件流控*/
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    	/*发送与接收*/
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//Defined DMA interrupt channels
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//Define preemption priority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//Definition of response priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//Open interrupt enable
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler(void) { 			// 串口USART1的中断句柄
    //保存串口接收到的数据
    uint8_t receive_value;
    //处理接收中断
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);	//清除中断标志
        receive_value = USART_ReceiveData(USART1);
        //判断第一个数据是否为包头0xDD
        if (receive_value == 0xDD && receive_table_value == 0) {
            //标志接收到包头
            receive_table_value = 1;
        }
        if (receive_table_value == 1) {
            RX_buffer[RX_number] = receive_value;
            RX_number++;
        }
        //当接收的帧长度符合时，进行相对应的处理
        //*****************************************//
        if (RX_number > (sizeof(SD_bf.control_target))) {
            for (Count_number = 0; Count_number < sizeof(SD_bf.control_target)-1; Count_number++) {
                check_sum += RX_buffer[Count_number];
            }
            if (RX_buffer[0] == 0xDD && check_sum == RX_buffer[sizeof(SD_bf.control_target) - 1]) {
                for (Count_number = 0; Count_number < sizeof(SD_bf.control_target); Count_number++)
                    RC_bf.buffer[Count_number] = RX_buffer[Count_number];

                //接收到数据，更新其标志位
                give_data_table = 1;
            }
            check_sum = 0;
            receive_table_value = 0;
            RX_number = 0;
        }
        //*****************************************//
    }
}

void Usart1_SendData(USART_TypeDef *USARTx, unsigned char TempData) {
    while (!(USARTx->SR & 0x80));
    USARTx->DR = TempData;
}

void usart1_send_data(unsigned char Send_number, unsigned char *Send_data) {
    unsigned char i;
    for (i = 0; i < Send_number; i++)
        Usart1_SendData(USART1, Send_data[i]);
}
