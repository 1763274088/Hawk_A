#include "PPM.h"

_F4_to_PPM_ F4_to_PPM_BUF;    //引入F4向PPM发送的数据联合体
_PPM_to_F4_ PPM_to_F4_BUF;    //引入PPM向F4发送的数据联合体

//extern _F4_to_PPM_ F4_to_PPM_BUF;    //引入F4向PPM发送的数据联合体
//extern _PPM_to_F4_ PPM_to_F4_BUF;    //引入PPM向F4发送的数据联合体

void Get_PPM_Data(void) {       // 获取需要的数据
	uint8_t check_sum = 0;      // 存放校验和
	uint8_t seng_length;        // 计数。
	PPM_to_F4_BUF.DATA.head = 0xDD;             // 装填标志头
    //PPM_to_F4_BUF.DATA.mode_Altiude = xx;     // 这里不装入新值
	//求出发送数据包的校验和并存放校验值
	for (seng_length = 0; seng_length < sizeof(PPM_to_F4_BUF.DATA) - 1; seng_length++)
		check_sum += PPM_to_F4_BUF.buffer[seng_length];
	PPM_to_F4_BUF.DATA.check = check_sum;
}
////////////////////////////////////////////////////////////////////

void Send_PPM_Data(void) {      // 发送监控数据
	Get_PPM_Data();     // 准备数据
	USART1_Send_Data(sizeof(F4_to_PPM_BUF.DATA), F4_to_PPM_BUF.buffer);
}
////////////////////////////////////////////////////////////////////

void USART1_Init(void) {         // 初始化与PPM通讯的串口
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
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//Definition of response priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//Open interrupt enable
    NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////////////////////////////////////

uint8_t USART1_Receive_Head;    //定义USART1的接收第一字节完成标志
uint8_t USART1_Receive_Data;    //定义USART1的完全接受完成标志
uint8_t USART1_RX_number;       //定义接受位
uint8_t USART1_RX_buffer[sizeof(F4_to_PPM_BUF.DATA)];    //定义USART1的接受缓冲器

void USART1_IRQHandler(void) {      // 串口USART1的中断句柄
	//保存串口接收到的数据
	uint8_t receive_value;
	uint8_t Count_number;
	uint8_t check_sum;
	//处理接收中断
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);     //清除中断标志
		receive_value = USART_ReceiveData(USART1);          // 接收数据
        
        // 判断第一个数据是否为包头0xAA
        if (receive_value == 0xDD && USART1_Receive_Head == 0) {
            //标志接收到包头
            USART1_Receive_Head = 1;
        }
        // 不断接收数据
        if (USART1_Receive_Head == 1) {
            USART1_RX_buffer[USART1_RX_number] = receive_value;	// 将接收到的数据存入RX缓冲器
            USART1_RX_number++;							// 指向下一个缓冲器
        }

		//当接收的帧长度符合时，进行相对应的处理
		//*****************************************//
		if (USART1_RX_number > (sizeof(F4_to_PPM_BUF.DATA))) {
			for (Count_number = 0; Count_number < sizeof(F4_to_PPM_BUF.DATA) - 1; Count_number++) {
				check_sum += USART1_RX_buffer[Count_number];
			}
			if (USART1_RX_buffer[0] == 0xDD && check_sum == USART1_RX_buffer[sizeof(F4_to_PPM_BUF.DATA) - 1]) {
				for (Count_number = 0; Count_number < sizeof(F4_to_PPM_BUF.DATA); Count_number++)
					F4_to_PPM_BUF.buffer[Count_number] = USART1_RX_buffer[Count_number];
				//接收到数据，更新其标志位
				USART1_Receive_Data = 1;
			}
			USART1_Receive_Head = 0;
			USART1_RX_number = 0;
		}
		//*****************************************//
	}
}
////////////////////////////////////////////////////////////////////

void USART1_Send_Char(USART_TypeDef *USARTx, uint8_t TempData) {    //USART1发送一个字节
	while (!(USARTx->SR & 0x80));
	USARTx->DR = TempData;
}
////////////////////////////////////////////////////////////////////

void USART1_Send_Data(uint8_t Send_number, uint8_t *Send_data) {    //USART1发送N个字节
	unsigned char i;
	for (i = 0; i < Send_number; i++)
    {
		USART1_Send_Char(USART1, Send_data[i]);
        delay_us(100);
    }
}
////////////////////////////////////////////////////////////////////
