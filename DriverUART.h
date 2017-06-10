/*
UART3 TX PTC17 						OK
      RX PTC18
UART5 TX PTE8						OK
      RX PTE9
*/
#ifndef _DRIVERUART_H_
#define _DRIVERUART_H_ 
#include "AllInclude.h"
void UARTInit();
#define PutChar(ch) (LPLD_UART_PutChar(UART3,ch))
#define U5PutChar(ch) (LPLD_UART_PutChar(UART5,ch))
#define GetChar()     (LPLD_UART_GetChar(UART3))
//#define U3PutChar(ch) (LPLD_UART_PutChar(UART3,ch))
//#define U5PutChar(ch) (LPLD_UART_PutChar(UART5,ch))
void RemoteCtrl();
#endif