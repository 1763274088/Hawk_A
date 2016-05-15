/********************************************************************************
  * @file    lsm303d.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    24-12-2015
  * @brief   Header for lsm303d.c module
*******************************************************************************/ 
#ifndef _LSM303D_H_
#define _LSM303D_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "spi.h"
#include "delay.h"

//**********************************************************************************//
//Macro definition area:Define the parameters of the device
//**********************************************************************************//
/****************************** START REGISTER MAPPING  *****************************/
#define LSM303DLHC_TEMP_OUT_L              0x05  /* Temperature Register magnetic field */
#define LSM303DLHC_TEMP_OUT_H              0x06  /* Temperature Register magnetic field */
#define LSM303DLHC_STATUS_M_H              0x07  /*  STATUS_M  Register  field */

#define LSM303DLHC_OUT_X_L_M               0x08  /* Output Register X magnetic field */
#define LSM303DLHC_OUT_X_H_M               0x09  /* Output Register X magnetic field */
#define LSM303DLHC_OUT_Y_L_M               0x0A  /* Output Register Y magnetic field */
#define LSM303DLHC_OUT_Y_H_M               0x0B  /* Output Register Y magnetic field */
#define LSM303DLHC_OUT_Z_L_M               0x0C  /* Output Register Z magnetic field */
#define LSM303DLHC_OUT_Z_H_M               0x0D  /* Output Register Z magnetic field */ 

#define LSM303DLHC_WHO_AM_I                0x0F  /* WHO_AM_I Register field */

#define LSM303DLHC_INT_CTRL_M							 0x12  /* INT_CTRL_M Register field */
#define LSM303DLHC_INT_SRC_M							 0x13  /* INT_SRC_M Register field */
#define LSM303DLHC_INT_THS_L_M						 0x14  /* INT_THS_L_M  Register field */
#define LSM303DLHC_INT_THS_H_M						 0x15  /* INT_THS_H_M  Register field */

#define LSM303DLHC_OFFSET_X_L_M						 0x16  /* OFFSET_X_L_M  Register field */
#define LSM303DLHC_OFFSET_X_H_M						 0x17  /* OFFSET_X_H_M  Register field */
#define LSM303DLHC_OFFSET_Y_L_M						 0x18  /* OFFSET_Y_L_M  Register field */
#define LSM303DLHC_OFFSET_Y_H_M						 0x19  /* OFFSET_Y_H_M  Register field */
#define LSM303DLHC_OFFSET_Z_L_M						 0x1A  /* OFFSET_Z_L_M  Register field */
#define LSM303DLHC_OFFSET_Z_H_M						 0x1B  /* OFFSET_Z_H_M  Register field */

#define LSM303DLHC_REFERENCE_X						 0x1C  /* Reference value for X Register field */
#define LSM303DLHC_REFERENCE_Y						 0x1D  /* Reference value for Y Register field */
#define LSM303DLHC_REFERENCE_Z						 0x1E  /* Reference value for Z Register field */

/* Acceleration Registers */
#define LSM303DLHC_CTRL0									 0x1F	/* Control register 0 acceleration */
#define LSM303DLHC_CTRL1									 0x20	/* Control register 1 acceleration */
#define LSM303DLHC_CTRL2									 0x21	/* Control register 2 acceleration */
#define LSM303DLHC_CTRL3									 0x22  /* Control register 3 acceleration */
#define LSM303DLHC_CTRL4									 0x23  /* Control register 4 acceleration */
#define LSM303DLHC_CTRL5									 0x24  /* Control register 5 acceleration */
#define LSM303DLHC_CTRL6									 0x25  /* Control register 6 acceleration */
#define LSM303DLHC_CTRL7									 0x26  /* Reference register acceleration */
#define LSM303DLHC_STATUS_A								 0x27  /* Status register acceleration */

#define LSM303DLHC_OUT_X_L_A							 0x28  /* Output Register X acceleration */
#define LSM303DLHC_OUT_X_H_A							 0x29  /* Output Register X acceleration */
#define LSM303DLHC_OUT_Y_L_A							 0x2A  /* Output Register Y acceleration */
#define LSM303DLHC_OUT_Y_H_A							 0x2B  /* Output Register Y acceleration */
#define LSM303DLHC_OUT_Z_L_A							 0x2C  /* Output Register Z acceleration */
#define LSM303DLHC_OUT_Z_H_A							 0x2D  /* Output Register Z acceleration */

#define LSM303DLHC_FIFO_CTRL							 0x2E  /* Fifo control Register acceleration */
#define LSM303DLHC_FIFO_SRC								 0x2F  /* Fifo src Register acceleration */

#define LSM303DLHC_IG_CFG1								 0x30  /* Interrupt 1 configuration Register acceleration */
#define LSM303DLHC_IG_SRC1								 0x31  /* Interrupt 1 source Register acceleration */
#define LSM303DLHC_IG_THS1								 0x32  /* Interrupt 1 Threshold register acceleration */
#define LSM303DLHC_IG_DUR1								 0x33  /* Interrupt 1 DURATION register acceleration */
#define LSM303DLHC_IG_CFG2								 0x34  /* Interrupt 2 configuration Register acceleration */
#define LSM303DLHC_IG_SRC2								 0x35  /* Interrupt 2 source Register acceleration */
#define LSM303DLHC_IG_THS2								 0x36  /* Interrupt 2 Threshold register acceleration */
#define LSM303DLHC_IG_DUR2								 0x37  /* Interrupt 2 DURATION register acceleration */

#define LSM303DLHC_CLICK_CFG							 0x38  /* Click configuration Register acceleration */
#define LSM303DLHC_CLICK_SRC							 0x39  /* Click 2 source Register acceleration */
#define LSM303DLHC_CLICK_THS							 0x3A  /* Click 2 Threshold register acceleration */

#define LSM303DLHC_TIME_LIMIT							 0x3B  /* Time Limit Register acceleration */
#define LSM303DLHC_TIME _LATENCY					 0x3C  /* Time Latency Register acceleration */
#define LSM303DLHC_TIME_WINDOW						 0x3D  /* Time window register acceleration */

#define LSM303DLHC_Act_THS								 0x3E	/* Act_THS Register field */
#define LSM303DLHC_Act_DUR								 0x3F	/* Act_DUR Register field */
/******************************* END REGISTER MAPPING  ******************************/
/************************************************************************************/
/***************************** START SPI Interface pins  ****************************/
#define LSM303DLHC_SPI                       SPI1
#define LSM303DLHC_SPI_CLK                   RCC_APB2Periph_SPI1

#define LSM303DLHC_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define LSM303DLHC_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define LSM303DLHC_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define LSM303DLHC_SPI_SCK_SOURCE            GPIO_PinSource5
#define LSM303DLHC_SPI_SCK_AF                

#define LSM303DLHC_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.6 */
#define LSM303DLHC_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define LSM303DLHC_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define LSM303DLHC_SPI_MISO_SOURCE           GPIO_PinSource6
#define LSM303DLHC_SPI_MISO_AF               

#define LSM303DLHC_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.7 */
#define LSM303DLHC_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define LSM303DLHC_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define LSM303DLHC_SPI_MOSI_SOURCE           GPIO_PinSource7
#define LSM303DLHC_SPI_MOSI_AF               

#define LSM303DLHC_SPI_CS_PIN                GPIO_Pin_15                  /* PC.15 */
#define LSM303DLHC_SPI_CS_GPIO_PORT          GPIOC                       /* GPIOC */
#define LSM303DLHC_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOC

#define LSM303DLHC_SPI_INT1_PIN              GPIO_Pin_4                  /* PB.04 */
#define LSM303DLHC_SPI_INT1_GPIO_PORT        GPIOB                       /* GPIOB */
#define LSM303DLHC_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define LSM303DLHC_SPI_INT1_EXTI_LINE        EXTI_Line4
#define LSM303DLHC_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOB
#define LSM303DLHC_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource4
#define LSM303DLHC_SPI_INT1_EXTI_IRQn        EXTI4_IRQn 

#define LSM303DLHC_SPI_INT2_PIN              GPIO_Pin_1                  /* PB.01 */
#define LSM303DLHC_SPI_INT2_GPIO_PORT        GPIOB                       /* GPIOB */
#define LSM303DLHC_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define LSM303DLHC_SPI_INT2_EXTI_LINE        EXTI_Line1
#define LSM303DLHC_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOB
#define LSM303DLHC_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
#define LSM303DLHC_SPI_INT2_EXTI_IRQn        EXTI1_IRQn 
/****************************** END SPI Interface pins  *****************************/
/************************************************************************************/
/************************* START LSM303DLHC Selection define ************************/
//Read/Write command
#define READWRITE_CMD              				 ((uint8_t)0x80) 

//Multiple byte read/write command
#define MULTIPLEBYTE_CMD           				 ((uint8_t)0x40)

//Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device
#define DUMMY_BYTE                 				 ((uint8_t)0x00)

//WHO_AM_I 
#define WHO_I_AM													 ((uint8_t)0x49)						

//Acc_Power_Mode_selection 
#define LSM303DLHC_NORMAL_MODE            ((uint8_t)0x00)
#define LSM303DLHC_LOWPOWER_MODE          ((uint8_t)0x08)

//Acc_OutPut_DataRate_Selection 
#define LSM303DLHC_ODR_1_HZ                ((uint8_t)0x10)  /*!< Output Data Rate = 1 Hz */
#define LSM303DLHC_ODR_10_HZ               ((uint8_t)0x20)  /*!< Output Data Rate = 10 Hz */
#define LSM303DLHC_ODR_25_HZ               ((uint8_t)0x30)  /*!< Output Data Rate = 25 Hz */
#define LSM303DLHC_ODR_50_HZ               ((uint8_t)0x40)  /*!< Output Data Rate = 50 Hz */
#define LSM303DLHC_ODR_100_HZ              ((uint8_t)0x50)  /*!< Output Data Rate = 100 Hz */
#define LSM303DLHC_ODR_200_HZ              ((uint8_t)0x60)  /*!< Output Data Rate = 200 Hz */
#define LSM303DLHC_ODR_400_HZ              ((uint8_t)0x70)  /*!< Output Data Rate = 400 Hz */
#define LSM303DLHC_ODR_1620_HZ_LP          ((uint8_t)0x80)  /*!< Output Data Rate = 1620 Hz only in Low Power Mode */
#define LSM303DLHC_ODR_1344_HZ             ((uint8_t)0x90)  /*!< Output Data Rate = 1344 Hz in Normal mode and 5376 Hz in Low Power Mode */

//Acc_Axes_Selection 
#define LSM303DLHC_X_ENABLE                ((uint8_t)0x01)
#define LSM303DLHC_Y_ENABLE                ((uint8_t)0x02)
#define LSM303DLHC_Z_ENABLE                ((uint8_t)0x04)
#define LSM303DLHC_AXES_ENABLE             ((uint8_t)0x07)
#define LSM303DLHC_AXES_DISABLE            ((uint8_t)0x00)

//Acc_High_Resolution
#define LSM303DLHC_HR_ENABLE               ((uint8_t)0x08)
#define LSM303DLHC_HR_DISABLE              ((uint8_t)0x00)

//Acc_Full_Scale_Selection
#define LSM303DLHC_FULLSCALE_2G            ((uint8_t)0x00)  /*!< 2 g */
#define LSM303DLHC_FULLSCALE_4G            ((uint8_t)0x10)  /*!< 3 g */
#define LSM303DLHC_FULLSCALE_8G            ((uint8_t)0x20)  /*!< 8 g */
#define LSM303DLHC_FULLSCALE_16G           ((uint8_t)0x30)  /*!< 16g */

//Acc_Block_Data_Update  
#define LSM303DLHC_BlockUpdate_Continous   ((uint8_t)0x00) /*!< Continuos Update */
#define LSM303DLHC_BlockUpdate_Single      ((uint8_t)0x80) /*!< Single Update: output registers not updated until MSB and LSB reading */

//Acc_Endian_Data_selection  
#define LSM303DLHC_BLE_LSB                 ((uint8_t)0x00) /*!< Little Endian: data LSB @ lower address */
#define LSM303DLHC_BLE_MSB	           ((uint8_t)0x40) /*!< Big Endian: data MSB @ lower address */

//Acc_Boot_Mode_selection  
#define LSM303DLHC_BOOT_NORMALMODE         ((uint8_t)0x00)
#define LSM303DLHC_BOOT_REBOOTMEMORY       ((uint8_t)0x80)
 
//Acc_High_Pass_Filter_Mode     
#define LSM303DLHC_HPM_NORMAL_MODE_RES     ((uint8_t)0x00)
#define LSM303DLHC_HPM_REF_SIGNAL          ((uint8_t)0x40)
#define LSM303DLHC_HPM_NORMAL_MODE         ((uint8_t)0x80)
#define LSM303DLHC_HPM_AUTORESET_INT       ((uint8_t)0xC0)

//Acc_High_Pass_CUT OFF_Frequency    
#define LSM303DLHC_HPFCF_8                 ((uint8_t)0x00)
#define LSM303DLHC_HPFCF_16                ((uint8_t)0x10)
#define LSM303DLHC_HPFCF_32                ((uint8_t)0x20)
#define LSM303DLHC_HPFCF_64                ((uint8_t)0x30)

//Acc_High_Pass_Filter_status     
#define LSM303DLHC_HIGHPASSFILTER_DISABLE  ((uint8_t)0x00)
#define LSM303DLHC_HIGHPASSFILTER_ENABLE   ((uint8_t)0x08)

//Acc_High_Pass_Filter_Click_status     
#define LSM303DLHC_HPF_CLICK_DISABLE       ((uint8_t)0x00)
#define LSM303DLHC_HPF_CLICK_ENABLE	       ((uint8_t)0x04)

//Acc_High_Pass_Filter_AOI1_status 
#define LSM303DLHC_HPF_AOI1_DISABLE        ((uint8_t)0x00)
#define LSM303DLHC_HPF_AOI1_ENABLE	       ((uint8_t)0x01)

//Acc_High_Pass_Filter_AOI2_status    
#define LSM303DLHC_HPF_AOI2_DISABLE        ((uint8_t)0x00)
#define LSM303DLHC_HPF_AOI2_ENABLE	   ((uint8_t)0x02)
 
//Acc_LSM303DLHC_Interrupt1_Configuration_definition
#define LSM303DLHC_IT1_CLICK               ((uint8_t)0x80)
#define LSM303DLHC_IT1_AOI1                ((uint8_t)0x40)
#define LSM303DLHC_IT1_AOI2                ((uint8_t)0x20)
#define LSM303DLHC_IT1_DRY1                ((uint8_t)0x10)
#define LSM303DLHC_IT1_DRY2                ((uint8_t)0x08)
#define LSM303DLHC_IT1_WTM                 ((uint8_t)0x04)
#define LSM303DLHC_IT1_OVERRUN             ((uint8_t)0x02)
 
//Acc_LSM303DLHC_Interrupt2_Configuration_definition 
#define LSM303DLHC_IT2_CLICK               ((uint8_t)0x80)
#define LSM303DLHC_IT2_INT1                ((uint8_t)0x40)
#define LSM303DLHC_IT2_INT2                ((uint8_t)0x20)
#define LSM303DLHC_IT2_BOOT                ((uint8_t)0x10)
#define LSM303DLHC_IT2_ACT                 ((uint8_t)0x08)
#define LSM303DLHC_IT2_HLACTIVE            ((uint8_t)0x02)

//Acc_INT_Combination_Status 
#define LSM303DLHC_OR_COMBINATION          ((uint8_t)0x00)  /*!< OR combination of enabled IRQs */
#define LSM303DLHC_AND_COMBINATION	       ((uint8_t)0x80)  /*!< AND combination of enabled IRQs */
#define LSM303DLHC_MOV_RECOGNITION         ((uint8_t)0x40)  /*!< 6D movement recognition */
#define LSM303DLHC_POS_RECOGNITION	       ((uint8_t)0xC0)  /*!< 6D position recognition */

//Acc_INT_Axes 
#define LSM303DLHC_Z_HIGH                  ((uint8_t)0x20)  /*!< Z High enabled IRQs */
#define LSM303DLHC_Z_LOW	                 ((uint8_t)0x10)  /*!< Z low enabled IRQs */
#define LSM303DLHC_Y_HIGH                  ((uint8_t)0x08)  /*!< Y High enabled IRQs */
#define LSM303DLHC_Y_LOW	                 ((uint8_t)0x04)  /*!< Y low enabled IRQs */
#define LSM303DLHC_X_HIGH                  ((uint8_t)0x02)  /*!< X High enabled IRQs */
#define LSM303DLHC_X_LOW	                 ((uint8_t)0x01)  /*!< X low enabled IRQs */

//Acc_INT_Click       
#define LSM303DLHC_Z_DOUBLE_CLICK          ((uint8_t)0x20)  /*!< Z double click IRQs */
#define LSM303DLHC_Z_SINGLE_CLICK	         ((uint8_t)0x10)  /*!< Z single click IRQs */
#define LSM303DLHC_Y_DOUBLE_CLICK          ((uint8_t)0x08)  /*!< Y double click IRQs */
#define LSM303DLHC_Y_SINGLE_CLICK	         ((uint8_t)0x04)  /*!< Y single click IRQs */
#define LSM303DLHC_X_DOUBLE_CLICK          ((uint8_t)0x02)  /*!< X double click IRQs */
#define LSM303DLHC_X_SINGLE_CLICK	         ((uint8_t)0x01)  /*!< X single click IRQs */

//Acc_INT1_Interrupt_status    
#define LSM303DLHC_INT1INTERRUPT_DISABLE   ((uint8_t)0x00)
#define LSM303DLHC_INT1INTERRUPT_ENABLE	   ((uint8_t)0x80)

//Acc_INT1_Interrupt_ActiveEdge
#define LSM303DLHC_INT1INTERRUPT_LOW_EDGE  ((uint8_t)0x20)
#define LSM303DLHC_INT1INTERRUPT_HIGH_EDGE ((uint8_t)0x00)

//Mag_Data_Rate 
#define LSM303DLHC_ODR_0_75_HZ              ((uint8_t) 0x00)  /*!< Output Data Rate = 0.75 Hz */
#define LSM303DLHC_ODR_1_5_HZ               ((uint8_t) 0x04)  /*!< Output Data Rate = 1.5 Hz */
#define LSM303DLHC_ODR_3_0_HZ               ((uint8_t) 0x08)  /*!< Output Data Rate = 3 Hz */
#define LSM303DLHC_ODR_7_5_HZ               ((uint8_t) 0x0C)  /*!< Output Data Rate = 7.5 Hz */
#define LSM303DLHC_ODR_15_HZ                ((uint8_t) 0x10)  /*!< Output Data Rate = 15 Hz */
#define LSM303DLHC_ODR_30_HZ                ((uint8_t) 0x14)  /*!< Output Data Rate = 30 Hz */
#define LSM303DLHC_ODR_75_HZ                ((uint8_t) 0x18)  /*!< Output Data Rate = 75 Hz */
#define LSM303DLHC_ODR_220_HZ               ((uint8_t) 0x1C)  /*!< Output Data Rate = 220 Hz */

//Mag_Full_Scale 
#define  LSM303DLHC_FS_1_3_GA               ((uint8_t) 0x20)  /*!< Full scale = ?.3 Gauss */
#define  LSM303DLHC_FS_1_9_GA               ((uint8_t) 0x40)  /*!< Full scale = ?.9 Gauss */
#define  LSM303DLHC_FS_2_5_GA               ((uint8_t) 0x60)  /*!< Full scale = ?.5 Gauss */
#define  LSM303DLHC_FS_4_0_GA               ((uint8_t) 0x80)  /*!< Full scale = ?.0 Gauss */
#define  LSM303DLHC_FS_4_7_GA               ((uint8_t) 0xA0)  /*!< Full scale = ?.7 Gauss */
#define  LSM303DLHC_FS_5_6_GA               ((uint8_t) 0xC0)  /*!< Full scale = ?.6 Gauss */
#define  LSM303DLHC_FS_8_1_GA               ((uint8_t) 0xE0)  /*!< Full scale = ?.1 Gauss */
 
//Magnetometer_Sensitivity  
#define LSM303DLHC_M_SENSITIVITY_XY_1_3Ga     1100  /*!< magnetometer X Y axes sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_1_9Ga     855   /*!< magnetometer X Y axes sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_2_5Ga     670   /*!< magnetometer X Y axes sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_4Ga       450   /*!< magnetometer X Y axes sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_4_7Ga     400   /*!< magnetometer X Y axes sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_5_6Ga     330   /*!< magnetometer X Y axes sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_8_1Ga     230   /*!< magnetometer X Y axes sensitivity for 8.1 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_1_3Ga      980   /*!< magnetometer Z axis sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_1_9Ga      760   /*!< magnetometer Z axis sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_2_5Ga      600   /*!< magnetometer Z axis sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_4Ga        400   /*!< magnetometer Z axis sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_4_7Ga      355   /*!< magnetometer Z axis sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_5_6Ga      295   /*!< magnetometer Z axis sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_8_1Ga      205   /*!< magnetometer Z axis sensitivity for 8.1 Ga full scale [LSB/Ga] */

//Mag_Working_Mode
#define LSM303DLHC_CONTINUOS_CONVERSION      ((uint8_t) 0x00)   /*!< Continuous-Conversion Mode */
#define LSM303DLHC_SINGLE_CONVERSION         ((uint8_t) 0x01)   /*!< Single-Conversion Mode */
#define LSM303DLHC_SLEEP                     ((uint8_t) 0x02)   /*!< Sleep Mode */                       

//Mag_Temperature_Sensor 
#define LSM303DLHC_TEMPSENSOR_ENABLE         ((uint8_t) 0x80)   /*!< Temp sensor Enable */
#define LSM303DLHC_TEMPSENSOR_DISABLE        ((uint8_t) 0x00)   /*!< Temp sensor Disable */

//LSM303D_CS_PIN
#define LSM303D_CS_LOW()       GPIO_ResetBits(LSM303DLHC_SPI_CS_GPIO_PORT, LSM303DLHC_SPI_CS_PIN)
#define LSM303D_CS_HIGH()      GPIO_SetBits(LSM303DLHC_SPI_CS_GPIO_PORT, LSM303DLHC_SPI_CS_PIN)
/*************************** END LSM303D Selection define  **************************/
//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//

//**********************************************************************************//
//**********************************************************************************//







#endif /* _LSM303D_H_*/

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
