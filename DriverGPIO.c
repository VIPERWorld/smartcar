#include "DriverGPIO.h"
/*** call for debug***/
extern int ExpectSpeed;
extern float32 ExpectOffset;
extern float32 P_Balance,D_Balance;
extern float32 P_Speed,I_Speed;
extern int8 MotorState;
extern int ExpectSpeedSet;
extern float32 PhysicalZero;
extern float32 P_Turn;
extern float32 D_Turn;
extern float32 D_Speed;
extern int SpeedIntegral;
GPIO_InitTypeDef gpio;
void LEDInit(){
    gpio.GPIO_PTx=PTB;
    gpio.GPIO_Pins=GPIO_Pin20|GPIO_Pin21|GPIO_Pin22|GPIO_Pin23;
    gpio.GPIO_Dir=DIR_OUTPUT;
    gpio.GPIO_Output=OUTPUT_H;
    gpio.GPIO_PinControl=IRQC_DIS;
    LPLD_GPIO_Init(gpio);
}
void SwitchInit(){
  gpio.GPIO_PTx=PTB;
  gpio.GPIO_Pins=GPIO_Pin8|GPIO_Pin9|GPIO_Pin16|GPIO_Pin17;
  gpio.GPIO_Dir=DIR_INPUT;
  //Switch.GPIO_PinControl=IRQC_FA|INPUT_PULL_UP;
  //Switch.GPIO_Isr=SwitchISR;
  LPLD_GPIO_Init(gpio);
  LPLD_GPIO_EnableIrq(gpio);
}
void ButtonInit(){
  /*
  gpio.GPIO_PTx=PTA;
  gpio.GPIO_Pins=GPIO_Pin10|GPIO_Pin11|GPIO_Pin12|GPIO_Pin13;
  gpio.GPIO_Dir=DIR_INPUT;
  gpio.GPIO_PinControl=IRQC_FA|INPUT_PULL_UP;
  gpio.GPIO_Isr=ButtonISR;
  LPLD_GPIO_Init(gpio);
  LPLD_GPIO_EnableIrq(gpio);
  */
    gpio.GPIO_PTx=PTE;
    gpio.GPIO_Pins=GPIO_Pin0|GPIO_Pin1|GPIO_Pin2|GPIO_Pin3|GPIO_Pin4|GPIO_Pin5;//|GPIO_Pin6|GPIO_Pin7
    gpio.GPIO_Dir=DIR_INPUT;
    gpio.GPIO_PinControl=IRQC_FA|INPUT_PULL_UP;
    gpio.GPIO_Isr=ButtonISR;
    LPLD_GPIO_Init(gpio);
    LPLD_GPIO_EnableIrq(gpio);
}
void GPIOInit(){
  /*
    GPIO_InitTypeDef gpio;
    gpio.GPIO_PTx=PTE;
    gpio.GPIO_Pins=GPIO_Pin0|GPIO_Pin1|GPIO_Pin2|GPIO_Pin3|GPIO_Pin4|GPIO_Pin5|GPIO_Pin6|GPIO_Pin7;
    gpio.GPIO_Dir=DIR_OUTPUT;
    gpio.GPIO_Output=OUTPUT_L;
    gpio.GPIO_PinControl=IRQC_DIS;
    LPLD_GPIO_Init(gpio);
  */
}
void BuzzerInit(){
    gpio.GPIO_PTx=PTD;
    gpio.GPIO_Pins=GPIO_Pin1;
    gpio.GPIO_Dir=DIR_OUTPUT;
    gpio.GPIO_Output=OUTPUT_L;
    gpio.GPIO_PinControl=IRQC_DIS;
    LPLD_GPIO_Init(gpio);
}
void InfraredTubeInit(){
  gpio.GPIO_PTx=PTC;
  gpio.GPIO_Pins=GPIO_Pin5|GPIO_Pin7;
  gpio.GPIO_Dir=DIR_INPUT;
  gpio.GPIO_PinControl=IRQC_FA|INPUT_PULL_UP;
  gpio.GPIO_Isr=InfraredTubeISR;
  LPLD_GPIO_Init(gpio);
  LPLD_GPIO_EnableIrq(gpio);
}
void TeleControlInit(){
  gpio.GPIO_PTx=PTD;
  gpio.GPIO_Pins=GPIO_Pin11|GPIO_Pin12|GPIO_Pin13|GPIO_Pin14;
  gpio.GPIO_Dir=DIR_INPUT;
  gpio.GPIO_PinControl=IRQC_ET|INPUT_PULL_UP;
  gpio.GPIO_Isr=TeleControlISR;
  LPLD_GPIO_Init(gpio);
  LPLD_GPIO_EnableIrq(gpio);
}
void TeleControlISR(){
  /****Coding Here*****/
      if(LPLD_GPIO_IsPinxExt(PORTD, GPIO_Pin11)){
LPLD_GPIO_Toggle_b(PTB, 20);
ExpectSpeed=ExpectSpeedSet;
ExpectOffset=0;

    }
      if(LPLD_GPIO_IsPinxExt(PORTD, GPIO_Pin12)){
LPLD_GPIO_Toggle_b(PTB, 21);
ExpectSpeed=0;
ExpectOffset=0;
SpeedIntegral=0;

    }
      if(LPLD_GPIO_IsPinxExt(PORTD, GPIO_Pin13)){
LPLD_GPIO_Toggle_b(PTB, 22);
MotorState*=-1;
    }
      if(LPLD_GPIO_IsPinxExt(PORTD, GPIO_Pin14)){
LPLD_GPIO_Toggle_b(PTB, 23);
//ExpectOffset=-180;
SpeedIntegral=0;

    }
}
void InfraredTubeISR(){
    /****Coding Here*****/
  printf("infraretube\n");

}
void ButtonISR(){

      if(LPLD_GPIO_IsPinxExt(PORTE, GPIO_Pin0)){
      if(!PTE0_I){
        SwitchDelay();
      if(!PTE0_I){
//        if(!PTB8_I){
//          if(!PTB9_I)P_Balance+=0.5;
//          
//        if(!PTB9_I)P_Speed+=5;
//        if(!PTB16_I)P_Turn+=4;
        if(!PTB8_I && !PTB9_I)P_Balance+=0.5;
        if(!PTB8_I && PTB9_I)P_Speed+=4;
        if(PTB8_I && !PTB9_I)P_Turn+=4;
        if(PTB8_I && PTB9_I);
        }
      }
    }

          if(LPLD_GPIO_IsPinxExt(PORTE, GPIO_Pin1)){
      if(!PTE1_I){
        SwitchDelay();
      if(!PTE1_I){
//        if(!PTB8_I)P_Balance-=0.5;
//        if(!PTB9_I)P_Speed-=5;
//        if(!PTB16_I)P_Turn-=4;
        //if(!PTB9_I)P_Turn-=2;
        if(!PTB8_I && !PTB9_I)P_Balance-=0.5;
        if(!PTB8_I && PTB9_I)P_Speed-=4;
        if(PTB8_I && !PTB9_I)P_Turn-=4;
        if(PTB8_I && PTB9_I);

        }
      }
    }
          if(LPLD_GPIO_IsPinxExt(PORTE, GPIO_Pin2)){
      if(!PTE2_I){
        SwitchDelay();
      if(!PTE2_I){
        if(!PTB8_I && !PTB9_I)D_Balance+=0.4;
        if(!PTB8_I && PTB9_I)I_Speed+=0.1;
        if(PTB8_I && !PTB9_I)D_Turn+=0.2;
        if(PTB8_I && PTB9_I);

//      if(!PTB8_I)D_Balance+=0.4;
//      if(!PTB16_I)D_Turn+=0.2;
//        if(!PTB9_I)I_Speed+=0.1;
        //D_Turn+=0.2;
        }
      }
    }
          if(LPLD_GPIO_IsPinxExt(PORTE, GPIO_Pin3)){
      if(!PTE3_I){
        SwitchDelay();
      if(!PTE3_I){
        if(!PTB8_I && !PTB9_I)D_Balance-=0.4;
        if(!PTB8_I && PTB9_I)I_Speed-=0.1;
        if(PTB8_I && !PTB9_I)D_Turn-=0.2;
        if(PTB8_I && PTB9_I);

//if(!PTB16_I)D_Turn-=0.2;
 //       if(!PTB9_I)I_Speed-=0.1;
        //D_Turn-=0.2;
        }
      }
    }
              if(LPLD_GPIO_IsPinxExt(PORTE, GPIO_Pin4)){
      if(!PTE4_I){
        SwitchDelay();
      if(!PTE4_I){
        if(!PTB8_I && !PTB9_I)PhysicalZero+=1;
        if(!PTB8_I && PTB9_I)ExpectSpeedSet+=20;
        if(PTB8_I && !PTB9_I);
        if(PTB8_I && PTB9_I);
        //P_Speed+=2;
        }
      }
    }
              if(LPLD_GPIO_IsPinxExt(PORTE, GPIO_Pin5)){
      if(!PTE5_I){
        SwitchDelay();
      if(!PTE5_I){
        if(!PTB8_I && !PTB9_I)PhysicalZero-=1;
        if(!PTB8_I && PTB9_I)ExpectSpeedSet-=20;
        if(PTB8_I && !PTB9_I);
        if(PTB8_I && PTB9_I);
        }
      }
    }
}
void SwitchISR(){

    /**************PTB8*************/

          if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin8)){
      if(!PTB8_I){
        SwitchDelay();
    if(!PTB8_I){
	PTB20_O=0;
	PTB21_O=0;
	PTB22_O=0;
	PTB23_O=0;
	while(!PTB8_I);
        PTB20_O=1;
	PTB21_O=1;
	PTB22_O=1;
	PTB23_O=1;
        }
      }
    }
    //PTB9
          if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin9)){
      if(!PTB9_I){
        SwitchDelay();
    if(!PTB9_I){
	PTB20_O=0;
	PTB21_O=0;
	PTB22_O=0;
	PTB23_O=0;
	while(!PTB9_I);
        PTB20_O=1;
	PTB21_O=1;
	PTB22_O=1;
	PTB23_O=1;
        }
      }
    }
    
    //PTB16
          if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin16)){
      if(!PTB16_I){
        SwitchDelay();
    if(!PTB16_I){
	PTB20_O=0;
	PTB21_O=0;
	PTB22_O=0;
	PTB23_O=0;
	while(!PTB16_I);
        PTB20_O=1;
	PTB21_O=1;
	PTB22_O=1;
	PTB23_O=1;
        }
      }
    }
    //PTB17
          if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin17)){
      if(!PTB17_I){
        SwitchDelay();
    if(!PTB17_I){
	PTB20_O=0;
	PTB21_O=0;
	PTB22_O=0;
	PTB23_O=0;
	while(!PTB17_I);
        PTB20_O=1;
	PTB21_O=1;
	PTB22_O=1;
	PTB23_O=1;
        }
      }
    }

}
void SwitchDelay(){
    uint16 i, n;
  for(i=0;i<300;i++)
  {
    for(n=0;n<100;n++)
    {
      asm("nop");
    }
  }
}