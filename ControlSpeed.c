#include "ControlSpeed.h"
extern int LeftSpeed,RightSpeed;
extern int SpeedDuty;
extern float32 P_Speed,I_Speed;
extern int ExpectSpeed;
extern int Speed;
extern int BalanceDuty;
extern float32 D_Speed;
extern int32 SpeedIntegral;
int SpeedError[2]={0},SpeedErrorLast=0;
void SpeedCtrl(){
  static int SpeedArray[5]={0};
  static int SpeedDutyLast=0;
  //static int SpeedError=0,SpeedErrorLast=0;
  SpeedArray[0]=SpeedArray[1];
  SpeedArray[1]=SpeedArray[2];
  SpeedArray[2]=SpeedArray[3];
  SpeedArray[3]=SpeedArray[4];
  SpeedArray[4]=(LeftSpeed+RightSpeed)/2;
//  Speed=(int)((SpeedArray[0]+SpeedArray[1]+SpeedArray[2]+SpeedArray[3]+SpeedArray[4]+SpeedArray[5]+SpeedArray[6]+SpeedArray[7]+SpeedArray[8]+SpeedArray[9])*0.1);
  Speed=(int)((SpeedArray[0]+SpeedArray[1]+SpeedArray[2]+SpeedArray[3]+SpeedArray[4])*0.2);

  //Speed-=ExpectSpeed;
  SpeedError[1]=ExpectSpeed-Speed;
  SpeedIntegral+=SpeedError[1];
  //积分限幅
  SpeedIntegral=SpeedIntegral>IntegralMax?IntegralMax:SpeedIntegral;
  SpeedIntegral=SpeedIntegral<-IntegralMax?-IntegralMax:SpeedIntegral;
  SpeedDuty=(int)(P_Speed * SpeedError[1] + I_Speed * SpeedIntegral + D_Speed * (SpeedError[1]-SpeedErrorLast));
  SpeedErrorLast=SpeedError[1];
  //平滑处理
  SpeedDuty=(int)(K_SpeedDuty * SpeedDuty + (1-K_SpeedDuty) * SpeedDutyLast);
  SpeedDutyLast=SpeedDuty;
  SpeedDuty=SpeedDuty>SpeedDutyMax?SpeedDutyMax:SpeedDuty;
  SpeedDuty=SpeedDuty<-SpeedDutyMax?-SpeedDutyMax:SpeedDuty;
  
  /*
  SpeedError[1]=ExpectSpeed-Speed;
  SpeedIntegral+=SpeedError[1];
  SpeedDuty=(int)(P_Speed * (SpeedError[1]-SpeedError[0]) + I_Speed * SpeedIntegral);
  SpeedDuty=SpeedDuty>SpeedDutyMax?SpeedDutyMax:SpeedDuty;
  SpeedDuty=SpeedDuty<-SpeedDutyMax?-SpeedDutyMax:SpeedDuty;
  SpeedError[0]=ExpectSpeed-Speed;
  */
}