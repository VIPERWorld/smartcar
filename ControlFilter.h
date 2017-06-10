#ifndef _CONTROLFILTER_H_
#define _CONTROLFILTER_H_ 
#include "AllInclude.h"
#define AccelMin 758  //加速度计最小值
#define AccelMax 2656  //最大值
//#define PhysicalZero 1961-26// 2300 //Zero //2520//1961 Min//  //车平衡时加速度计值
#define GyroIntegralTime 0.005//0.0045  //陀螺仪积分时间
#define K_AngleSpeed 0.9 //角速度互补融合系数

#define K_Angle 0.65   //角度互补融合系数

//#define TurnGyroStandard 2000  //转向角速度基准
#define dt 0.005//0.0045
#define K_Accel 0.005//0.002   
#define GyroCOE 7.07503474//7.6617//13.29 //13.2266667 //13.466667 //6.7324//4.034146 //8.632456  //陀螺仪输出到角速度的转换系数29.87593
void GetAngle();
void GetAngleSpeed();

void KalmanFilter(float32 Accel,float32 Gyro);
#endif