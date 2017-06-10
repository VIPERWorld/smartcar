#include "DriverUART.h"
UART_InitTypeDef uart3_init_struct;
extern int ExpectSpeed;
extern int ExpectOffset;
void UARTInit(){
  
    uart3_init_struct.UART_Uartx = UART5;
    uart3_init_struct.UART_BaudRate = 9600;
    uart3_init_struct.UART_TxPin = PTE8;
    uart3_init_struct.UART_RxPin = PTE9;
    LPLD_UART_Init(uart3_init_struct);
  
}
void RemoteCtrl(){
  char cmd;
  cmd=LPLD_UART_GetChar(UART3);
  if(cmd=='Z')ExpectSpeed=0,ExpectOffset=0;
  if(cmd=='A')ExpectSpeed=150;
  if(cmd=='E')ExpectSpeed=-150;
  if(cmd=='G')ExpectOffset=150;
  if(cmd=='C')ExpectOffset=-150;
}