/********************************************************************************
  * @file    TCA62724.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    29-12-2015
  * @brief   Header for TCA62724.c module
*******************************************************************************/ 
#ifndef _TCA62724_H_
#define _TCA62724_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"

//**********************************************************************************//
//Macro definition area:Define the parameters of the device
//**********************************************************************************//
/****************************** START REGISTER MAPPING  *****************************/
//*Slave address
#define TCA62724_IIC_write         						((uint8_t)0xAA)  /* 	Write Address 	*/
#define TCA62724_IIC_read         						((uint8_t)0xAB)  /*  	Read address  	*/
//*Sub-address
//When this bit is set to H auto-increment is OFF
#define TCA62724_IIC_PWM0         						((uint8_t)0x81)  /* PWM Duty Data Setup of OUT0 */
#define TCA62724_IIC_PWM1         						((uint8_t)0x82)  /* PWM Duty Data Setup of OUT1 */
#define TCA62724_IIC_PWM2         						((uint8_t)0x83)  /* PWM Duty Data Setup of OUT2 */
#define TCA62724_IIC_ENABLE_SHDN         			((uint8_t)0x84)  /* Data Setup of ENABLE / SHDN */
//when it is set to L auto-increment is ON.
#define TCA62724_IIC_PWM0_auto         				((uint8_t)0x01)  /* PWM Duty Data Setup of OUT0 */
#define TCA62724_IIC_PWM1_auto         				((uint8_t)0x02)  /* PWM Duty Data Setup of OUT1 */
#define TCA62724_IIC_PWM2_auto         				((uint8_t)0x03)  /* PWM Duty Data Setup of OUT2 */
#define TCA62724_IIC_ENABLE_SHDN_auto         ((uint8_t)0x04)  /* Data Setup of ENABLE / SHDN */
/******************************* END REGISTER MAPPING  ******************************/
/************************************************************************************/
/***************************** START IIC Interface pins  ****************************/
#define TCA62724_IIC                       		GPIOB
#define TCA62724_IIC_CLK                   		RCC_AHB1Periph_GPIOB 

#define TCA62724_IIC_SCK_PIN               		GPIO_Pin_10                  /* PB.10 */
#define TCA62724_IIC_SCK_GPIO_PORT         		GPIOB                        /* GPIOB */
#define TCA62724_IIC_SCK_GPIO_CLK          		RCC_AHB1Periph_GPIOB
#define TCA62724_IIC_SCK_SOURCE            		GPIO_PinSource10

#define TCA62724_IIC_SDA_PIN               		GPIO_Pin_11                  /* PB.11 */
#define TCA62724_IIC_SDA_GPIO_PORT         		GPIOB                        /* GPIOB */
#define TCA62724_IIC_SDA_GPIO_CLK          		RCC_AHB1Periph_GPIOB
#define TCA62724_IIC_SDA_SOURCE            		GPIO_PinSource11

//Direction
#define TCA62724_IIC_SDA_IN()  						 		{GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=0<<11*2;}	//PB11 Input Mode
#define TCA62724_IIC_SDA_OUT() 						 		{GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=1<<11*2;}  //PB11 Output Mode

#define TCA62724_IIC_SCL    							 		PBout(10) //SCL
#define TCA62724_IIC_SDA    							 		PBout(11) //SDA	 
#define TCA62724_READ_SDA   							 		PBin(11)  //Input 
/****************************** END IIC Interface pins  *****************************/
/************************************************************************************/
/************************** START TCA62724 Selection define *************************/
//*DATA byte PWM0, PWM1, and PWM2 DATA 				PWM ON Duty DATA (0/15 to 15/15)
#define TCA62724_IIC_PWM_Duty_no         			((uint8_t)0x00)  /* PWM ON Duty	0/15 */
#define TCA62724_IIC_PWM_Duty_1         			((uint8_t)0x01)  /* PWM ON Duty	1/15 */
#define TCA62724_IIC_PWM_Duty_2         			((uint8_t)0x02)  /* PWM ON Duty	2/15 */
#define TCA62724_IIC_PWM_Duty_3         			((uint8_t)0x03)  /* PWM ON Duty	3/15 */
#define TCA62724_IIC_PWM_Duty_4         			((uint8_t)0x04)  /* PWM ON Duty	4/15 */
#define TCA62724_IIC_PWM_Duty_5         			((uint8_t)0x05)  /* PWM ON Duty	5/15 */
#define TCA62724_IIC_PWM_Duty_6         			((uint8_t)0x06)  /* PWM ON Duty	6/15 */
#define TCA62724_IIC_PWM_Duty_7         			((uint8_t)0x07)  /* PWM ON Duty	7/15 */
#define TCA62724_IIC_PWM_Duty_8         			((uint8_t)0x08)  /* PWM ON Duty	8/15 */
#define TCA62724_IIC_PWM_Duty_9         			((uint8_t)0x09)  /* PWM ON Duty	9/15 */
#define TCA62724_IIC_PWM_Duty_10         			((uint8_t)0x0A)  /* PWM ON Duty	10/15*/
#define TCA62724_IIC_PWM_Duty_11         			((uint8_t)0x0B)  /* PWM ON Duty	11/15*/
#define TCA62724_IIC_PWM_Duty_12         			((uint8_t)0x0C)  /* PWM ON Duty	12/15*/
#define TCA62724_IIC_PWM_Duty_13         			((uint8_t)0x0D)  /* PWM ON Duty	13/15*/
#define TCA62724_IIC_PWM_Duty_14         			((uint8_t)0x0E)  /* PWM ON Duty	14/15*/
#define TCA62724_IIC_PWM_Duty_15         			((uint8_t)0x0F)  /* PWM ON Duty	15/15*/

//ENABLE / SHDN DATA
#define TCA62724_IIC_ENABLE_Open         			((uint8_t)0x02)  /* Output blinks at PWM0, PWM1, and PWM2 rate */
#define TCA62724_IIC_ENABLE_Dowm       				((uint8_t)0x00)  /* Output is OFF*/
#define TCA62724_IIC_SHDN_Open         				((uint8_t)0x01)  /* Output blinks at PWM0, PWM1, and PWM2 rate */
#define TCA62724_IIC_SHDN_Dowm         				((uint8_t)0x00)  /* Power-saving mode */
#define TCA62724_IIC_ENABLE_SHDN_Open         ((uint8_t)0x03)  /* Open */
#define TCA62724_IIC_ENABLE_SHDN_Dowm       	((uint8_t)0x00)  /* Dowm*/
/*************************** END TCA62724 Selection define **************************/
//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Definition Structure area:Defines the structure of the operating parameters
//**********************************************************************************//
//typedef   union _FM25V01_WRITE_ADDRESS
//{
//		uint16_t write_adderss;
//		uint8_t  adderss_area[sizeof(uint16_t)];	
//}FM25V01_WRITE_ADDRESS;
//extern FM25V01_WRITE_ADDRESS FM25V01_Write;

//typedef   union _FM25V01_READ_ADDRESS
//{
//		uint16_t read_adderss;
//		uint8_t  adderss_area_read[sizeof(uint16_t)];	
//}FM25V01_READ_ADDRESS;
//extern FM25V01_READ_ADDRESS FM25V01_Read;



//typedef struct _FM25V01_Manufacturer_Product_ID
//{
//    uint8_t    Continuation_code_1;      		//Continuation code  0x7F
//    uint8_t    Continuation_code_2;      		//Continuation code  0x7F
//    uint8_t    Continuation_code_3;      		//Continuation code  0x7F 
//    uint8_t    Continuation_code_4 ;      	//Continuation code  0x7F 
//    uint8_t    Continuation_code_5 ;      	//Continuation code  0x7F
//    uint8_t    Continuation_code_6;      		//Continuation code  0x7F 
//    uint8_t    JEDEC_assigned_Ramtron ;     //JEDEC assigned Ramtron C2h in bank 7 0xC2 
//    uint8_t    Device_ID_1 ;      					//Density: 01h=128K, 02h=256K, 03h=512K, 04=1M 	
//    uint8_t    Device_ID_2 ;      					//00h=FM25V01		
//}_FM25V01_MANUFACTURER_PRODUCT_ID,*P_FM25V01_MANUFACTURER_PRODUCT_ID;
////这里是小端模式
//typedef   union _FM25V01_Manufacturer_Product_ID_
//{
//    _FM25V01_MANUFACTURER_PRODUCT_ID FM25V01_Product_ID;
//    uint8_t           ID_buffer[sizeof(_FM25V01_MANUFACTURER_PRODUCT_ID)];
//}FM25V01_Manufacturer_Product_ID;

//extern FM25V01_Manufacturer_Product_ID FM25V01_ID;

///* L3GD20 struct */
//typedef struct
//{
//  uint8_t Power_Mode;                         /* Power-down/Sleep/Normal Mode */
//  uint8_t Output_DataRate;                    /* OUT data rate */
//  uint8_t Axes_Enable;                        /* Axes enable */
//  uint8_t Band_Width;                         /* Bandwidth selection */
//  uint8_t BlockData_Update;                   /* Block Data Update */
//  uint8_t Endianness;                         /* Endian Data selection */
//  uint8_t Full_Scale;                         /* Full Scale selection */
//}L3GD20_InitTypeDef;

//**********************************************************************************//
//**********************************************************************************//


//**********************************************************************************//
//Function definition area:All functions defined in the file
//**********************************************************************************//
/***************************** START Function definition ****************************/
static void TCA62724_IIC_Init(void);
static void TCA62724_IIC_Start(void);
static void TCA62724_IIC_Stop(void);
u8 TCA62724_IIC_Wait_Ack(void);
static void TCA62724_IIC_Ack(void);
static void TCA62724_IIC_NAck(void);

void TCA62724_IIC_Send_Byte(u8 Send_data);
uint8_t TCA62724_IIC_Read_Byte(unsigned char ack);

void TCA62724_Init(void);
void TCA62724_Write_Pwm_Data(uint8_t mode,uint8_t PWM0_Data,uint8_t PWM1_Data,uint8_t PWM2_Data);
void TCA62724_Read_staus_Data(uint8_t* First_byte,uint8_t* Second_byte );
/****************************** END Function definition *****************************/
//**********************************************************************************//




#endif /* _TCA62724_H_*/

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
