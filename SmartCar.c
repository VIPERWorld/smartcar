#include "AllInclude.h"
extern uint8 Pixel[128];
extern int LeftSpeed,RightSpeed;
extern int LeftDuty,RightDuty;
extern float32 Angle,AngleSpeed;
extern int GyroStandard;
extern float32 P_Balance,D_Balance,P_Speed,I_Speed,P_Turn;
extern uint8 PixelProcessBegin;
extern void Havemidline();
extern void CCDhandle();
extern uint8 midline_2[2];
extern float32 ExpectOffset;
extern int PhysicalZero;
extern float32 GyroIntegral;
extern int8 MotorState;
extern uint8 Pixel_2[128],PixelB_2[128];
extern struct PixelLineStruct PixelLineNear[10],PixelLineForward[10];
extern uint8 PixelNear[128],PixelForward[128];
void delay();
extern int8 Mode;
extern int8 AllHigh;
float32 tmp;
extern int ExpectSpeed;
extern float32 TmpAngle;
void main(){
    DisableInterrupts;
    FPUEnable();
/************System Init*************/
    OLEDInit();
    OLEDShowString(0,0,"System Initing ...");
    OLEDShowString(0,2,"Please wait without touching !");
    MotorInit();
    UARTInit();//Default for UART5
    ADCInit();
    SwitchInit();
    LEDInit();
    CCDInit();
    EncoderInit();
    //FTM3Init();
    BuzzerInit();
    TeleControlInit();
    InfraredTubeInit();
    ButtonInit();
    MPU6050Init();
/***********End of System Init********/
    //U5PutChar('a');
    //PutChar('b');
/***************/
    delay();
    EnableInterrupts;
    GetGyroStandard();
    //GetPhysicalZero();
    ScheduleInit();
    OLEDClear();
    MotorState=-1;
    //GyroIntegral=(float32)(-GetADC0AD12()+PhysicalZero);
    GyroIntegral=(float32)(GetAccelZ());
    TmpAngle=(float32)(GetAccelZ());
    ExpectSpeed=0;
    LED1(1);
    LED2(1);
    LED3(1);
    LED4(1);
   for(;;){
     if(Mode>=1)Buzzer(1);
     else Buzzer(0);
     //Buzzer(1);
     /*
          if(PixelProcessBegin){
       if(PixelBinaryzation()==1){
        // CCDView(PixelNear);
         ExpectOffset=0;
         PixelProcessBegin=0;
       }else{
         //CCDView(PixelNear);
         ExpectOffset=ModeIdentification(PixelLineNear,PixelIndex(PixelNear,PixelLineNear,10));
         PixelProcessBegin=0;
         //CCDView(PixelNear);
       }
     }
*/

    /*
     if(PixelProcessBegin){
       CCDhandle();
       Havemidline();
       ExpectOffset=midline_2[1]-64;
       //GetTurnAngleSpeed();
       //TurnCtrl();
       PixelProcessBegin=0;
     }
     */
    OLEDShow();


    }
}
      
 ///////////////////////////////////////////////////////
/*
   LPLD_UART_PutCharArr(UART3,"Input something:\r\n",18);
   int8 get;
   // /ar ch;
    while(1)
    {
   // / = in_char();
    //   out_char(ch);
      get = LPLD_UART_GetChar(UART3);
      LPLD_UART_PutChar(UART3,get);
    } 
    ///////////////////////////////////////////////////////
*/


/*
uint32 sec=1;
void pit_test_isr(){
    U5PutChar('a');
    PIT->CHANNEL[PIT2].LDVAL = sec*1000000*(g_bus_clock/1000000)-1;
    sec++;
}
*/
void delay()
{
  uint16 i, n;
  for(i=0;i<90;i++)
  {
    for(n=0;n<200;n++)
    {
      asm("nop");
    }
  }
}
    
     /*******
         accel_x = MPU6050GetResult(ACCEL_XOUT_H);
    accel_y = MPU6050GetResult(ACCEL_YOUT_H);
    accel_z = MPU6050GetResult(ACCEL_ZOUT_H);
    gyro_x  = MPU6050GetResult(GYRO_XOUT_H);
    gyro_y  = MPU6050GetResult(GYRO_YOUT_H);
    gyro_z  = MPU6050GetResult(GYRO_ZOUT_H);
     *****/
    

