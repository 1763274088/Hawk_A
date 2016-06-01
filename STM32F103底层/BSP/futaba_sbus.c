#include "futaba_sbus.h"

extern int mode;

//DMA1的通道3
void USART3_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    /* config USART1 clock */
    RCC_APB1PeriphClockCmd(SBUS_CLK, ENABLE);				//Open peripherals and serial clock
    RCC_APB2PeriphClockCmd(SBUS_USART_GPIO_CLK, ENABLE);    //Open peripherals and serial clock
    RCC_AHBPeriphClockCmd(SBUS_DMA_CLK, ENABLE);			//Open DMA clock

    GPIO_InitStructure.GPIO_Pin = SBUS_USART_PIN;        //Initialization receive pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SBUS_USART_GPIO_PORT, &GPIO_InitStructure);

    /* USART3 mode config */
    USART_InitStructure.USART_BaudRate = SBUS_USART_BaudRate;    //Set the baud rate of 100000
    USART_InitStructure.USART_WordLength = SBUS_USART_WordLength;    //Set to 8 data bits
    USART_InitStructure.USART_StopBits = SBUS_USART_StopBits;    //Setting stop bits to 2
    USART_InitStructure.USART_Parity = SBUS_USART_Parity;    //Set to even parity
    USART_InitStructure.USART_HardwareFlowControl = SBUS_USART_HardwareFlowControl;    //Set to no hardware flow control
    USART_InitStructure.USART_Mode = SBUS_USART_Mode;    //Set to receive mode
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);

    /* Defines the serial port receive interrupt DMA */
    NVIC_InitStructure.NVIC_IRQChannel = SBUS_NVIC_IRQChannel;    //Defined DMA interrupt channels
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SBUS_NVIC_PreemptionPriority;//Define preemption priority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SBUS_NVIC_SubPriority;    //Definition of response priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = SBUS_NVIC_Cmd;    //Open interrupt enable
    NVIC_Init(&NVIC_InitStructure);

    /*设置DMA源：内存地址&串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr =
            (uint32_t) & (SBUS_DMA_PeripheralBaseAddr);
    /*内存地址(要传输的变量的指针)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) sbus_rx_buffer;
    /*方向：从外设到内存*/
    DMA_InitStructure.DMA_DIR = SBUS_DMA_DIR;
    /*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
    DMA_InitStructure.DMA_BufferSize = SBUS_DMA_BufferSize;
    /*外设地址不增*/
    DMA_InitStructure.DMA_PeripheralInc = SBUS_DMA_PeripheralInc;
    /*内存地址自增*/
    DMA_InitStructure.DMA_MemoryInc = SBUS_DMA_MemoryInc;
    /*外设数据单位*/
    DMA_InitStructure.DMA_PeripheralDataSize = SBUS_DMA_PeripheralDataSize;
    /*内存数据单位 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = SBUS_DMA_MemoryDataSize;
    /*DMA模式：循环*/
    DMA_InitStructure.DMA_Mode = SBUS_DMA_Mode;
    /*优先级：非常高*/
    DMA_InitStructure.DMA_Priority = SBUS_DMA_Priority;
    /*禁止内存到内存的传输	*/
    DMA_InitStructure.DMA_M2M = SBUS_DMA_M2M;
    /*配置DMA1的3通道*/
    DMA_Init(SBUS_DMA_Channel, &DMA_InitStructure);
    DMA_Cmd(SBUS_DMA_Channel, ENABLE);
    DMA_ITConfig(SBUS_DMA_Channel, SBUS_DMA_IT_Mode, ENABLE);
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
}
////////////////////////////////////////

void DMA1_Channel3_IRQHandler(void) {
    //判断是否为DMA发送完成中断
    if (DMA_GetFlagStatus(DMA1_FLAG_TC3) == SET) {
        DMA_ClearFlag(DMA1_FLAG_TC3);
        DMA_ClearITPendingBit(DMA1_IT_TC3);
        mode = 1;

        //先接收起始位和结束位，判断起始位是否为0x0f，结束位为0x00.
        Sbus_Receive_Data.startbyte = sbus_rx_buffer[0];
        Sbus_Receive_Data.endbyte = sbus_rx_buffer[24];

        //当满足是正常的一帧数据包时，才拷贝数据，否则，清零
        if (Sbus_Receive_Data.startbyte == 0x0f && Sbus_Receive_Data.endbyte == 0x00) {
            Sbus_Receive_Data.Channel_received_value[ 0] = (sbus_rx_buffer[ 1] | (sbus_rx_buffer[2] << 8)) &0x07ff;                                     //!< Channel 0
            Sbus_Receive_Data.Channel_received_value[ 1] = ((sbus_rx_buffer[ 2] >> 3) | (sbus_rx_buffer[3] << 5)) &0x07ff;                              //!< Channel 1
            Sbus_Receive_Data.Channel_received_value[ 2] = ((sbus_rx_buffer[ 3] >> 6) | (sbus_rx_buffer[4] << 2) | (sbus_rx_buffer[5] << 10)) &0x07ff;  //!< Channel 2 			油门通道
            Sbus_Receive_Data.Channel_received_value[ 3] = ((sbus_rx_buffer[ 5] >> 1) | (sbus_rx_buffer[6] << 7)) &0x07ff;    							//!< Channel 3
            Sbus_Receive_Data.Channel_received_value[ 4] = ((sbus_rx_buffer[ 6] >> 4) | (sbus_rx_buffer[7] << 4)) &0x07ff;    							//!< Channel 4
            Sbus_Receive_Data.Channel_received_value[ 5] = ((sbus_rx_buffer[ 7] >> 7) | (sbus_rx_buffer[8] << 1) | (sbus_rx_buffer[9] << 9)) &0x07ff;	//!< Channel 5
            Sbus_Receive_Data.Channel_received_value[ 6] = ((sbus_rx_buffer[ 9] >> 2) | (sbus_rx_buffer[10] << 6)) &0x07ff;								//!< Channel 6
            Sbus_Receive_Data.Channel_received_value[ 7] = ((sbus_rx_buffer[10] >> 5) | (sbus_rx_buffer[11] << 3)) &0x07ff;								//!< Channel 7
            Sbus_Receive_Data.Channel_received_value[ 8] = (sbus_rx_buffer[12] | (sbus_rx_buffer[13] << 8)) &0x07ff;									//!< Channel 8
            Sbus_Receive_Data.Channel_received_value[ 9] = ((sbus_rx_buffer[13] >> 3) | (sbus_rx_buffer[14] << 5)) &0x07ff;								//!< Channel 9
            Sbus_Receive_Data.Channel_received_value[10] = ((sbus_rx_buffer[14] >> 6) | (sbus_rx_buffer[15] << 2) | (sbus_rx_buffer[16] << 10)) &0x07ff;//!< Channel 10
            Sbus_Receive_Data.Channel_received_value[11] = ((sbus_rx_buffer[16] >> 1) | (sbus_rx_buffer[17] << 7)) &0x07ff;								//!< Channel 11
            Sbus_Receive_Data.Channel_received_value[12] = ((sbus_rx_buffer[17] >> 4) | (sbus_rx_buffer[18] << 4)) &0x07ff;								//!< Channel 12
            Sbus_Receive_Data.Channel_received_value[13] = ((sbus_rx_buffer[18] >> 7) | (sbus_rx_buffer[19] << 1) | (sbus_rx_buffer[20] << 9)) &0x07ff;	//!< Channel 13
            Sbus_Receive_Data.Channel_received_value[14] = ((sbus_rx_buffer[20] >> 2) | (sbus_rx_buffer[21] << 6)) &0x07ff;								//!< Channel 14
            Sbus_Receive_Data.Channel_received_value[15] = ((sbus_rx_buffer[21] >> 5) | (sbus_rx_buffer[22] << 3)) &0x07ff;								//!< Channel 15

            Sbus_Receive_Data.flags = sbus_rx_buffer[23];
        }
        else {
            Sbus_Receive_Data.Channel_received_value[0] = 0;	//!< Channel 0
            Sbus_Receive_Data.Channel_received_value[1] = 0;	//!< Channel 1
            Sbus_Receive_Data.Channel_received_value[2] = 0;	//!< Channel 2
            Sbus_Receive_Data.Channel_received_value[3] = 0;	//!< Channel 3
            Sbus_Receive_Data.Channel_received_value[4] = 0;	//!< Channel 4
            Sbus_Receive_Data.Channel_received_value[5] = 0;	//!< Channel 5
            Sbus_Receive_Data.Channel_received_value[6] = 0;	//!< Channel 6
            Sbus_Receive_Data.Channel_received_value[7] = 0;	//!< Channel 7

            Sbus_Receive_Data.Channel_received_value[8] = 0;	//!< Channel 8
            Sbus_Receive_Data.Channel_received_value[9] = 0;	//!< Channel 9
            Sbus_Receive_Data.Channel_received_value[10] = 0;	//!< Channel 10
            Sbus_Receive_Data.Channel_received_value[11] = 0;	//!< Channel 11
            Sbus_Receive_Data.Channel_received_value[12] = 0;	//!< Channel 12
            Sbus_Receive_Data.Channel_received_value[13] = 0;	//!< Channel 13
            Sbus_Receive_Data.Channel_received_value[14] = 0;	//!< Channel 14
            Sbus_Receive_Data.Channel_received_value[15] = 0;	//!< Channel 15

            Sbus_Receive_Data.flags = 0;

        }
    }
    else
        mode = 0;
}
//////////////////////////////