#ifndef _CONTROL_H_
#define _CONTROL_H_ 
#include "AllInclude.h"

#define BalancePeriod 5
#define SpeedPeriod 16
#define SafeMode()     if(Angle==AngleLast)cnt++;\
                       else cnt=0;\
                       if(cnt>=150){\
                       MotorState=-2;\
                       }\
                       AngleLast=Angle;
void ScheduleInit();
void GlobalControl();
void OutputSmooth();
#endif