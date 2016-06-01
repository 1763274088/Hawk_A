#include "lsm303d.h"
#include "delay.h"
#include "spi.h"

LSM_Data Mag;
LSM_Data ACC;
LSM_Data MagFinal;
LSM_Data MAG_OFFSET;

u8 lsm303d_buff_mag[6];
u8 lsm303d_buff_acc[6];

#define MAG_Gain        0.1     //
#define FILTER_NUM      9       // 滑动窗口滤波窗口大小

u16 MAG_X_BUF[FILTER_NUM];
u16 MAG_Y_BUF[FILTER_NUM];
u16 MAG_Z_BUF[FILTER_NUM];

//lsm303d 片选引脚初始化
void LSM303D_GPIO(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);              // LSM303D CS引脚
}
/////////////////////////////////////

//LSM写寄存器
//addr 寄存器地址 data 要写入的数据
void LSM_WriteReg(u8 addr, u8 data) {
    LSM_L;
    SPI1_ReadWriteByte(SPI1, addr);
    SPI1_ReadWriteByte(SPI1, data);
    LSM_H;
}
/////////////////////////////////////

//读寄存器
//reg:要读的寄存器
u8 LSM_ReadReg(u8 reg) {
    u8 reg_val;
    LSM_L;
    SPI1_ReadWriteByte(SPI1, reg | 0X80);
    delay_us(2);
    reg_val = SPI1_ReadWriteByte(SPI1, 0XFF);
    delay_us(1);
    LSM_H;
    return reg_val;
}
/////////////////////////////////////

/////////////////////////////////////
void LSM_Set_CTRL1(u8 com) {
    LSM_L;
    SPI1_ReadWriteByte(SPI1, CTRL1);
    SPI1_ReadWriteByte(SPI1, com);
    LSM_H;
}

void LSM_Set_CTRL2(u8 com) {
    LSM_L;
    SPI1_ReadWriteByte(SPI1, CTRL2);
    SPI1_ReadWriteByte(SPI1, com);
    LSM_H;
}

void LSM_Set_CTRL5(u8 com) {
    LSM_L;
    SPI1_ReadWriteByte(SPI1, CTRL5);
    SPI1_ReadWriteByte(SPI1, com);
    LSM_H;
}

void LSM_Set_CTRL6(u8 com) {
    LSM_L;
    SPI1_ReadWriteByte(SPI1, CTRL6);
    SPI1_ReadWriteByte(SPI1, com);
    LSM_H;
}

void LSM_Set_CTRL7(u8 com) {
    LSM_L;
    SPI1_ReadWriteByte(SPI1, CTRL7);
    SPI1_ReadWriteByte(SPI1, com);
    LSM_H;
}
/////////////////////////////////////

//初始化LSM303D
void LSM303D_Init(void) {
    LSM303D_GPIO();

    LSM_Set_CTRL1(0X87);  //地磁 400HZ 持续更新 X Y Z都工作
    delay_ms(2);
    LSM_Set_CTRL2(0XCA);  //加速度计 50hz ±4 g 不自检 四线SPI
    delay_ms(2);
    LSM_Set_CTRL5(0XF8);  //温度传感器工作
    delay_ms(2);
    LSM_Set_CTRL6(0X40);  //±8 gauss
    delay_ms(2);
    LSM_Set_CTRL7(0XB0);
    delay_ms(2);
}

s16 LSM303D_TEMP(void) {
    u16 temp;
    u8 H, L;
    L = LSM_ReadReg(TEMP_OUT_L);
    H = LSM_ReadReg(TEMP_OUT_H);
    temp = ((s16) H) << 8 | (s16) L;
    return (temp >> 4);
}

void LSM_Read_OFFSET(void) {
    u8 H, L;

    L = LSM_ReadReg(OUT_X_L_M);
    H = LSM_ReadReg(OUT_X_H_M);
    MAG_OFFSET.X = ((s16) H << 8) | L;

    L = LSM_ReadReg(OUT_Y_L_M);
    H = LSM_ReadReg(OUT_Y_H_M);
    MAG_OFFSET.Y = ((s16) H << 8) | L;

    L = LSM_ReadReg(OUT_Z_L_M);
    H = LSM_ReadReg(OUT_Z_H_M);
    MAG_OFFSET.Z = ((s16) H << 8) | L;
}

void LSM303D_DataPrepare(void) {
    u8 H, L;
    static u8 filter_cnt = 0;
    u32 temp1 = 0, temp2 = 0, temp3 = 0;
    u8 i = 0;

    LSM_Read_OFFSET();

    L = LSM_ReadReg(OUT_X_L_M);
    H = LSM_ReadReg(OUT_X_H_M);
    Mag.X = ((s16) H << 8) | L - MAG_OFFSET.X;
    L = LSM_ReadReg(OUT_Y_L_M);
    H = LSM_ReadReg(OUT_Y_H_M);
    Mag.Y = ((s16) H << 8) | L - MAG_OFFSET.Y;
    L = LSM_ReadReg(OUT_Z_L_M);
    H = LSM_ReadReg(OUT_Z_H_M);
    Mag.Z = ((s16) H << 8) | L - MAG_OFFSET.Z;
    L = LSM_ReadReg(OUT_X_L_A);
    H = LSM_ReadReg(OUT_X_H_A);
    ACC.X = ((s16) H << 8) | L;
    L = LSM_ReadReg(OUT_Y_L_A);
    H = LSM_ReadReg(OUT_Y_H_A);
    ACC.Y = ((s16) H << 8) | L;
    L = LSM_ReadReg(OUT_Z_L_A);
    H = LSM_ReadReg(OUT_Z_H_A);
    ACC.Z = ((s16) H << 8) | L;

    MAG_X_BUF[filter_cnt] = Mag.X;
    MAG_Y_BUF[filter_cnt] = Mag.Y;
    MAG_Z_BUF[filter_cnt] = Mag.Z;

    for (i = 0; i < FILTER_NUM; i++) {
        temp1 += MAG_X_BUF[i];
        temp2 += MAG_Y_BUF[i];
        temp3 += MAG_Z_BUF[i];
    }
    Mag.X = temp1 / FILTER_NUM;
    Mag.Y = temp2 / FILTER_NUM;
    Mag.Z = temp3 / FILTER_NUM;

    filter_cnt++;
    if (filter_cnt == FILTER_NUM) filter_cnt = 0;

    MagFinal.X = Mag.X * MAG_Gain;
    MagFinal.Y = Mag.Y * MAG_Gain;
    MagFinal.Z = Mag.Z * MAG_Gain;
}
