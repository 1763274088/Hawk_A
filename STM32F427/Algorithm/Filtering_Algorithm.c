#include "Filtering_Algorithm.h"

//##############################################################//
//$$$$$$$$$$$$$$$$$$$$$����ƽ���˲���������$$$$$$$$$$$$$$$$$$$$$//
//����ƽ���˲���ֵ����
#define MA_FILTER_NUM_first 			10
#define MA_FILTER_NUM_second 			20
//����ƽ���˲�����ֵ
uint32_t MA_Filter_cnt = 0;	
//����ƽ���˲�������
int32_t	MA_Filter_Buffer_first[3][MA_FILTER_NUM_first+1 ];
int32_t	MA_Filter_Buffer_second[3][MA_FILTER_NUM_second+1];
//�����˲��ۼ�ֵ
int32_t MA_FILTER_Sum_1=0,MA_FILTER_Sum_2=0,MA_FILTER_Sum_3=0;
int32_t	MA_FILTER_Sum_4=0,MA_FILTER_Sum_5=0,MA_FILTER_Sum_6=0;
//##############################################################//

//=============================================================//
/*
����ƽ���˲��㷨

*/
void Moving_Average_Filter(s16 *buffer_1_Before,s16 *buffer_2_Before,float *buffer_1_after,float *buffer_2_after)
{
		//�����������
		MA_FILTER_Sum_1=0;
		MA_FILTER_Sum_2=0;
		MA_FILTER_Sum_3=0;
		MA_FILTER_Sum_4=0;
		MA_FILTER_Sum_5=0;
		MA_FILTER_Sum_6=0;

		//����仺������ĩβֵ
		MA_Filter_Buffer_first[0][MA_FILTER_NUM_first]=buffer_1_Before[0];
		MA_Filter_Buffer_first[1][MA_FILTER_NUM_first]=buffer_1_Before[1];	
		MA_Filter_Buffer_first[2][MA_FILTER_NUM_first]=buffer_1_Before[2];
	
		MA_Filter_Buffer_second[0][MA_FILTER_NUM_second]=buffer_2_Before[0];
		MA_Filter_Buffer_second[1][MA_FILTER_NUM_second]=buffer_2_Before[1];
		MA_Filter_Buffer_second[2][MA_FILTER_NUM_second]=buffer_2_Before[2];
	
		//�����������ƶ����������Ƚ������Ǹ�����
		for(MA_Filter_cnt = 0; MA_Filter_cnt < MA_FILTER_NUM_first; MA_Filter_cnt++)
		{
		//�������������ƶ����������λ	
		MA_Filter_Buffer_first[0][MA_Filter_cnt]=MA_Filter_Buffer_first[0][MA_Filter_cnt+1];
		MA_Filter_Buffer_first[1][MA_Filter_cnt]=MA_Filter_Buffer_first[1][MA_Filter_cnt+1];	
		MA_Filter_Buffer_first[2][MA_Filter_cnt]=MA_Filter_Buffer_first[2][MA_Filter_cnt+1];
	
		//�����ۼӲ���
		MA_FILTER_Sum_1+=MA_Filter_Buffer_first[0][MA_Filter_cnt];
		MA_FILTER_Sum_2+=MA_Filter_Buffer_first[1][MA_Filter_cnt];
		MA_FILTER_Sum_3+=MA_Filter_Buffer_first[2][MA_Filter_cnt];			
		}
		
		//�����������ƶ����������Ƚ������Ǹ�����
		for(MA_Filter_cnt = 0; MA_Filter_cnt < MA_FILTER_NUM_second; MA_Filter_cnt++)
		{
		//�������������ƶ����������λ	
		MA_Filter_Buffer_second[0][MA_Filter_cnt]=MA_Filter_Buffer_second[0][MA_Filter_cnt+1];
		MA_Filter_Buffer_second[1][MA_Filter_cnt]=MA_Filter_Buffer_second[1][MA_Filter_cnt+1];
		MA_Filter_Buffer_second[2][MA_Filter_cnt]=MA_Filter_Buffer_second[2][MA_Filter_cnt+1];			
	
		//�����ۼӲ���
		MA_FILTER_Sum_4+=MA_Filter_Buffer_second[0][MA_Filter_cnt];
		MA_FILTER_Sum_5+=MA_Filter_Buffer_second[1][MA_Filter_cnt];
		MA_FILTER_Sum_6+=MA_Filter_Buffer_second[2][MA_Filter_cnt];				
		}
		

		//�ۼӺͣ������������õ��˲�ֵ
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