/*
Motor
pins LeftMotor PTC1 BACK
               PTC2 FORWARD
     RightMotor PTC3 BACK
                PTC4 FORWARD
Encoder(A,B) of Encoder 				OK
	LeftMotor PTB0(A) PTB1(B)
	RightMotor PTB19(A) PTB18(B)
*/
#ifndef _DRIVERFTM_H_
#define _DRIVERFTM_H_ 
#include "AllInclude.h"
//typedef enum{
//	LEFT, 
//	RIGHT,
//}MotorPosition;
//void MotorChangeDuty(MotorPosition MotorPos,int32 Duty);
#define FTM3ChangeCH0Duty(x)        LPLD_FTM_PWM_ChangeDuty(FTM3, FTM_Ch0, (x))
#define FTM3ChangeCH1Duty(x)        LPLD_FTM_PWM_ChangeDuty(FTM3, FTM_Ch1, (x))
#define FTM0ChangeCH0Duty(x)    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0,  (x));//back
#define FTM0ChangeCH1Duty(x)    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch1, (x));//forward
#define FTM0ChangeCH2Duty(x)    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch2,  (x));//back
#define FTM0ChangeCH3Duty(x)    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch3, (x));//forward
#define LeftMotorDeadVol 0
#define RightMotorDeadVol 0
#define MaxDuty 10000
void MotorInit();
void EncoderInit();
void GetSpeed();
void FTM3Init();
void UpdateDuty();
#endif