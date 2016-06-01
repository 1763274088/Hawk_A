#include "Filtering_Algorithm.h"

//##############################################################//
//$$$$$$$$$$$$$$$$$$$$$滑动平均滤波变量定义$$$$$$$$$$$$$$$$$$$$$//
//滑动平均滤波数值个数
#define MA_FILTER_NUM_first		10
#define MA_FILTER_NUM_second	20
//滑动平均滤波计数值
uint32_t MA_Filter_cnt = 0;
//滑动平均滤波缓冲区
int32_t MA_Filter_Buffer_first[3][MA_FILTER_NUM_first + 1];
int32_t MA_Filter_Buffer_second[3][MA_FILTER_NUM_second + 1];
//滑动滤波累加值
int32_t MA_FILTER_Sum_1 = 0, MA_FILTER_Sum_2 = 0, MA_FILTER_Sum_3 = 0;
int32_t MA_FILTER_Sum_4 = 0, MA_FILTER_Sum_5 = 0, MA_FILTER_Sum_6 = 0;
//##############################################################//

//=============================================================//
//滑动平均滤波算法（低通滤波算法）
void Moving_Average_Filter(s16 *buffer_1_Before, s16 *buffer_2_Before, float *buffer_1_after, float *buffer_2_after) {
    //先清掉缓冲区
    MA_FILTER_Sum_1 = 0;
    MA_FILTER_Sum_2 = 0;
    MA_FILTER_Sum_3 = 0;
    MA_FILTER_Sum_4 = 0;
    MA_FILTER_Sum_5 = 0;
    MA_FILTER_Sum_6 = 0;
	
    //先填充缓冲区的末尾值
    MA_Filter_Buffer_first[0][MA_FILTER_NUM_first] = buffer_1_Before[0];
    MA_Filter_Buffer_first[1][MA_FILTER_NUM_first] = buffer_1_Before[1];
    MA_Filter_Buffer_first[2][MA_FILTER_NUM_first] = buffer_1_Before[2];

    MA_Filter_Buffer_second[0][MA_FILTER_NUM_second] = buffer_2_Before[0];
    MA_Filter_Buffer_second[1][MA_FILTER_NUM_second] = buffer_2_Before[1];
    MA_Filter_Buffer_second[2][MA_FILTER_NUM_second] = buffer_2_Before[2];

    //缓冲区往左移动，丢掉最先进来的那个数据
    for (MA_Filter_cnt = 0; MA_Filter_cnt < MA_FILTER_NUM_first; MA_Filter_cnt++) {
        //所有数据往左移动，丢掉最低位
        MA_Filter_Buffer_first[0][MA_Filter_cnt] = MA_Filter_Buffer_first[0][MA_Filter_cnt + 1];
        MA_Filter_Buffer_first[1][MA_Filter_cnt] = MA_Filter_Buffer_first[1][MA_Filter_cnt + 1];
        MA_Filter_Buffer_first[2][MA_Filter_cnt] = MA_Filter_Buffer_first[2][MA_Filter_cnt + 1];

        //进行累加操作
        MA_FILTER_Sum_1 += MA_Filter_Buffer_first[0][MA_Filter_cnt];
        MA_FILTER_Sum_2 += MA_Filter_Buffer_first[1][MA_Filter_cnt];
        MA_FILTER_Sum_3 += MA_Filter_Buffer_first[2][MA_Filter_cnt];
    }

    //缓冲区往左移动，丢掉最先进来的那个数据
    for (MA_Filter_cnt = 0; MA_Filter_cnt < MA_FILTER_NUM_second; MA_Filter_cnt++) {
        //所有数据往左移动，丢掉最低位
        MA_Filter_Buffer_second[0][MA_Filter_cnt] = MA_Filter_Buffer_second[0][MA_Filter_cnt + 1];
        MA_Filter_Buffer_second[1][MA_Filter_cnt] = MA_Filter_Buffer_second[1][MA_Filter_cnt + 1];
        MA_Filter_Buffer_second[2][MA_Filter_cnt] = MA_Filter_Buffer_second[2][MA_Filter_cnt + 1];

        //进行累加操作
        MA_FILTER_Sum_4 += MA_Filter_Buffer_second[0][MA_Filter_cnt];
        MA_FILTER_Sum_5 += MA_Filter_Buffer_second[1][MA_Filter_cnt];
        MA_FILTER_Sum_6 += MA_Filter_Buffer_second[2][MA_Filter_cnt];
    }

    //累加和，除以数量，得到滤波值
    buffer_1_after[0] = (float) (MA_FILTER_Sum_1) / (float) MA_FILTER_NUM_first;
    buffer_1_after[1] = (float) (MA_FILTER_Sum_2) / (float) MA_FILTER_NUM_first;
    buffer_1_after[2] = (float) (MA_FILTER_Sum_3) / (float) MA_FILTER_NUM_first;

    buffer_2_after[0] = (float) (MA_FILTER_Sum_4) / (float) MA_FILTER_NUM_second;
    buffer_2_after[1] = (float) (MA_FILTER_Sum_5) / (float) MA_FILTER_NUM_second;
    buffer_2_after[2] = (float) (MA_FILTER_Sum_6) / (float) MA_FILTER_NUM_second;
}
//=============================================================//

/***********************************************************
** 函数名称: IIR_I_Filter
** 功能描述: IIR直接I型滤波器
** 输    入: *x     储存未滤波的数据
**           *y     储存滤波后的数据
**           *b     储存系数b
**           *a     储存系数a
**           nb     数组*b的长度
**           na     数组*a的长度
**           LpfFactor
** 输    出: OutData
** 说    明: 无
** 函数原型: y(n) = b0*x(n) + b1*x(n-1) + b2*x(n-2) - a1*y(n-1) - a2*y(n-2)

************************************************************/
double IIR_I_Filter(double *x, double *y, double *b, double *a, short nb, short na) {
    double z1, z2;
    short i;
    double OutData;
    for (i = nb - 1; i > 0; i--) {
        x[i] = x[i - 1];
    }
    for (z1 = 0, i = 0; i < nb; i++) {
        z1 += x[i] * b[i];
    }
    for (i = na - 1; i > 0; i--) {
        y[i] = y[i - 1];
    }
    for (z2 = 0, i = 1; i < na; i++) {
        z2 += y[i] * a[i];
    }
    y[0] = z1 - z2;
    OutData = y[0];
    return OutData;
}


#define KalmanFilter_Q 	0.666666f
#define KalmanFilter_R 	0.333333f
void Data_KalmanFilter(float *cov_history, float *Data_history, s16 *Data_now, float *Data_after)
{
	float x_mid[3];    // 假设值不变，预设值
	float p_mid[3];    // 将历史的协方差。。。
	float kg[3];       // 卡尔曼增益

    // 假设值不变，预设值
    x_mid[0] = Data_history[0];
    x_mid[1] = Data_history[1];
    x_mid[2] = Data_history[2];

    // 将历史的协方差。。。
    p_mid[0] = cov_history[0] + KalmanFilter_Q;
    p_mid[1] = cov_history[1] + KalmanFilter_Q;
    p_mid[2] = cov_history[2] + KalmanFilter_Q;
	
    // 计算卡尔曼增益
    kg[0] = p_mid[0] / (p_mid[0] + KalmanFilter_R);
	kg[1] = p_mid[1] / (p_mid[1] + KalmanFilter_R);
	kg[2] = p_mid[2] / (p_mid[2] + KalmanFilter_R);

    // 计算卡尔曼测试值
    Data_history[0] = Data_after[0] = x_mid[0] + kg[0] * (Data_now[0] - x_mid[0]);
    Data_history[1] = Data_after[1] = x_mid[1] + kg[1] * (Data_now[1] - x_mid[1]);
    Data_history[2] = Data_after[2] = x_mid[2] + kg[2] * (Data_now[2] - x_mid[2]);

    // 计算测试值的协方差并将其储存，方便下次使用
    cov_history[0] = (1 - kg[0]) * p_mid[0];
    cov_history[1] = (1 - kg[1]) * p_mid[1];
    cov_history[2] = (1 - kg[2]) * p_mid[2];
}
//=============================================================//
