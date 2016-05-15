/********************************************************************************
  * @file    bsp.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    24-3-2016
  * @brief   Header for bsp.c module
*******************************************************************************/ 
#ifndef _BSP_H_
#define _BSP_H_
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"
#include "uart.h"
#include "mpu6000.h"
#include "ms5611.h"
#include "sdio_sdcard.h"
#include "malloc.h" 
#include "tim.h"
#include "led.h" 
#include "ff.h"  
#include "exfuns.h"  
#include "spi2.h"
#include "fm25v01.h"
#include "TCA62724.h"
#include "communication.h"

//**********************************************************************************//
//Macro definition area:Define the parameters of the device
//**********************************************************************************//
/****************************** START REGISTER MAPPING  *****************************/



/******************************* END REGISTER MAPPING  ******************************/
/************************************************************************************/
/***************************** START IIC Interface pins  ****************************/



/****************************** END IIC Interface pins  *****************************/
/************************************************************************************/
/************************** START TCA62724 Selection define *************************/



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
void BSP_initialization(void);			//外设初始化函数，包含所有的舒适化操作
void Sensor_Calibration(void);			//校准传感器
void Parameter_loading(void);				//加载控制参数
/****************************** END Function definition *****************************/
//**********************************************************************************//




#endif /* _BSP_H_*/

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
