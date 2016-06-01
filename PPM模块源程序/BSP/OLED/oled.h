#ifndef _OLED_H
#define _OLED_H

#include "stm32f10x.h"
#define OLED_GPIO1    GPIOA
#define OLED_GPIO2    GPIOB

#define OLED_RCCEN1		RCC_APB2Periph_GPIOA
#define OLED_RCCEN2		RCC_APB2Periph_GPIOB

#define OLED_SCL		GPIO_Pin_5      //D0	PORT B
#define OLED_SDA		GPIO_Pin_4      //D1	PORT B
#define OLED_RST		GPIO_Pin_3      //RST	PORT B
#define OLED_DC			GPIO_Pin_15		//DC	PORT A

#define OLED_D0_OL		GPIO_ResetBits(OLED_GPIO2, OLED_SCL)	//D0 IO口输出低电平
#define OLED_D0_OH		GPIO_SetBits(OLED_GPIO2, OLED_SCL)  	//D0 IO口输出高电平

#define OLED_D1_OL		GPIO_ResetBits(OLED_GPIO2, OLED_SDA)	//D1 IO口输出低电平
#define OLED_D1_OH		GPIO_SetBits(OLED_GPIO2, OLED_SDA) 		//D1 IO口输出高电平

#define OLED_RST_OL		GPIO_ResetBits(OLED_GPIO2, OLED_RST)	//RST IO口输出低电平
#define OLED_RST_OH		GPIO_SetBits(OLED_GPIO2, OLED_RST) 		//RST IO口输出高电平

#define OLED_DC_OL		GPIO_ResetBits(OLED_GPIO1, OLED_DC)		//DC IO口输出低电平
#define OLED_DC_OH		GPIO_SetBits(OLED_GPIO1, OLED_DC) 		//DC IO口输出高电平

void OLED_WrDat(unsigned char data);
void OLED_WrCmd(unsigned char cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void OLED_Fill(unsigned char bmp_data) ;
void OLED_CLS(void);
void OLED_DLY_ms(unsigned int ms);
void adjust(unsigned char a) ;
void SetStartColumn(unsigned char d);
void SetAddressingMode(unsigned char d);
void SetColumnAddress(unsigned char a, unsigned char b);
void SetPageAddress(unsigned char a, unsigned char b);
void SetStartLine(unsigned char d);
void SetContrastControl(unsigned char d) ;
void Set_Charge_Pump(unsigned char d);
void Set_Segment_Remap(unsigned char d);
void Set_Entire_Display(unsigned char d);
void Set_Inverse_Display(unsigned char d);
void Set_Multiplex_Ratio(unsigned char d);
void Set_Display_On_Off(unsigned char d);
void SetStartPage(unsigned char d);
void Set_Common_Remap(unsigned char d);
void Set_Display_Offset(unsigned char d);
void Set_Display_Clock(unsigned char d);
void Set_Precharge_Period(unsigned char d);
void Set_Common_Config(unsigned char d);
void Set_VCOMH(unsigned char d);
void Set_NOP(void);
void OLED_Init(void);
void OLED_PutPixel(unsigned char x, unsigned char y);
void OLED_P6x8Str(u8 x, u8 y, u8 ch[]);
void OLED_P8x16Str(u8 x, u8 y, u8 ch[]);
void Dis_Char(unsigned char y, unsigned char x, unsigned char asc);
void Dis_Num(unsigned char y, unsigned char x, unsigned int num, unsigned char N);
void Dis_Float(unsigned char Y, unsigned char X, double real, unsigned char N);
void Dis_Float2(unsigned char Y, unsigned char X, double real, unsigned char N1, unsigned char N2);
void Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char bmp[]);
void OLED_P8x16Num_8bit(unsigned char x, unsigned char y, unsigned char Number);
void OLED_P6x8Num_8bit(unsigned char x, unsigned char y, unsigned char Number);
void OLED_3num(unsigned char x, unsigned char y, unsigned char number);
void OLED_4num(unsigned char x, unsigned char y, int number);
void OLED_Num(unsigned char x, unsigned char y, unsigned char asc);
void OLED_Num5(unsigned char x, unsigned char y, unsigned int number);
;
#endif
