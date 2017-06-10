#include "DriverFTM.h"
extern int LeftSpeed,RightSpeed;
FTM_InitTypeDef ftm_init_struct;
PIT_InitTypeDef pit_init_struct;

uint16 LeftEncoderResult=0,RightEncoderResult=0;
extern int LeftDuty,RightDuty;
extern int LeftSpeed,RightSpeed;
void FTM3Init(){
    ftm_init_struct.FTM_Ftmx = FTM3;
    ftm_init_struct.FTM_Mode = FTM_MODE_PWM;
    ftm_init_struct.FTM_PwmFreq =10000;
    //ftm_init_struct.FTM_PwmDeadtimeCfg = DEADTIME_CH01;
    //ftm_init_struct.FTM_PwmDeadtimeDiv = DEADTIME_DIV1;
    LPLD_FTM_Init(ftm_init_struct);
    //LPLD_FTM_PWM_Enable(FTM3, FTM_Ch0, 0, PTD0, ALIGN_LEFT);//PTD0
    LPLD_FTM_PWM_Enable(FTM3, FTM_Ch1, 0, PTD1, ALIGN_LEFT);//PTD1
}
void MotorInit(){ 
    ftm_init_struct.FTM_Ftmx = FTM0;
    ftm_init_struct.FTM_Mode = FTM_MODE_PWM;
    ftm_init_struct.FTM_PwmFreq =10000;
    //ftm_init_struct.FTM_PwmDeadtimeCfg = DEADTIME_CH01;
    //ftm_init_struct.FTM_PwmDeadtimeDiv = DEADTIME_DIV1;
    LPLD_FTM_Init(ftm_init_struct);
    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch0, 0, PTC1, ALIGN_LEFT);//PTC1
    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch1, 0, PTC2, ALIGN_LEFT);//PTC2
    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch2, 0, PTC3, ALIGN_LEFT);//PTC3
    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch3, 0, PTC4, ALIGN_LEFT);//PTC4
}
void EncoderInit(){
    ftm_init_struct.FTM_Ftmx = FTM1;
    ftm_init_struct.FTM_Mode = FTM_MODE_QD;
    ftm_init_struct.FTM_QdMode = QD_MODE_PHAB;
    LPLD_FTM_Init(ftm_init_struct);
    LPLD_FTM_QD_Enable(FTM1,PTB0,PTB1);
    
    ftm_init_struct.FTM_Ftmx = FTM2;
    ftm_init_struct.FTM_Mode = FTM_MODE_QD;          
    ftm_init_struct.FTM_QdMode = QD_MODE_PHAB;          //QD_MODE_PHAB  为AB相正交解码，QD_MODE_CNTDIR为计数与方向解码模式
    LPLD_FTM_Init(ftm_init_struct);
    LPLD_FTM_QD_Enable(FTM2,PTB18,PTB19);
/*
    pit_init_struct.PIT_Pitx = PIT3;
    pit_init_struct.PIT_PeriodMs = 20;
    pit_init_struct.PIT_PeriodS=NULL;
    pit_init_struct.PIT_PeriodUs=NULL;
    pit_init_struct.PIT_Isr = EncoderCounterISR;
    LPLD_PIT_Init(pit_init_struct);
    LPLD_PIT_EnableIrq(pit_init_struct);
 */
}
void GetSpeed(){
    RightSpeed = LPLD_FTM_GetCounter(FTM1);
    LeftSpeed = LPLD_FTM_GetCounter(FTM2);
    //if(LeftDuty<=0)LeftSpeed-=65535;
    //if(RightDuty<=0)RightSpeed-=65535;
    //if(RightDuty<0)RightSpeed-=65535;
    //if(LeftSpeed<0)LeftSpeed-=65535;
    //if(LeftDuty<0)LeftSpeed=LeftSpeed;
    //if(RightDuty<0)RightSpeed=RightSpeed;
    //printf("%d\t%d\n",L_qd_result,R_qd_result);
    LPLD_FTM_ClearCounter(FTM1);
    LPLD_FTM_ClearCounter(FTM2);
    LeftSpeed=(LeftSpeed>=30000)?LeftSpeed-65535:LeftSpeed;
    RightSpeed=(RightSpeed>=30000)?RightSpeed-65535:RightSpeed;
    
    //LPLD_UART_PutChar(UART5,qd_result);
}
//void MotorChangeDuty(MotorPosition MotorPos,int32 Duty){
//    switch(MotorPos)
//	{
//		case LEFT :
//			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0, Duty > 0 ? 0 : -Duty);//back
//			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch1, Duty > 0 ? Duty : 0);//forward
//			break;
//		case RIGHT :
//			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch2, Duty > 0 ? 0 : -Duty);//back
//			LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch3, Duty > 0 ? Duty : 0);//forward
//                        break;
//	}
//}

void UpdateDuty(){
  //static int LDutyArray[5]={0};
  //static int RDutyArray[5]={0};
  
  //static int cnt=0;
  LeftDuty=-LeftDuty;
  RightDuty=-RightDuty;
  /*
  if(ABS(LeftDuty)>7000 || ABS(RightDuty)>7000)cnt++;
  
  if(cnt>100){
  LeftDuty=0;
  RightDuty=0;
  }
  */
  if(LeftDuty>=MaxDuty)LeftDuty=MaxDuty;
  if(LeftDuty<=-MaxDuty)LeftDuty=-MaxDuty;
  if(RightDuty>=MaxDuty)RightDuty=MaxDuty;
  if(RightDuty<=-MaxDuty)RightDuty=-MaxDuty;
  /*
  LDutyArray[0]=LDutyArray[1];
  LDutyArray[1]=LDutyArray[2];
  LDutyArray[2]=LeftDuty;
  RDutyArray[0]=RDutyArray[1];
  RDutyArray[1]=RDutyArray[2];
  RDutyArray[2]=RightDuty;
  
  LeftDuty=(int)((LDutyArray[0]+LDutyArray[1]+LDutyArray[2])*0.3333);
  RightDuty=(int)((RDutyArray[0]+RDutyArray[1]+RDutyArray[2])*0.3333);
  */
  if(LeftDuty>=0) {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0,  0);//back
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch1, LeftDuty+LeftMotorDeadVol);//forward
  } else {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch1,  0);//forward
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0, -(LeftDuty-LeftMotorDeadVol));//back
  }

    if(RightDuty<=0) {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch2,  0);//back
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch3, -(RightDuty-RightMotorDeadVol));//forward
  } else {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch3,  0);//forward
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch2,  RightDuty+RightMotorDeadVol);//back
  }
}
