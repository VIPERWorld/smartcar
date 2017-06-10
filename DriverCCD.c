#include "DriverCCD.h"
GPIO_InitTypeDef CCDPins;
ADC_InitTypeDef CCD_init_struct;
LPTMR_InitTypeDef Exposure_Delay;
uint8 Pixel_1[128],Pixel_2[128];
void CCDInit(){
  ///////////////CLK & SI/////////////////////
    CCDPins.GPIO_PTx=PTA;
    CCDPins.GPIO_Pins=GPIO_Pin17|GPIO_Pin16|GPIO_Pin14|GPIO_Pin15;///PTA17 PTA16 PTA14 PTA15
    CCDPins.GPIO_Dir=DIR_OUTPUT;
    CCDPins.GPIO_Output=OUTPUT_L;
    CCDPins.GPIO_PinControl=IRQC_DIS;
    LPLD_GPIO_Init(CCDPins);
    ////AD1/////
    CCD_init_struct.ADC_Adcx = ADC1;
    CCD_init_struct.ADC_DiffMode = ADC_SE;
    CCD_init_struct.ADC_BitMode = SE_8BIT;
    CCD_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;
    CCD_init_struct.ADC_HwAvgSel = HW_4AVG;
    CCD_init_struct.ADC_CalEnable = TRUE;
    LPLD_ADC_Init(CCD_init_struct);
    LPLD_ADC_Chn_Enable(ADC1, AD10);//PTB4
    LPLD_ADC_Chn_Enable(ADC1, AD11);//PTB5
    //LPLD_ADC_Chn_Enable(ADC1, AD12);//PTB6
    //LPLD_ADC_Chn_Enable(ADC1, AD13);//PTB7

 /*   
    ////EXPOSURE_TIME_DELAY//////
  Exposure_Delay.LPTMR_Mode = LPTMR_MODE_TIMER;
  Exposure_Delay.LPTMR_PeriodMs = 1000; //1s
  LPLD_LPTMR_Init(Exposure_Delay); 
 */
}
void CCD1Integration(){
    unsigned char i;

    CCD1SI(1);            /* SI  = 1 */
    CCD1CLK(1);           /* CLK = 1 */
    CCD1SI(0);            /* SI  = 0 */
    CCD1CLK(0);           /* CLK = 0 */

    for(i=0; i<127; i++) {
        CCD1CLK(1);       /* CLK = 1 */
        CCD1CLK(0);       /* CLK = 0 */
    }
    CCD1CLK(1);           /* CLK = 1 */
    CCD1CLK(0);           /* CLK = 0 */
}
void CCD1ImageCapture(unsigned char * ImageData) {

    unsigned char i;
    CCD1SI(1);            /* SI  = 1 */
    CCD1CLK(1);           /* CLK = 1 */
    CCD1SI(0);            /* SI  = 0 */

    //Delay 10us for sample the first pixel
    /**/
 /*   for(i = 0; i < 50; i++) {
      SamplingDelay() ;  //200ns
    }*/

    //Sampling Pixel 1

    *ImageData =  LPLD_ADC_Get(ADC1, AD10);
    ImageData ++ ;
    CCD1CLK(0);           /* CLK = 0 */

    for(i=0; i<127; i++) {
        CCD1CLK(1);       /* CLK = 1 */
    //Sampling Pixel 2~128
       *ImageData =  LPLD_ADC_Get(ADC1, AD10);
        ImageData ++ ;
        CCD1CLK(0);       /* CLK = 0 */
    }
    CCD1CLK(1);           /* CLK = 1 */
    CCD1CLK(0);           /* CLK = 0 */
}
void CCD2Integration(){
    unsigned char i;

    CCD2SI(1);            /* SI  = 1 */
    CCD2CLK(1);           /* CLK = 1 */
    CCD2SI(0);            /* SI  = 0 */
    CCD2CLK(0);           /* CLK = 0 */

    for(i=0; i<127; i++) {
        CCD2CLK(1);       /* CLK = 1 */
        CCD2CLK(0);       /* CLK = 0 */
    }
    CCD2CLK(1);           /* CLK = 1 */
    CCD2CLK(0);           /* CLK = 0 */
}
void CCD2ImageCapture(unsigned char * ImageData) {

    unsigned char i;
    CCD2SI(1);            /* SI  = 1 */
    CCD2CLK(1);           /* CLK = 1 */
    CCD2SI(0);            /* SI  = 0 */

    //Delay 10us for sample the first pixel
    /**/
 /*   for(i = 0; i < 50; i++) {
      SamplingDelay() ;  //200ns
    }*/

    //Sampling Pixel 1

    *ImageData =  LPLD_ADC_Get(ADC1, AD11);
    ImageData ++ ;
    CCD2CLK(0);           /* CLK = 0 */

    for(i=0; i<127; i++) {
        CCD2CLK(1);       /* CLK = 1 */
    //Sampling Pixel 2~128
       *ImageData =  LPLD_ADC_Get(ADC1, AD11);
        ImageData ++ ;
        CCD2CLK(0);       /* CLK = 0 */
    }
    CCD2CLK(1);           /* CLK = 1 */
    CCD2CLK(0);           /* CLK = 0 */
}