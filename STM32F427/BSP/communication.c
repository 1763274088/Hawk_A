#include "communication.h"
#include "uart.h"
_REMOTE_RECEIVE	RC_bf;


uint8_t check_sum=0;

//����F1�������������
uint8_t RX_buffer[18]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//���յĸ���
uint8_t RX_number=0;
//�������ֵ
uint8_t Count_number=0;
//���յı�־λ����
uint8_t receive_table_value=0;	




//������յ����ݱ�־λ
uint8_t	give_data_table=0;

void USART6_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure; 
			DMA_InitTypeDef  DMA_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 

	 	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;  //RX
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;   //TX
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

		USART_InitStructure.USART_BaudRate   = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits   = USART_StopBits_1;
		USART_InitStructure.USART_Parity     = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
		USART_Init(USART6, &USART_InitStructure);
		/* ʹ�� USART */
		USART_Cmd(USART6, ENABLE);
		
		// ʹ�ܽ����ж�ͨ�� 
		NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 		

		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

}

/*

		DMA_DeInit(DMA2_Stream2);
		while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE){}												//�ȴ�DMA������ 
		
		// ���� DMA Stream 
		DMA_InitStructure.DMA_Channel = DMA_Channel_5; 														//ͨ��ѡ��
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;					//DMA�����ַ
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)give_buffer;						//DMA �洢��0��ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = 17;																		//���ݴ����� 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//���������ģʽ
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										//�洢������ģʽ
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�������ݳ���:8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//�洢�����ݳ���:8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;														//ʹ��ѭ��ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;											//�е����ȼ�
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;										//�ص�FIFO         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;							//
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;								//�洢��ͻ�����δ���
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;				//����ͻ�����δ���
		DMA_Init(DMA2_Stream2, &DMA_InitStructure);																//��ʼ��DMA Stream
	
		// ʹ�� DMA Stream �ж�ͨ�� 
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 			
					
	  // ʹ�ܽ��մ�������ж� 
		DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE); 				
	  USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);   
		DMA_Cmd(DMA2_Stream2, ENABLE);                      											//����DMA���� 

*/

//}



void USART6_IRQHandler(void)
{
	//���洮�ڽ��յ�������
	uint8_t	receive_value;
	//��������ж�
	if(USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);//����жϱ�־
		receive_value=USART_ReceiveData(USART6);
		
		//�жϵ�һ�������Ƿ�Ϊ��ͷ0xAA
		if(receive_value==0xAA&&receive_table_value==0)
		{
		//��־���յ���ͷ
		receive_table_value=1;
		}

		if(receive_table_value==1)
		{
		RX_buffer[RX_number]=receive_value;
		RX_number++;	
		}
		
		//�����յ�֡���ȷ���ʱ���������Ӧ�Ĵ���
		//*****************************************//
		if(RX_number>=18)
		{	
		for(Count_number=0;Count_number<17;Count_number++)
		{
		check_sum+=RX_buffer[Count_number];
		}

		if(RX_buffer[0]==0xAA&&RX_buffer[1]==0xBB&&check_sum==RX_buffer[17])
		{

		for(Count_number=0;Count_number<18;Count_number++)
		RC_bf.receive_buffer[Count_number]=RX_buffer[Count_number];	
			
		//���յ����ݣ��������־λ
		give_data_table=1;
		}
		check_sum=0;
		receive_table_value=0;
		RX_number=0;
		}
		//*****************************************//		
		
	}
}



void Usart6_SendData(USART_TypeDef* USARTx, unsigned char TempData)
{
    while(!(USARTx->SR&0x80));            
    USARTx->DR = TempData;               
}

void usart_send_data(unsigned char	Send_number,unsigned char *Send_data)
{
		unsigned char i;
		for(i=0;i<Send_number;i++)
		Usart6_SendData(USART6, Send_data[i]);	
}



/*
//У��ͱ����־
uint8_t	check_sum=0;

void DMA2_Stream2_IRQHandler(void)
{
	uint8_t i;

//	  ����DMA_GetITStatus��ʹ���ϴ������⣬������־�ˣ�����޷�ͨ��������ʹ�ú���
//	  DMA_GetFlagStatusȴ���ԣ������¼һ��������⣬�����������

	if (DMA_GetFlagStatus(DMA2_Stream2,DMA_FLAG_TCIF2)==SET)
 	{
 		// ���DMA������ɱ�־ 
 		DMA_ClearITPendingBit(DMA2_Stream2, DMA_FLAG_TCIF2);	
		check_sum=0;
		for(i=0;i<16;i++)
		check_sum+=give_buffer[i];
		//���ж����ݰ�ͷ�Ƿ�����
		if(give_buffer[0]==0xAA&&check_sum==give_buffer[16])
		{
			
		for(i=0;i<17;i++)
		RC_bf.receive_buffer[i]=give_buffer[i];		
		//ȷ���յ���Ч������	
		give_data_table=1;
		}
		

 	}

}
*/







