#include "Filtering_Algorithm.h"

//##############################################################//
//$$$$$$$$$$$$$$$$$$$$$滑动平均滤波变量定义$$$$$$$$$$$$$$$$$$$$$//
//滑动平均滤波数值个数
#define MA_FILTER_NUM_first 			10
#define MA_FILTER_NUM_second 			20
//滑动平均滤波计数值
uint32_t MA_Filter_cnt = 0;	
//滑动平均滤波缓冲区
int32_t	MA_Filter_Buffer_first[3][MA_FILTER_NUM_first+1 ];
int32_t	MA_Filter_Buffer_second[3][MA_FILTER_NUM_second+1];
//滑动滤波累加值
int32_t MA_FILTER_Sum_1=0,MA_FILTER_Sum_2=0,MA_FILTER_Sum_3=0;
int32_t	MA_FILTER_Sum_4=0,MA_FILTER_Sum_5=0,MA_FILTER_Sum_6=0;
//##############################################################//

//=============================================================//
/*
滑动平均滤波算法

*/
void Moving_Average_Filter(s16 *buffer_1_Before,s16 *buffer_2_Before,float *buffer_1_after,float *buffer_2_after)
{
		//先清掉缓冲区
		MA_FILTER_Sum_1=0;
		MA_FILTER_Sum_2=0;
		MA_FILTER_Sum_3=0;
		MA_FILTER_Sum_4=0;
		MA_FILTER_Sum_5=0;
		MA_FILTER_Sum_6=0;

		//先填充缓冲区的末尾值
		MA_Filter_Buffer_first[0][MA_FILTER_NUM_first]=buffer_1_Before[0];
		MA_Filter_Buffer_first[1][MA_FILTER_NUM_first]=buffer_1_Before[1];	
		MA_Filter_Buffer_first[2][MA_FILTER_NUM_first]=buffer_1_Before[2];
	
		MA_Filter_Buffer_second[0][MA_FILTER_NUM_second]=buffer_2_Before[0];
		MA_Filter_Buffer_second[1][MA_FILTER_NUM_second]=buffer_2_Before[1];
		MA_Filter_Buffer_second[2][MA_FILTER_NUM_second]=buffer_2_Before[2];
	
		//缓冲区往左移动，丢掉最先进来的那个数据
		for(MA_Filter_cnt = 0; MA_Filter_cnt < MA_FILTER_NUM_first; MA_Filter_cnt++)
		{
		//所有数据往左移动，丢掉最低位	
		MA_Filter_Buffer_first[0][MA_Filter_cnt]=MA_Filter_Buffer_first[0][MA_Filter_cnt+1];
		MA_Filter_Buffer_first[1][MA_Filter_cnt]=MA_Filter_Buffer_first[1][MA_Filter_cnt+1];	
		MA_Filter_Buffer_first[2][MA_Filter_cnt]=MA_Filter_Buffer_first[2][MA_Filter_cnt+1];
	
		//进行累加操作
		MA_FILTER_Sum_1+=MA_Filter_Buffer_first[0][MA_Filter_cnt];
		MA_FILTER_Sum_2+=MA_Filter_Buffer_first[1][MA_Filter_cnt];
		MA_FILTER_Sum_3+=MA_Filter_Buffer_first[2][MA_Filter_cnt];			
		}
		
		//缓冲区往左移动，丢掉最先进来的那个数据
		for(MA_Filter_cnt = 0; MA_Filter_cnt < MA_FILTER_NUM_second; MA_Filter_cnt++)
		{
		//所有数据往左移动，丢掉最低位	
		MA_Filter_Buffer_second[0][MA_Filter_cnt]=MA_Filter_Buffer_second[0][MA_Filter_cnt+1];
		MA_Filter_Buffer_second[1][MA_Filter_cnt]=MA_Filter_Buffer_second[1][MA_Filter_cnt+1];
		MA_Filter_Buffer_second[2][MA_Filter_cnt]=MA_Filter_Buffer_second[2][MA_Filter_cnt+1];			
	
		//进行累加操作
		MA_FILTER_Sum_4+=MA_Filter_Buffer_second[0][MA_Filter_cnt];
		MA_FILTER_Sum_5+=MA_Filter_Buffer_second[1][MA_Filter_cnt];
		MA_FILTER_Sum_6+=MA_Filter_Buffer_second[2][MA_Filter_cnt];				
		}
		

		//累加和，除以数量，得到滤波值
		buffer_1_after[0]=(float)( MA_FILTER_Sum_1 )/(float)MA_FILTER_NUM_first;
		buffer_1_after[1]=(float)( MA_FILTER_Sum_2 )/(float)MA_FILTER_NUM_first;
		buffer_1_after[2]=(float)( MA_FILTER_Sum_3 )/(float)MA_FILTER_NUM_first;
		
		buffer_2_after[0]=(float)( MA_FILTER_Sum_4 )/(float)MA_FILTER_NUM_second;
		buffer_2_after[1]=(float)( MA_FILTER_Sum_5 )/(float)MA_FILTER_NUM_second;
		buffer_2_after[2]=(float)( MA_FILTER_Sum_6 )/(float)MA_FILTER_NUM_second;		
}
//=============================================================//



void IIR_Digital_Filter()
{




}



void Kalman_Filter()
{






}