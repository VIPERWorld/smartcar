/*
CCD1 CLK PTA16 SI PTA17 AO PTB4 			OK
CCD2 CLK PTA15 SI PTA14 AO PTB5 			OK
SE8
#define GetADC1AD12()     LPLD_ADC_Get(ADC1,AD12)//PTB6 OK
#define GetADC1AD13()     LPLD_ADC_Get(ADC1,AD13)//PTB7 OK
*/
#ifndef _DRIVERCCD_H_
#define _DRIVERCCD_H_ 
#include "AllInclude.h"
//extern uint8 gPixel[128] = {0};
void CCDInit();
void CCD1ImageCapture(unsigned char * ImageData);
void CCD1Integration();
void CCD2ImageCapture(unsigned char * ImageData);
void CCD2Integration();
//void CCD1GetLine(uint8 *pixel);
//void CCD2GetLine(uint8 *pixel);

#define SamplingDelay()		asm("nop");asm("nop")
//#define CCDelay()       asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop")
#define CCD1SI(x)        (PTA17_O=x)
#define CCD1CLK(x)       (PTA16_O=x)
#define CCD2SI(x)        (PTA14_O=x)
#define CCD2CLK(x)       (PTA15_O=x)
#define ExposureDelay(x)      LPLD_LPTMR_DelayMs(x)
#define GetADC1AD10()       LPLD_ADC_Get(ADC1,AD10)//PTB4
#define GetADC1AD11()       LPLD_ADC_Get(ADC1,AD11)//PTB5
#define GetADC1AD12()       LPLD_ADC_Get(ADC1,AD12)//PTB6
#define GetADC1AD13()       LPLD_ADC_Get(ADC1,AD13)//PTB7
#endif