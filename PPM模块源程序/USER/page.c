#include "page.h"

extern _F4_to_PPM_ F4_to_PPM_BUF;    //引入F4向PPM发送的数据联合体
extern _PPM_to_F4_ PPM_to_F4_BUF;    //引入PPM向F4发送的数据联合体

// 导入记录页面记录器
uint8_t page;

void page1(void)    // 显示信息，为监控模式
{
	OLED_P6x8Str(0, 0, "Rol:");
    OLED_P6x8Str(0, 1, "Pit:");
    OLED_P6x8Str(0, 2, "Yaw:");
	OLED_P6x8Str(0, 3, "MT1:");
	OLED_P6x8Str(0, 4, "MT2:");
	OLED_P6x8Str(0, 5, "MT3:");
	OLED_P6x8Str(0, 6, "MT4:");
    Dis_Float(0, 25, (float)F4_to_PPM_BUF.DATA.Rol /10, 1);
    Dis_Float(1, 25, (float)F4_to_PPM_BUF.DATA.Pit /10, 1);
    Dis_Float(2, 25, (float)F4_to_PPM_BUF.DATA.Yaw /10, 1);
    Dis_Num(3, 4, F4_to_PPM_BUF.DATA.motor[0], 4);
    Dis_Num(4, 4, F4_to_PPM_BUF.DATA.motor[1], 4);
    Dis_Num(5, 4, F4_to_PPM_BUF.DATA.motor[2], 4);
    Dis_Num(6, 4, F4_to_PPM_BUF.DATA.motor[3], 4);
	//显示页码
	Dis_Char( 7, 127, '1');
    //按键扫描
    if (Key_Scan() == KEY_ENTER) {
        OLED_CLS();
        page++;
    }
}

void page2(void)    // 为控制油门界面
{
	OLED_P6x8Str( 0, 2, "Power:");
	OLED_P6x8Str( 0, 4, "Mode:");
	Dis_Num( 2, 50, PPM_to_F4_BUF.DATA.mode_Altiude & 0x0FFF, 4);   //显示油门
	switch ((PPM_to_F4_BUF.DATA.mode_Altiude >> 12) & 0x000F) {     //显示模式
		case 0:
			OLED_P6x8Str( 45, 4, " Stop ");
			break;
		case 1:
			OLED_P6x8Str( 45, 4, "Manual");
			break;
		case 2:
			OLED_P6x8Str( 45, 4, "Automa");
			break;
		default:
			break;
	}
	Dis_Char( 7, 127, '2');     //显示页码
    switch (Key_Scan()) {	    // 判断换页
        case KEY_UP:
			if((PPM_to_F4_BUF.DATA.mode_Altiude & 0x0FFF) < 4000)
				PPM_to_F4_BUF.DATA.mode_Altiude += 20;
			Send_PPM_Data();
            break;
		case KEY_DOWN:
			if((PPM_to_F4_BUF.DATA.mode_Altiude & 0x0FFF) > 2000)
				PPM_to_F4_BUF.DATA.mode_Altiude -= 20;
			Send_PPM_Data();
			break;
		case KEY_LEFT:
			if (((PPM_to_F4_BUF.DATA.mode_Altiude >> 12) & 0x000F) > 0)
				PPM_to_F4_BUF.DATA.mode_Altiude -= 0x1000;
			break;
		case KEY_RIGHT:
			if (((PPM_to_F4_BUF.DATA.mode_Altiude >> 12) & 0x000F) < 2)
				PPM_to_F4_BUF.DATA.mode_Altiude += 0x1000;
			break;
        case KEY_ENTER:
            OLED_CLS();
            page++;
        default:
            break;
    }
}

void page3(void)    // 为控制界面
{
	Dis_Char( 7, 127, '3');
        //按键扫描
    if (Key_Scan() == KEY_ENTER) {
        OLED_CLS();
        page = 0;
    }
}

void OLED_Display(void) {
	switch (page) {
		case 0:
			page1();
			break;
		case 1:
			page2();
			break;
		case 2:
			page3();
			break;
		default:
			page1();
			break;
	}
}
