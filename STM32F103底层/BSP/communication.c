#include "communication.h"
#include<string.h>

//接收来自F4控制电机标志位
uint8_t motor_receive_mark=0;
uint8_t LED_give_table=0;

uint8_t check_sum=0;
//接收6个无刷电机的输出值
uint8_t usart2_rx_buffer[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t give_number=0;
uint8_t receive_table_value=0;	

void USART2_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure; 
		DMA_InitTypeDef DMA_InitStructure;

		/* config USART2 clock */	
	  RCC_APB1PeriphClockCmd(TO_Master_CLK,ENABLE);       
    RCC_APB2PeriphClockCmd(TO_Master_USART_GPIO_TX_CLK , ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);																//Open DMA clock		
		USART_DeInit(TO_Master_Periph);  
	
    GPIO_InitStructure.GPIO_Pin = TO_Master_USART_TX_PIN;             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
    GPIO_Init(TO_Master_USART_GPIO_TX_PORT, &GPIO_InitStructure);                  
    GPIO_InitStructure.GPIO_Pin =  TO_Master_USART_RX_PIN;           
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
//     USART_ITConfig(TO_Master_Periph, TO_Master_USART_Interrupt_TXMode, ENABLE);	
		USART_Cmd(TO_Master_Periph, ENABLE); 

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;														//Defined DMA interrupt channels
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;														//Define preemption priority
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;																	//Definition of response priority
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																			//Open interrupt enable
		NVIC_Init(&NVIC_InitStructure);		


		//=======================================================================================//
// 		//接收DMA配置	
// 		/* Defines the serial port receive interrupt DMA */			
// 		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;														//Defined DMA interrupt channels
// 		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;														//Define preemption priority
// 		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;																	//Definition of response priority
// 		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																			//Open interrupt enable
// 		NVIC_Init(&NVIC_InitStructure);		
// 		
// 		/*设置DMA源：内存地址&串口数据寄存器地址*/
//     DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(USART2->DR );									//Set DMA transfer source address   
// 		/*内存地址(要传输的变量的指针)*/
//     DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_rx_buffer;									//Set DMA memory address
// 		/*方向：从外设到内存*/		
//     DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC ;																	//Set the transfer direction
// 		/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
//     DMA_InitStructure.DMA_BufferSize = usart2_rx_length;																//Set the transfer size
// 		/*外设地址不增*/	    
//     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 										//Setting the peripheral address is not incremented
// 		/*内存地址自增*/
//     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;															//Set the memory address increment	
// 		/*外设数据单位*/	
//     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;							//Setting Peripheral DMA transfer unit
// 		/*内存数据单位 8bit*/
//     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;											//Set DMA memory transfer unit	 
// 		/*DMA模式：循环*/
//     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;																		//Set DMA transfer mode	 
// 		/*优先级：非常高*/	
//     DMA_InitStructure.DMA_Priority = DMA_Priority_High;																	//Set DMA transfer priority  
// 		/*禁止内存到内存的传输	*/
//     DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																				//Set DMA memory to prohibit the transmission of memory
// 		/*配置DMA1的3通道*/		   
//     DMA_Init(DMA1_Channel6, &DMA_InitStructure); 	   
// 		
// 		DMA_Cmd (DMA1_Channel6,ENABLE);									
// 		DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);  

// 		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
		//=======================================================================================//


}


void USART2_IRQHandler(void)                	
	{

		uint8_t	number;
	
		uint8_t	receive_value;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
		{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
			
		receive_value	=USART_ReceiveData(USART2);
		if(receive_value==0xCC&&receive_table_value==0)
		{
		receive_table_value=1;
		}
		
		if(receive_table_value==1)
		{
		usart2_rx_buffer[give_number]=receive_value;
		give_number++;
		}
		
		if(give_number>=14)
		{
		if(usart2_rx_buffer[0]==0xCC)	
		{

	 	for(number=0;number<13;number++)
		{
		check_sum+=usart2_rx_buffer[number];
		}
		
		if(check_sum==usart2_rx_buffer[13])
		{
		for(number=0;number<14;number++)
		motor_rc.RX_buffer[number]=usart2_rx_buffer[number];	
			
	// 		//接收到数据，更新其标志位
		  motor_receive_mark=1;
			LED_give_table=1;		
		}
		check_sum=0;
		receive_table_value=0;
		
		}
		give_number=0;
		receive_table_value=0;
		}
		
			
		}

} 	


// //串口2接收DMA中断处理函数
// void DMA1_Channel6_IRQHandler(void)
// {	
// 		uint8_t receive_length;
// 		uint8_t check_sum;
// 	
// 		//判断是否为DMA发送完成中断
// 		if(DMA_GetFlagStatus(DMA1_FLAG_TC6)==SET) 
// 	{ 
// 		DMA_ClearITPendingBit(DMA1_IT_TC6);	

// 				//=================================================================//
// 		/**********************处理接收回来的电机控制值*********************/
// 		//先判断接收回来的数据包头是否等于0xCC
// 		if(usart2_rx_buffer[0]==0xCC)
// 		{

// 			check_sum=0;
// 			for(receive_length=0;receive_length<13;receive_length++)
// 			check_sum+=usart2_rx_buffer[receive_length];

// 			if(check_sum==usart2_rx_buffer[13])
// 			{
// 		//接收到数据，更新其标志位
// 		motor_receive_mark=1;
// 			LED_give_table=1;				
// 				
// 			motor_rc.RX_buffer[0]=usart2_rx_buffer[0];
// 			motor_rc.RX_buffer[1]=usart2_rx_buffer[1];
// 			motor_rc.RX_buffer[2]=usart2_rx_buffer[2];
// 			motor_rc.RX_buffer[3]=usart2_rx_buffer[3];
// 			motor_rc.RX_buffer[4]=usart2_rx_buffer[4];
// 			motor_rc.RX_buffer[5]=usart2_rx_buffer[5];
// 			motor_rc.RX_buffer[6]=usart2_rx_buffer[6];
// 			motor_rc.RX_buffer[7]=usart2_rx_buffer[7];
// 			motor_rc.RX_buffer[8]=usart2_rx_buffer[8];
// 			motor_rc.RX_buffer[9]=usart2_rx_buffer[9];
// 			motor_rc.RX_buffer[10]=usart2_rx_buffer[10];
// 			motor_rc.RX_buffer[11]=usart2_rx_buffer[11];
// 			motor_rc.RX_buffer[12]=usart2_rx_buffer[12];
// 			motor_rc.RX_buffer[13]=usart2_rx_buffer[13];
// 			}
// 		}
// 		
// 		
// 		
// 	}		

// }


void Usart2_SendData(USART_TypeDef* USARTx, unsigned char TempData)
{
    while(!(USARTx->SR&0x80));            
    USARTx->DR = TempData;               
}


void usart_send_data(unsigned char	Send_number,unsigned char *Send_data)
{
		unsigned char i;
		for(i=0;i<Send_number;i++)
		Usart2_SendData(USART2, Send_data[i]);	
}




// //DMA1的通道7
// //串口2发送DMA初始化函数
// void	Usart2_send_dma_init(void)
// {
// 		NVIC_InitTypeDef NVIC_InitStructure; 
// 		DMA_InitTypeDef DMA_InitStructure;

// 	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);																//Open DMA clock	
// 	
// 		/* Defines the serial port receive interrupt DMA */			
// 		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;													//Defined DMA interrupt channels
// 		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;													//Define preemption priority
// 		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;																//Definition of response priority
// 		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																		//Open interrupt enable
// 		NVIC_Init(&NVIC_InitStructure);		

// 		//配置接收DMA
// 		/*设置DMA源：内存地址&串口数据寄存器地址*/
//     DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(USART2->DR );								//Set DMA transfer source address   
// 		/*内存地址(要传输的变量的指针)*/
//     DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart2_tx_buffer;								//Set DMA memory address
// 		/*方向：从内存到外设*/		
//     DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;																//Set the transfer direction
// 		/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
//     DMA_InitStructure.DMA_BufferSize = usart2_tx_length;															//Set the transfer size
// 		/*外设地址不增*/	    
//     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 									//Setting the peripheral address is not incremented
// 		/*内存地址自增*/
//     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;														//Set the memory address increment	
// 		/*外设数据单位*/	
//     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;						//Setting Peripheral DMA transfer unit
// 		/*内存数据单位 8bit*/
//     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;										//Set DMA memory transfer unit	 
// 		/*DMA模式：循环*/
//     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;																	//Set DMA transfer mode	 
// 		/*优先级：中等*/
//     DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;															//Set DMA transfer priority  
// 		/*禁止内存到内存的传输	*/
//     DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																			//Set DMA memory to prohibit the transmission of memory
// 		/*配置DMA1的7通道*/		   
//     DMA_Init(DMA1_Channel7, &DMA_InitStructure); 	   
// 		
// 		DMA_Cmd (DMA1_Channel7,ENABLE);									
// 		DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);  

// 		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);	

// }

// //串口2发送DMA中断处理函数
// void DMA1_Channel7_IRQHandler(void)
// {	
// 		//判断是否为DMA发送完成中断
// 		if(DMA_GetFlagStatus(DMA1_FLAG_TC7)==SET) 
// 	{ 
// 		DMA_ClearFlag(DMA1_FLAG_TC7); 
// 		DMA_ClearITPendingBit(DMA1_IT_TC7);	
// 	}		

// }

