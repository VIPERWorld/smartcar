#ifndef _DRIVERDEBUGGER_H_
#define _DRIVERDEBUGGER_H_ 
#include "AllInclude.h"
void CCD1View(uint8 delayms);//SI PTA17 CLK PTA16 AO PTB4
void CCD2View(uint8 delayms);//SI PTA14 CLK PTA15 AO PTB5
void Plot();
void AppShow();
void GetGyroStandard();
void GetGyroStandardISR();
void GetPhysicalZero();
void GetPhysicalZeroISR();
void OLEDShow();
void CCDView(uint8 * Pixel);
void OLEDShowLine(uint8 *);

#endif