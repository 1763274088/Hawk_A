#include "l3gd20h.h "








void L3GD20_Write(uint8_t* Write_Buffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  L3GD20_CS_LOW();
  
  /* Send the Address of the indexed register */
	SPI1_ReadWriteByte(SPI1,WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
		SPI1_ReadWriteByte(SPI1,*Write_Buffer);	
    NumByteToWrite--;
    Write_Buffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  L3GD20_CS_HIGH();
}


void L3GD20_Read(uint8_t* Read_Buffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  L3GD20_CS_LOW();
  
  /* Send the Address of the indexed register */
	SPI1_ReadWriteByte(SPI1,ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to L3GD20 (Slave device) */
    *Read_Buffer = SPI1_ReadWriteByte(SPI1,DUMMY_BYTE);		
    NumByteToRead--;
    Read_Buffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  L3GD20_CS_HIGH();
} 




void L3GD20H_CS_Init(void)
{
		//Define the corresponding structure
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Enable INT1 GPIO clock */	
		RCC_AHB1PeriphClockCmd(L3GD20_SPI_CS_GPIO_CLK, ENABLE);

		/* Configure GPIO PIN for Lis Chip select */
		GPIO_InitStructure.GPIO_Pin = L3GD20_SPI_CS_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(L3GD20_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

		/* Deselect : Chip Select high */
		GPIO_SetBits(L3GD20_SPI_CS_GPIO_PORT, L3GD20_SPI_CS_PIN);
  
		/* Configure GPIO PINs to detect Interrupts */
		GPIO_InitStructure.GPIO_Pin = L3GD20_SPI_INT1_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_Init(L3GD20_SPI_INT1_GPIO_PORT, &GPIO_InitStructure); 
}  


uint8_t L3GD20_GetDataStatus(void)
{
  uint8_t L3GD20_Status; 
	
  /* Read STATUS_REG register */
  L3GD20_Read(&L3GD20_Status, L3GD20_STATUS_REG_ADDR, 1);   
	
  return L3GD20_Status;
}



void L3GD20_FilterCmd(uint8_t HighPassFilterState)
 {
  uint8_t L3GD20_HighPassFilterState;
	 
  /* Read CTRL_REG5 register */
  L3GD20_Read(&L3GD20_HighPassFilterState, L3GD20_CTRL_REG5_ADDR, 1);                  
  L3GD20_HighPassFilterState &= 0xEF;
  L3GD20_HighPassFilterState |= HighPassFilterState;
  /* Write value to MEMS CTRL_REG5 regsister */
  L3GD20_Write(&L3GD20_HighPassFilterState, L3GD20_CTRL_REG5_ADDR, 1);
}



void L3GD20_FilterConfig(L3GD20_FilterConfigTypeDef *L3GD20_FilterStruct) 
{
  uint8_t L3GD20_Modality;
  
  /* Read CTRL_REG2 register */
  L3GD20_Read(&L3GD20_Modality, L3GD20_CTRL_REG2_ADDR, 1);
	
  L3GD20_Modality &= 0xC0;
 
  /* Configure MEMS: mode and cutoff frquency */
  L3GD20_Modality |= (uint8_t) (L3GD20_FilterStruct->HighPassFilter_Mode_Selection |\
                      L3GD20_FilterStruct->HighPassFilter_CutOff_Frequency);                             

  /* Write value to MEMS CTRL_REG2 regsister */
  L3GD20_Write(&L3GD20_Modality, L3GD20_CTRL_REG2_ADDR, 1);
}



void L3GD20_INT1InterruptCmd(uint8_t InterruptState)
{  
  uint8_t L3GD20_INT1_InterruptCmd;
  
  /* Read CTRL_REG3 register */
  L3GD20_Read(&L3GD20_INT1_InterruptCmd, L3GD20_CTRL_REG3_ADDR, 1);
                  
  L3GD20_INT1_InterruptCmd &= 0x7F;	
  L3GD20_INT1_InterruptCmd |= InterruptState;
  
  /* Write value to MEMS CTRL_REG3 regsister */
  L3GD20_Write(&L3GD20_INT1_InterruptCmd, L3GD20_CTRL_REG3_ADDR, 1);
}


void L3GD20_INT2InterruptCmd(uint8_t InterruptState)
{  
  uint8_t L3GD20_INT2_InterruptCmd;
  
  /* Read CTRL_REG3 register */
  L3GD20_Read(&L3GD20_INT2_InterruptCmd, L3GD20_CTRL_REG3_ADDR, 1);
                  
  L3GD20_INT2_InterruptCmd &= 0xF7;	
  L3GD20_INT2_InterruptCmd |= InterruptState;
  
  /* Write value to MEMS CTRL_REG3 regsister */
  L3GD20_Write(&L3GD20_INT2_InterruptCmd, L3GD20_CTRL_REG3_ADDR, 1);
}



void L3GD20_INT1InterruptConfig(L3GD20_InterruptConfigTypeDef *L3GD20_IntConfigStruct)
{
  uint8_t ctrl_cfr = 0x00, ctrl3 = 0x00;
  
  /* Read INT1_CFG register */
  L3GD20_Read(&ctrl_cfr, L3GD20_INT1_CFG_ADDR, 1);
  
  /* Read CTRL_REG3 register */
  L3GD20_Read(&ctrl3, L3GD20_CTRL_REG3_ADDR, 1);
  
  ctrl_cfr &= 0x80;
  
  ctrl3 &= 0xDF;
  
  /* Configure latch Interrupt request and axe interrupts */                   
  ctrl_cfr |= (uint8_t)(L3GD20_IntConfigStruct->Latch_Request| \
                   L3GD20_IntConfigStruct->Interrupt_Axes);
                   
  ctrl3 |= (uint8_t)(L3GD20_IntConfigStruct->Interrupt_ActiveEdge);
  
  /* Write value to MEMS INT1_CFG register */
  L3GD20_Write(&ctrl_cfr, L3GD20_INT1_CFG_ADDR, 1);
  
  /* Write value to MEMS CTRL_REG3 register */
  L3GD20_Write(&ctrl3, L3GD20_CTRL_REG3_ADDR, 1);
}


void L3GD20_RebootCmd(void)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG5 register */
  L3GD20_Read(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
  
  /* Enable or Disable the reboot memory */
  tmpreg |= L3GD20_BOOT_REBOOTMEMORY;
  
  /* Write value to MEMS CTRL_REG5 regsister */
  L3GD20_Write(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
}


void L3GD20_Init(L3GD20_InitTypeDef *L3GD20_InitStruct)
{  
  uint8_t ctrl1 = 0x00, ctrl4 = 0x00;
  
  /* Configure the low level interface ---------------------------------------*/
  L3GD20H_CS_Init();
  
  /* Configure MEMS: data rate, power mode, full scale and axes */
  ctrl1 |= (uint8_t) (L3GD20_InitStruct->Power_Mode | L3GD20_InitStruct->Output_DataRate | \
                    L3GD20_InitStruct->Axes_Enable | L3GD20_InitStruct->Band_Width);
  
  ctrl4 |= (uint8_t) (L3GD20_InitStruct->BlockData_Update | L3GD20_InitStruct->Endianness | \
                    L3GD20_InitStruct->Full_Scale);
                    
  /* Write value to MEMS CTRL_REG1 regsister */
  L3GD20_Write(&ctrl1, L3GD20_CTRL_REG1_ADDR, 1);
  
  /* Write value to MEMS CTRL_REG4 regsister */
  L3GD20_Write(&ctrl4, L3GD20_CTRL_REG4_ADDR, 1);
}







