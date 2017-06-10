/*
ON BOARD LEDn() PTB20 PTB21 PTB22 PTB23			OK
	Switcher PTD7(*)
Switcher  PTB8 PTB9 PTB16 PTB17 
*/
#ifndef _DRIVERGPIO_H_
#define _DRIVERGPIO_H_ 
#include "AllInclude.h"
void LEDInit();
void SwitchInit();
void ButtonInit();
void SwitchISR();
void ButtonISR();
void TeleControlISR();
void SwitchDelay();
void GPIOInit();
void InfraredTubeInit();
void TeleControlInit();
void InfraredTubeISR();
void BuzzerInit();
#define Buzzer(x) PTD1_O=(x)
#define LED1(x) PTB20_O=~(x)
#define LED2(x) PTB21_O=~(x)
#define LED3(x) PTB22_O=~(x)
#define LED4(x) PTB23_O=~(x)

#endif