#include "fm25v01.h"


void FM25V01_CS_Init(void)
{
		//Define the corresponding structure
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Enable INT1 GPIO clock */	
		RCC_AHB1PeriphClockCmd(FM25V01_SPI_CS_GPIO_CLK, ENABLE);

		/* Configure GPIO PIN for Lis Chip select */
		GPIO_InitStructure.GPIO_Pin = FM25V01_SPI_CS_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(FM25V01_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

		/* Deselect : Chip Select high */
		GPIO_SetBits(FM25V01_SPI_CS_GPIO_PORT, FM25V01_SPI_CS_PIN);
}  


void FM25V01_Write_Byte(uint8_t Write_data )
{
		SPI2_ReadWriteByte(FM25V01_SPI, Write_data);	
}

uint8_t FM25V01_Read_Byte(void)
{
		uint8_t Read_value;
		Read_value=SPI2_ReadWriteByte(FM25V01_SPI, 0x00);
		return (Read_value);
}



void FM25V01_Init(void)
{

	
} 


void FM25V01_Write_Memory(uint16_t Write_address,uint8_t Write_Data)
{
		FM25V01_Write.write_adderss=Write_address;
		//The chip select line low
		FM25V01_CS_LOW();
	
		//First open Write Enable
		FM25V01_Write_Byte(FM25V01_WREN);	
		FM25V01_CS_HIGH();
//		delay_ms(2);	
		FM25V01_CS_LOW();	
		//Then send the write memory command
		FM25V01_Write_Byte(FM25V01_WRITE);		
		//Then send the high eight of the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Write.adderss_area[1]));	
		//Then send the level of  the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Write.adderss_area[0]));
		//The data is written into the memory area
		FM25V01_Write_Byte((uint8_t)(Write_Data));
	
		//The chip select lines pulled
		FM25V01_CS_HIGH();
}


void FM25V01_multiple_Write_Memory(uint16_t Write_address,uint8_t *Write_Data,uint8_t length)
{
		uint8_t i;
		FM25V01_Write.write_adderss=Write_address;
		//The chip select line low
		FM25V01_CS_LOW();
	
		//First open Write Enable
		FM25V01_Write_Byte(FM25V01_WREN);	
		FM25V01_CS_HIGH();

		FM25V01_CS_LOW();	
		//Then send the write memory command
		FM25V01_Write_Byte(FM25V01_WRITE);		
		//Then send the high eight of the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Write.adderss_area[1]));	
		//Then send the level of  the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Write.adderss_area[0]));
	
		for(i=0;i<length;i++)
		{
		//The data is written into the memory area
		FM25V01_Write_Byte((uint8_t)*(Write_Data++));
		}
	
		//The chip select lines pulled
		FM25V01_CS_HIGH();
}




uint8_t FM25V01_Read_Memory(uint16_t Read_address)
{
		uint8_t Read_Data;
		FM25V01_Read.read_adderss=Read_address;
		//The chip select line low
		FM25V01_CS_LOW();
	
		//Then send the read memory command
		FM25V01_Write_Byte(FM25V01_READ);		
		//Then send the high eight of the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Read.adderss_area_read[1]));	
		//Then send the level of  the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Read.adderss_area_read[0]));
		//Read data from memory
		Read_Data=FM25V01_Read_Byte();
	
		//The chip select lines pulled
		FM25V01_CS_HIGH();
		return Read_Data;
}

void FM25V01_multiple_Read_Memory(uint16_t Read_address,uint8_t *Read_buffer,uint8_t length)
{
		uint8_t i;
		FM25V01_Read.read_adderss=Read_address;
		//The chip select line low
		FM25V01_CS_LOW();
	
		//Then send the read memory command
		FM25V01_Write_Byte(FM25V01_READ);		
		//Then send the high eight of the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Read.adderss_area_read[1]));	
		//Then send the level of  the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Read.adderss_area_read[0]));
	
		for(i=0;i<length;i++)
		{
		//Read data from memory
		*(Read_buffer++)=FM25V01_Read_Byte();
		}
		
		//The chip select lines pulled
		FM25V01_CS_HIGH();
	
}

void FM25V01_Fast_Read_Memory(uint16_t Read_address,uint8_t* Read_Data)
{
		FM25V01_Read.read_adderss=Read_address;
		//The chip select line low
		FM25V01_CS_LOW();
	
		//Then send the read memory command
		FM25V01_Write_Byte(FM25V01_READ);		
		//Then send the high eight of the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Read.adderss_area_read[1]));	
		//Then send the level of  the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Read.adderss_area_read[0]));

		//Read data from memory
		*Read_Data=FM25V01_Read_Byte();
	
		//The chip select lines pulled
		FM25V01_CS_HIGH();
}

void FM25V01_Fast_multiple_Read_Memory(uint16_t Read_address,uint8_t *Read_buffer,uint8_t length)
{
		uint8_t i;
		FM25V01_Read.read_adderss=Read_address;
		//The chip select line low
		FM25V01_CS_LOW();
	
		//Then send the read memory command
		FM25V01_Write_Byte(FM25V01_READ);		
		//Then send the high eight of the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Read.adderss_area_read[1]));	
		//Then send the level of  the address 
		FM25V01_Write_Byte((uint8_t)(FM25V01_Read.adderss_area_read[0]));
	
		for(i=0;i<length;i++)
		{
		//Read data from memory
		*(Read_buffer++)=FM25V01_Read_Byte();
		}
	
		//The chip select lines pulled
		FM25V01_CS_HIGH();
}

void FM25V01_Sleep_Mode_Entry(void)
{
		//The chip select line low
		FM25V01_CS_LOW();	
		//Then send the Sleep Mode command
		FM25V01_Write_Byte(FM25V01_SLEEP);		
		//The chip select lines pulled
		FM25V01_CS_HIGH();
}


void FM25V01_Read_Status_Register(uint8_t* Read_Status_Data)
{
		//The chip select line low
		FM25V01_CS_LOW();
		//Then send the read Status Register command
		FM25V01_Write_Byte(FM25V01_RDSR);		
		//Read Status Register data from memory
		*Read_Status_Data=FM25V01_Read_Byte();
		//The chip select lines pulled
		FM25V01_CS_HIGH();
}


void FM25V01_Read_Device_ID_mask(void)
{
		uint8_t i;
		//The chip select line low
		FM25V01_CS_LOW();
	
		//Then send the read Device ID command
		FM25V01_Write_Byte(FM25V01_RDID);	
		for(i=0;i<9;i++)	
		FM25V01_ID.ID_buffer[i]=FM25V01_Read_Byte();

		//The chip select lines pulled
		FM25V01_CS_HIGH();
}





