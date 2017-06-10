#include "DriverADC.h"
ADC_InitTypeDef ADC0_init_struct;
ADC_InitTypeDef ADC2_init_struct;
ADC_InitTypeDef ADC3_init_struct;

void ADCInit(){
    ADC0_init_struct.ADC_Adcx = ADC0;
    ADC0_init_struct.ADC_DiffMode = ADC_SE;
    ADC0_init_struct.ADC_BitMode = SE_12BIT;
    ADC0_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;
    ADC0_init_struct.ADC_HwAvgSel = HW_8AVG;
    ADC0_init_struct.ADC_CalEnable = TRUE;
    LPLD_ADC_Init(ADC0_init_struct);
    //LPLD_ADC_Chn_Enable(ADC0, AD9);//PTB1
    //LPLD_ADC_Chn_Enable(ADC0, AD10);//PTB4
    //LPLD_ADC_Chn_Enable(ADC0, AD11);//PTB2
    //LPLD_ADC_Chn_Enable(ADC0, AD12);//PTB2
    //LPLD_ADC_Chn_Enable(ADC0, AD13);//PTB3
    LPLD_ADC_Chn_Enable(ADC0, AD14);//PTC0
    //LPLD_ADC_Chn_Enable(ADC0, AD15);//PTC1
    LPLD_ADC_Chn_Enable(ADC0, AD17);//PTE24
    LPLD_ADC_Chn_Enable(ADC0, AD18);//PTE25


    
    ADC2_init_struct.ADC_Adcx = ADC2;
    ADC2_init_struct.ADC_DiffMode = ADC_SE;
    ADC2_init_struct.ADC_BitMode = SE_12BIT;
    ADC2_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;
    ADC2_init_struct.ADC_HwAvgSel = HW_8AVG;
    ADC2_init_struct.ADC_CalEnable = TRUE;
    LPLD_ADC_Init(ADC2_init_struct);
    LPLD_ADC_Chn_Enable(ADC2, AD12);//PTA29
    LPLD_ADC_Chn_Enable(ADC2, AD13);//PTA28
/*
    ADC3_init_struct.ADC_Adcx = ADC3;
    ADC3_init_struct.ADC_DiffMode = ADC_SE;
    ADC3_init_struct.ADC_BitMode = SE_12BIT;
    ADC3_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;
    ADC3_init_struct.ADC_HwAvgSel = HW_8AVG;
    ADC3_init_struct.ADC_CalEnable = TRUE;
    LPLD_ADC_Init(ADC3_init_struct);
    LPLD_ADC_Chn_Enable(ADC3, AD16);//PTE11
    LPLD_ADC_Chn_Enable(ADC3, AD17);//PTE12
*/
}
