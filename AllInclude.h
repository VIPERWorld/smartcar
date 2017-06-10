#ifndef _ALLINCLUDE_H_
#define _ALLINCLUDE_H_ 
#include "common.h"
#include "DriverFTM.h"
#include "DriverADC.h"
#include "DriverGPIO.h"
#include "DriverDebugger.h"
#include "DriverCCD.h"
#include "Control.h"
#include "ControlBalance.h"
#include "ControlFilter.h"
#include "ControlSpeed.h"
#include "ControlTurn.h"
#include "ControlPixelProcess.h"
#include "DEV_MPU6050.h"
/***********Debugger*************/
#include "LPLD_OLED.h"
#include "DriverUART.h"
#include "SDS.h"
#include "DataScope_DP.h"
#define FPUEnable() SCB->CPACR |=((3UL << 10*2)|(3UL << 11*2))
#endif