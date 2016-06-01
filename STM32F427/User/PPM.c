#include "PPM.h"

_F4_to_PPM_ F4_to_PPM_BUF;      //定义F4向PPM发送的数据联合体
_PPM_to_F4_ PPM_to_F4_BUF;      //定义PPM向F4发送的数据联合体

//extern _F4_to_PPM_ F4_to_PPM_BUF;    //引入F4向PPM发送的数据联合体
//extern _PPM_to_F4_ PPM_to_F4_BUF;    //引入PPM向F4发送的数据联合体

extern float ROLL, YAW, PIT;    //引用陀螺仪角度值
extern _MOTOR_CONTROL motor_tc; // 应用主函数中的电机控制结构体

void Get_PPM_Data(void) {       // 获取需要的数据
	uint8_t check_sum = 0;         // 存放校验和
	uint8_t seng_length;        // 计数。
	F4_to_PPM_BUF.DATA.head = 0xDD;             // 装填标志头
	// 将现在的传感器数据*10整数化装入发送F4_to_PPM_BUF
	F4_to_PPM_BUF.DATA.Pit = (int)(PIT * 10);
	F4_to_PPM_BUF.DATA.Rol = (int)(ROLL * 10);
	F4_to_PPM_BUF.DATA.Yaw = (int)(YAW * 10);
	// 将电机控制值装入F4_to_PPM_BUF
	F4_to_PPM_BUF.DATA.motor[0] = motor_tc.info.Control_value[0];
	F4_to_PPM_BUF.DATA.motor[1] = motor_tc.info.Control_value[1];
	F4_to_PPM_BUF.DATA.motor[2] = motor_tc.info.Control_value[2];
	F4_to_PPM_BUF.DATA.motor[3] = motor_tc.info.Control_value[3];
	//求出发送数据包的校验和
	for (seng_length = 0; seng_length < sizeof(F4_to_PPM_BUF.DATA) - 1; seng_length++)
		check_sum += F4_to_PPM_BUF.buffer[seng_length];
	// 存放校验值
	F4_to_PPM_BUF.DATA.check = check_sum;
}
////////////////////////////////////////////////////////////////////

void Send_PPM_Data(void) {      // 发送监控数据
	Get_PPM_Data();     // 准备数据
	usart3_send_data(sizeof(F4_to_PPM_BUF.DATA), F4_to_PPM_BUF.buffer);
}
////////////////////////////////////////////////////////////////////

void Uart3_init(void) {         // 初始化与PPM通讯的串口
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   //RX
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;   //TX
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
////////////////////////////////////////////////////////////////////

uint8_t UART3_receive_head;     //定义UART3的接收第一字节完成标志
uint8_t UART3_Receive_Data;     //定义UART3的完全接受完成标志
uint8_t UART3_RX_number;        //定义接受位
uint8_t UART3_RX_buffer[sizeof(F4_to_PPM_BUF.DATA)];    //定义UART3的接受缓冲器

void USART3_IRQHandler(void) {      // 串口USART3的中断句柄
	//保存串口接收到的数据
	uint8_t receive_value;
	uint8_t Count_number;
	uint8_t check_sum;
	//处理接收中断
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);    //清除中断标志
		// 接收数据
		receive_value = USART_ReceiveData(USART3);

        // 判断第一个数据是否为包头0xAA
        if (receive_value == 0xDD && UART3_receive_head == 0) {
            //标志接收到包头
            UART3_receive_head = 1;
        }
        // 不断接收数据
        if (UART3_receive_head == 1) {
            UART3_RX_buffer[UART3_RX_number] = receive_value;	// 将接收到的数据存入RX缓冲器
            UART3_RX_number++;							// 指向下一个缓冲器
        }

		//当接收的帧长度符合时，进行相对应的处理
		//*****************************************//
		if (UART3_RX_number > (sizeof(PPM_to_F4_BUF.DATA))) {
			for (Count_number = 0; Count_number < sizeof(PPM_to_F4_BUF.DATA) - 1; Count_number++) {
				check_sum += UART3_RX_buffer[Count_number];
			}
			if (UART3_RX_buffer[0] == 0xDD && check_sum == UART3_RX_buffer[sizeof(PPM_to_F4_BUF.DATA) - 1]) {
				for (Count_number = 0; Count_number < sizeof(PPM_to_F4_BUF.DATA); Count_number++)
					PPM_to_F4_BUF.buffer[Count_number] = UART3_RX_buffer[Count_number];
				//接收到数据，更新其标志位
				UART3_Receive_Data = 1;
			}
			UART3_receive_head = 0;
			UART3_RX_number = 0;
		}
		//*****************************************//
	}
}
////////////////////////////////////////////////////////////////////

void Usart3_Send_Char(USART_TypeDef *USARTx, uint8_t TempData) {    //UART3发送一个字节
	while (!(USARTx->SR & 0x80));
	USARTx->DR = TempData;
}
////////////////////////////////////////////////////////////////////

void usart3_send_data(uint8_t Send_number, uint8_t *Send_data) {    //UART3发送N个字节
	unsigned char i;
	for (i = 0; i < Send_number; i++)
		Usart3_Send_Char(USART3, Send_data[i]);
}
////////////////////////////////////////////////////////////////////
