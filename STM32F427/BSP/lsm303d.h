#include "stm32f4xx.h"
#include "stdint.h"

#ifndef _LSM303D_H
#define _LSM303D_H

#define LSM_H GPIO_SetBits(GPIOC,GPIO_Pin_15)
#define LSM_L GPIO_ResetBits(GPIOC,GPIO_Pin_15)

#define TEMP_OUT_L     0X05
#define TEMP_OUT_H     0X06   //12位
#define STATUS_M       0X07 
#define OUT_X_L_M      0X08   //磁力计X轴低位
#define OUT_X_H_M      0X09   //磁力计X轴高位
#define OUT_Y_L_M      0X0A   //磁力计Y轴低位
#define OUT_Y_H_M      0X0B   //磁力计Y轴高位
#define OUT_Z_L_M      0X0C   //磁力计Z轴低位
#define OUT_Z_H_M      0X0D   //磁力计Z轴高位
#define WHO_AM_I       0X0F   //0X49
#define INT_CTRL_M     0X12
#define INT_SRC_M      0X13
#define INT_THS_L_M    0X14
#define INT_THS_H_M    0X15
#define OFFSET_X_L_M   0X16
#define OFFSET_X_H_M   0X17
#define OFFSET_Y_L_M   0X18
#define OFFSET_Y_H_M   0X19
#define OFFSET_Z_L_M   0X1A
#define OFFSET_Z_H_M   0X1B
#define REFERENCE_X    0X1C
#define REFERENCE_Y    0X1D
#define REFERENCE_Z    0X1E
#define CTRL0          0X1F
#define CTRL1          0X20
#define CTRL2          0X21
#define CTRL3          0X22
#define CTRL4          0X23
#define CTRL5          0X24
#define CTRL6          0X25
#define CTRL7          0X26
#define STATUS_A       0X27
#define OUT_X_L_A      0X28   //加速度X轴低位
#define OUT_X_H_A      0X29   //加速度X轴高位
#define OUT_Y_L_A      0X2A   //加速度Y轴低位  
#define OUT_Y_H_A      0X2B   //加速度Y轴高位
#define OUT_Z_L_A      0X2C   //加速度Z轴低位
#define OUT_Z_H_A      0X2D   //加速度Z轴高位

typedef struct LSM303D{
    s16 X;
    s16 Y;
    s16 Z;
} LSM_Data;
extern  LSM_Data Mag;
extern  LSM_Data ACC;
extern  LSM_Data MagFinal;
extern  LSM_Data MAG_OFFSET;
             
u8 LSM_ReadReg(u8 reg);
void  LSM303D_Init(void);
void LSM303D_DataPrepare(void);
s16 LSM303D_TEMP(void);

#endif
