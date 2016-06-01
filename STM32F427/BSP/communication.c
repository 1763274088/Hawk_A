#include "communication.h"
#include "uart.h"
#include "PPM.h"

extern _F4_to_PPM_ F4_to_PPM_BUF;    //引入F4向PPM发送的数据联合体
extern _PPM_to_F4_ PPM_to_F4_BUF;    //引入PPM向F4发送的数据联合体

// 用来存放校验总和
uint8_t check_sum = 0;

// 保存F1传输过来的数据
uint8_t RX_buffer[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 接收的个数
uint8_t RX_number = 0;
// 处理计数值
uint8_t Count_number = 0;
// 接收的标志位处理
uint8_t receive_table_value = 0;

// 保存接收到数据标志位
uint8_t give_data_table = 0;

/* 串口配置
	PC6(TX)-------------SERIAL_FMU_TO_IO
	PC7(RX)-------------SERIAL_IO_TO_FMU				*/
void USART6_Config(void) { 			// 配置与103通信的串口
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
//    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);    	// 定义C区
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);    	// UART通讯
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);        // DMA2时钟使能

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;       	// RX
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      	// 上拉
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       	// 复用
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     	// 推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	// 速度较快
    GPIO_Init(GPIOC, &GPIO_InitStructure);				// 配置PC7

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;          	// TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);            	// 配置PC6

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

    USART_InitStructure.USART_BaudRate = 115200;                                 	// UART速度
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                   	// 8BIT数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                        	// 一位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            	// 不进行奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件控制模式，无
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    				// UART传输接收模式
    USART_Init(USART6, &USART_InitStructure);                                   	// UART总配置。

    USART_Cmd(USART6, ENABLE);
}


// UART6的数据发送函数
void Usart6_SendData(USART_TypeDef *USARTx, unsigned char TempData) { 	
    while (!(USARTx->SR & 0x80)); 			// 当处于忙碌状态时自循环
    USARTx->DR = TempData; 					// 空闲状态重新装载数据
}

// 从数据数组中发送Send_number个数据
void usart6_send_data(unsigned char Send_number, unsigned char *Send_data) {
    unsigned char i;
    for (i = 0; i < Send_number; i++) 			// 发送数据数组的Send_number个
        Usart6_SendData(USART6, Send_data[i]);	// 
	GPIO_ToggleBits(GPIOE, GPIO_Pin_13);
}

// 处理串口接收的数据
/*void USART6_IRQHandler(void) {		// 意味UART6的中断句柄
    //保存串口接收到的数据
    uint8_t receive_value;
    //处理接收中断
    if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET) {
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);		//清除中断标志
		
        receive_value = USART_ReceiveData(USART6);			// 保存接收数据
        // 判断第一个数据是否为包头0xAA
        if (receive_value == 0xAA && receive_table_value == 0) {
            //标志接收到包头
            receive_table_value = 1;
        }
        // 不断接收数据
        if (receive_table_value == 1) {
            RX_buffer[RX_number] = receive_value;	// 将接收到的数据存入RX缓冲器
            RX_number++;							// 指向下一个缓冲器
        }
        // 当接收的帧长度符合时，进行相对应的处理
        //----------------------------------------------
        if (RX_number >= 18) {
			// 将前17位全部加起来
            for (Count_number = 0; Count_number < 17; Count_number++) {
                check_sum += RX_buffer[Count_number];
            }
			// 前两个包头正确，0xAA，0xBB，而且末尾的校验值也正确
            if (RX_buffer[0] == 0xAA && RX_buffer[1] == 0xBB && check_sum == RX_buffer[17]) {
				// 将数据全部移入RC.bf的接收缓冲器
                for (Count_number = 0; Count_number < 18; Count_number++)
                    RC_bf.buffer[Count_number] = RX_buffer[Count_number];
                // 接收到数据，更新其标志位
                give_data_table = 1;
				GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
            }
			// 复位所有寄存器
            check_sum = 0;
            receive_table_value = 0;
            RX_number = 0;
        }
        //----------------------------------------------
    }
}*/
/////////////////////////////////////////////////////////
