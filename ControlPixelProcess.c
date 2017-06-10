#include "ControlPixelProcess.h"
uint8 PixelNear[128],PixelForward[128];
extern float32 ExpectOffset;
extern float32 ExpectOffsetLast;
struct PixelLineStruct PixelLineNear[10],PixelLineForward[10];
uint8 LineStepNear=0,LineStepForward=0;
uint8 RoadWidth=57;
uint8 State=0;
int8 AllHigh=0;
int8 Mode=0;
uint8 PixelBinaryzation(){
    uint8 i,ThresholdNearMax,ThresholdNearMin,ThresholdForwardMax,ThresholdForwardMin,ThresholdNear,ThresholdForward;
    int32 PixelSumNear=0,PixelSumForward=0;
    //平滑处理
    
    for(i=1;i<127;i++){
        PixelNear[i] = (uint8)(0.15*PixelNear[i-1]+0.7*PixelNear[i]+0.15*PixelNear[i+1]);
        PixelForward[i] = (uint8)(0.15*PixelForward[i-1]+0.7*PixelForward[i]+0.15*PixelForward[i+1]);
    }
    
    //边界处理
    PixelNear[4] = PixelNear[5];   PixelNear[123] = PixelNear[122];
    PixelNear[3] = PixelNear[4];   PixelNear[124] = PixelNear[123];
    PixelNear[2] = PixelNear[3];   PixelNear[125] = PixelNear[124];
    PixelNear[1] = PixelNear[2];   PixelNear[126] = PixelNear[125];
    PixelNear[0] = PixelNear[1];   PixelNear[127] = PixelNear[126];

    PixelForward[4] = PixelForward[5];   PixelForward[123] = PixelForward[122];
    PixelForward[3] = PixelForward[4];   PixelForward[124] = PixelForward[123];
    PixelForward[2] = PixelForward[3];   PixelForward[125] = PixelForward[124];
    PixelForward[1] = PixelForward[2];   PixelForward[126] = PixelForward[125];
    PixelForward[0] = PixelForward[1];   PixelForward[127] = PixelForward[126];
    ThresholdNearMax=ThresholdNearMin=PixelNear[0];
    ThresholdForwardMax=ThresholdForwardMin=PixelForward[0];
    for(i=1;i<128;i++){
      if(PixelNear[i]>ThresholdNearMax)ThresholdNearMax=PixelNear[i];
      if(PixelNear[i]<ThresholdNearMin)ThresholdNearMin=PixelNear[i];
      if(PixelForward[i]>ThresholdForwardMax)ThresholdForwardMax=PixelForward[i];
      if(PixelForward[i]<ThresholdForwardMin)ThresholdForwardMin=PixelForward[i];
    }
    
    //计算阈值
    //ThresholdNear=(ThresholdNearMax+ThresholdNearMin)/2+ThresholdNearOffset;
    //ThresholdForward=(ThresholdForwardMax+ThresholdForwardMax)/2+ThresholdForwardOffset;
    ThresholdNear=230;
    ThresholdForward=230;
    for(i=0;i<=127;i++){//二值化
      PixelSumNear+=PixelNear[i];
      PixelSumForward+=PixelForward[i];
      PixelNear[i]=PixelNear[i]>ThresholdNear?255:0;
      PixelForward[i]=PixelForward[i]>ThresholdForward?255:0;
    }
    

    if(((PixelSumNear/128)>NearPixelHighThreshold)){
      AllHigh=1;
    }else AllHigh=0;
    //AllHigh=0;
   
    /*
        for(i=0;i<=127;i++){//二值化
      if(PixelNear[i]>200)PixelNear[i]=255;
      else PixelNear[i]=0;
     // PixelNear[i]=(PixelNear[i]>100)?255:0;
      //PixelForward[i]=PixelForward[i]>100?255:0;
    }
    
    return 0;
    */
    /*
    if(((PixelSumNear/128)>NearPixelHighThreshold)  || ((PixelSumNear/128)<NearPixelLowThreshold))
      return 1;
    //1:近线全白或全黑
    else return 0;
    */
    return 0;
}
uint8 PixelIndex(uint8 Pixel[],struct PixelLineStruct PixelLine[],uint8 length){
  uint8 i,StepNum=0;
  for(i=0;i<=126;i++){
    if(Pixel[i]==0&&Pixel[i+1]==255){//上升跳变
      PixelLine[StepNum].Type=UP;
      PixelLine[StepNum].Position=i;
      StepNum++;
    }else if(Pixel[i]==255&&Pixel[i+1]==0){//下降跳变
      PixelLine[StepNum].Type=DOWN;
      PixelLine[StepNum].Position=i;
      StepNum++;
    }
    if(StepNum>=length)break;//需要异常处理，PixelLine溢出，默认长度10
  }
  return StepNum;
}
int16 PixelForwardIndex(uint8 *Pixel){
  uint8 i=0,j=0;
  if(Pixel[64]==0){
    for(i=64;i<=126;i++){
      if(Pixel[i]==0 && Pixel[i+1]==255)break;
    }
    for(j=64;j>=1;j--){
      if(Pixel[j]==0 && Pixel[j-1]==255)break;
    }
    return -(i+j)/2;
  }else{
    for(i=64;i<=126;i++){
      if(Pixel[i]==255 && Pixel[i+1]==0)break;
    }
    for(j=64;j>=1;j--){
      if(Pixel[j]==255 && Pixel[j-1]==0)break;
    }
    return (i+j)/2;
  }
}

float32 ModeIdentification(struct PixelLineStruct *PixelLine,uint8 PixelStep){
  //uint8 i;
  float32 ExpectOffsetTmp=0;
  if(AllHigh==1)State=100;
  else State=0;
  if((Mode>=1 && Mode<=2) || (Mode>=4 && Mode <=6)){
  Mode++;
  return Offset;
  }
  if(Mode==3 && AllHigh==1){
    Mode=4;
  }
  if(Mode==7)Mode=0;
  //if(PixelIndex(PixelForward,PixelLineForward,10)==2 && PixelLineForward[0].Type==DOWN  && AllHigh==1){
  if(PixelForwardIndex(PixelForward)<=-20 && AllHigh==1){
    State=100;
    Mode=1;
    return Offset;
  }else State=0;
  if(AllHigh==1)return 0;
  /********************************/
  if(AllHigh==1)return 0;
  
  if(PixelStep==1){//单跳变，丢线
    if(PixelLine[0].Type==UP)//                                 ____\~~~~~~
      return (PixelLine[0].Position+(RoadWidth/2.0)-63.5);
    else//                                                      ~~~~~\______
      return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
  }else if(PixelStep==2){//
    if(PixelLine[0].Type==UP){//                                 ______\~~~~Num~~~~\______   - Left
      if((PixelLine[1].Position-PixelLine[0].Position)>RoadWidthThreshold)
        return (PixelLine[1].Position+PixelLine[0].Position)/2.0-63.5;
      else //                                                   _____\~\____
        return 0;
    }else{//
      //if(PixelLine[0].Position>(127-PixelLine[1].Position)){//     ~~~~~~~~~~~~~~\___\~~
      //ExpectOffsetTmp=(PixelLine[0].Position-(RoadWidth/2.0)-63.5);
      if(ABS((PixelLine[0].Position-(RoadWidth/2.0)-63.5)-ExpectOffsetLast) < ABS((PixelLine[1].Position+(RoadWidth/2.0)-63.5)-ExpectOffsetLast)){
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
      }else{ //                                                   ~~~\____\~~~~~~~~~~~~~~~~~~
        return (PixelLine[1].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else if(PixelStep==3){//
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(127-PixelLine[2].Position))//__\~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                 //                  ___\~~\__\~~~~~~~~~~~~~~~~
        return (PixelLine[2].Position+(RoadWidth/2.0)-63.5);
    }else{
      if((PixelLine[2].Position-PixelLine[1].Position)>PixelLine[0].Position)//~~~~\__\~~~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[1].Position)/2.0-63.5;
      else                              //                                  ~~~~~~~~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
    }
  }else if(PixelStep==4){
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(PixelLine[3].Position-PixelLine[2].Position))//__\~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                              //                        __\~~\__\~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[3].Position)/2.0-63.5; 
    }else{// 
      if((PixelLine[2].Position-PixelLine[1].Position)>RoadWidthThreshold)//                      ~~~\__\~~~~~~Num~~~~~~~\__\~~
        return (PixelLine[1].Position+PixelLine[2].Position)/2.0-63.5;
      else{
        if(PixelLine[0].Position>(127-PixelLine[3].Position))//                                  ~~~~~~~~~~~~~~\__\~~\__\~~~
          return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
        else                                    //                                                ~~~\__\~~\__\~~~~~~~~~~~~~~
          return (PixelLine[3].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else  return 0;
}
//圆环
/*
float32 ModeIdentification(struct PixelLineStruct *PixelLine,uint8 PixelStep){
  //uint8 i;
  //float32 ExpectOffsetTmp=0;
 
  static uint8 AllHigh=0;
  //if(AllHigh==2)return Offset;
  if(AllHigh==1)State=100;
  else if(AllHigh>=2 && AllHigh<=CNT)State=200;
  else State=0;
  if(PixelStep==1){//单跳变，丢线
    //AllHigh=0;
    if(PixelLine[0].Type==UP){//                                 ____\~~~~~~
      //ExpectOffsetTmp=PixelLine[0].Position+(RoadWidth/2.0)-63.5;
      if(AllHigh>=2 && AllHigh<=CNT){
        AllHigh++;
        return Offset;
      }else if(AllHigh>CNT){
        AllHigh=0;
        return 0;
      }else
      if((128-PixelLine[0].Position)>=AllHighWidthThreshold){
        AllHigh=1;
        return 0;
      }
      if(AllHigh==2)
      AllHigh=0;
      return (PixelLine[0].Position+(RoadWidth/2.0)-63.5);
    }else{//                                                      ~~~~~\______
      if(AllHigh>=2 && AllHigh<=CNT){
        AllHigh++;
        return Offset;
      }else if(AllHigh>CNT){
        AllHigh=0;
        return 0;
      }else if(PixelLine[0].Position>=AllHighWidthThreshold){
        AllHigh=1;
        return 0;
      }
      return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
    }
  }else if(PixelStep==2){//
    if(PixelLine[0].Type==UP){//                                 ______\~~~~Num~~~~\______   - Left
      if(AllHigh>=2 && AllHigh<=CNT){
        AllHigh++;
        return Offset;
      }else if(AllHigh>CNT)AllHigh=0;

      if((PixelLine[1].Position-PixelLine[0].Position)>=RoadWidthThreshold){
        if((PixelLine[1].Position-PixelLine[0].Position)>=AllHighWidthThreshold){
          AllHigh=1;
          return 0;
        }
        AllHigh=0;
        RoadWidth=PixelLine[1].Position-PixelLine[0].Position;
        
        return (PixelLine[1].Position+PixelLine[0].Position)/2.0-63.5;
      }else //                                                   _____\~\____
        return 0;
    }else{//
      if((PixelLine[1].Position-PixelLine[0].Position)<RoadWidthThreshold){//阈值！！！！！     ~~~~~~~~~~~~~\_\~~~~~~~~~
       if(AllHigh>=2 && AllHigh<=CNT){
        AllHigh++;
        return Offset;
      }else if(AllHigh>CNT)AllHigh=0;
        return 0;
      }else{
        if(AllHigh>=2 && AllHigh<=CNT){
        AllHigh++;
        return Offset;
      }else if(AllHigh>CNT)AllHigh=0;
        if(AllHigh==1){
        //State=100;
        AllHigh++;
        return Offset;
        }
      if(PixelLine[0].Position>(127-PixelLine[1].Position))//     ~~~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
      else //                                                   ~~~\____\~~~~~~~~~~~~~~~~~~
        return (PixelLine[1].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else if(PixelStep==3){//
      if(AllHigh>=2 && AllHigh<=CNT){
        AllHigh++;
        return Offset;
      }else if(AllHigh>CNT)AllHigh=0;
    AllHigh=0;
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(127-PixelLine[2].Position))//__\~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                 //                  ___\~~\__\~~~~~~~~~~~~~~~~
        return (PixelLine[2].Position+(RoadWidth/2.0)-63.5);
    }else{
      if((PixelLine[2].Position-PixelLine[1].Position)>PixelLine[0].Position)//~~~~\__\~~~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[1].Position)/2.0-63.5;
      else                              //                                  ~~~~~~~~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
    }
  }else if(PixelStep==4){
    AllHigh=0;
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(PixelLine[3].Position-PixelLine[2].Position))//__\~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                              //                        __\~~\__\~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[3].Position)/2.0-63.5; 
    }else{// 
      if((PixelLine[2].Position-PixelLine[1].Position)>RoadWidthThreshold)//                      ~~~\__\~~~~~~Num~~~~~~~\__\~~
        return (PixelLine[1].Position+PixelLine[2].Position)/2.0-63.5;
      else{
        if(PixelLine[0].Position>(127-PixelLine[3].Position))//                                  ~~~~~~~~~~~~~~\__\~~\__\~~~
          return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
        else                                    //                                                ~~~\__\~~\__\~~~~~~~~~~~~~~
          return (PixelLine[3].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else{//0 跳变
    if(PixelStep==0){
      AllHigh=1;
      return 0;
    }else   return 0;
  }
  
}
*/
/*
float32 ModeIdentification(struct PixelLineStruct *PixelLine,uint8 PixelStep){
  //uint8 i;
  //float32 ExpectOffsetTmp=0;
  if(PixelStep==1){//单跳变，丢线
    if(PixelLine[0].Type==UP)//                                 ____\~~~~~~
      //ExpectOffsetTmp=PixelLine[0].Position+(RoadWidth/2.0)-63.5;
      return (PixelLine[0].Position+(RoadWidth/2.0)-63.5);
    else//                                                      ~~~~~\______
      return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
  }else if(PixelStep==2){//
    if(PixelLine[0].Type==UP){//                                 ______\~~~~Num~~~~\______   - Left
      if((PixelLine[1].Position-PixelLine[0].Position)>RoadWidthThreshold)
        return (PixelLine[1].Position+PixelLine[0].Position)/2.0-63.5;
      else //                                                   _____\~\____
        return 0;
    }else{//
      if((PixelLine[1].Position-PixelLine[0].Position)<RoadWidthThreshold)//阈值！！！！！     ~~~~~~~~~~~~~\_\~~~~~~~~~
        return 0;
      else{
      if(PixelLine[0].Position>(127-PixelLine[1].Position))//     ~~~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
      else //                                                   ~~~\____\~~~~~~~~~~~~~~~~~~
        return (PixelLine[1].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else if(PixelStep==3){//
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(127-PixelLine[2].Position))//__\~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                 //                  ___\~~\__\~~~~~~~~~~~~~~~~
        return (PixelLine[2].Position+(RoadWidth/2.0)-63.5);
    }else{
      if((PixelLine[2].Position-PixelLine[1].Position)>PixelLine[0].Position)//~~~~\__\~~~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[1].Position)/2.0-63.5;
      else                              //                                  ~~~~~~~~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
    }
  }else if(PixelStep==4){
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(PixelLine[3].Position-PixelLine[2].Position))//__\~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                              //                        __\~~\__\~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[3].Position)/2.0-63.5; 
    }else{// 
      if((PixelLine[2].Position-PixelLine[1].Position)>RoadWidthThreshold)//                      ~~~\__\~~~~~~Num~~~~~~~\__\~~
        return (PixelLine[1].Position+PixelLine[2].Position)/2.0-63.5;
      else{
        if(PixelLine[0].Position>(127-PixelLine[3].Position))//                                  ~~~~~~~~~~~~~~\__\~~\__\~~~
          return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
        else                                    //                                                ~~~\__\~~\__\~~~~~~~~~~~~~~
          return (PixelLine[3].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else  return 0;
}
*/
/***********************************************************************************************************/
/*
float32 ModeIdentification(struct PixelLineStruct *PixelLine,uint8 PixelStep){
  //uint8 i;
  //float32 ExpectOffsetTmp=0;
  static int8 Mode=0;
  if(PixelStep==1){//单跳变，丢线
    if(PixelLine[0].Type==UP)//                                 ____\~~~~~~
      //ExpectOffsetTmp=PixelLine[0].Position+(RoadWidth/2.0)-63.5;
      return (PixelLine[0].Position+(RoadWidth/2.0)-63.5);
    else//                                                      ~~~~~\______
      return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
  }else if(PixelStep==2){//
    if(PixelLine[0].Type==UP){//                                 ______\~~~~Num~~~~\______   - Left
      if((PixelLine[1].Position-PixelLine[0].Position)>RoadWidthThreshold){
        if(Mode==15 && (PixelLine[1].Position-PixelLine[0].Position)>
        Mode++;
        if(Mode>=15)Mode=15;
        return (PixelLine[1].Position+PixelLine[0].Position)/2.0-63.5;
      }
      else //                                                   _____\~\____
        return 0;
    }else{//
      if((PixelLine[1].Position-PixelLine[0].Position)<RoadWidthThreshold)//阈值！！！！！     ~~~~~~~~~~~~~\_\~~~~~~~~~
        return 0;
      else{
      if(PixelLine[0].Position>(127-PixelLine[1].Position))//     ~~~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
      else //                                                   ~~~\____\~~~~~~~~~~~~~~~~~~
        return (PixelLine[1].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else if(PixelStep==3){//
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(127-PixelLine[2].Position))//__\~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                 //                  ___\~~\__\~~~~~~~~~~~~~~~~
        return (PixelLine[2].Position+(RoadWidth/2.0)-63.5);
    }else{
      if((PixelLine[2].Position-PixelLine[1].Position)>PixelLine[0].Position)//~~~~\__\~~~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[1].Position)/2.0-63.5;
      else                              //                                  ~~~~~~~~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
    }
  }else if(PixelStep==4){
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(PixelLine[3].Position-PixelLine[2].Position))//__\~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                              //                        __\~~\__\~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[3].Position)/2.0-63.5; 
    }else{// 
      if((PixelLine[2].Position-PixelLine[1].Position)>RoadWidthThreshold)//                      ~~~\__\~~~~~~Num~~~~~~~\__\~~
        return (PixelLine[1].Position+PixelLine[2].Position)/2.0-63.5;
      else{
        if(PixelLine[0].Position>(127-PixelLine[3].Position))//                                  ~~~~~~~~~~~~~~\__\~~\__\~~~
          return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
        else                                    //                                                ~~~\__\~~\__\~~~~~~~~~~~~~~
          return (PixelLine[3].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else  return 0;
}
*/
/*
float32 ModeIdentification(struct PixelLineStruct *PixelLine,uint8 PixelStep){
  //uint8 i;
  static int8 Mode=0;
  static float32 ExpectOffsetTmp=0;
  uint8 LeftLine=0,RightLine=0;
  if(Mode==21)State=20;
  if(Mode!=21)State=0;
  if(PixelStep==1){//单跳变，丢线
    if(PixelLine[0].Type==UP){//                                 ____\~~~~~~
      ExpectOffsetTmp=PixelLine[0].Position+(RoadWidth/2.0)-63.5+Offset;
      if(Mode<0){
        State=0;
        return ExpectOffsetTmp;
      }
      if(Mode==21)return ExpectOffsetTmp;
      if(Mode==20 && (ABS(LeftLine-PixelLine[0].Position)<5 && ABS(ExpectOffset-ExpectOffsetTmp)<8 && ABS(PixelLine[0].Position-(63.5-RoadWidth/2.0))<10)){
        Mode=21;
        //ExpectOffsetTmp+=Offset;
        return PixelLine[0].Position+(RoadWidth/2.0)-63.5;
      }
    else return ExpectOffsetTmp;
    }
      //return (PixelLine[0].Position+(RoadWidth/2.0)-63.5);
    else{//                                                      ~~~~~\______
      ExpectOffsetTmp=PixelLine[0].Position-(RoadWidth/2.0)-63.5-Offset;
      if(Mode<0){
        State=0;
        return ExpectOffsetTmp;
      }
      if(Mode==21)return ExpectOffsetTmp;
      if(Mode==20 && (ABS(RightLine-PixelLine[0].Position)<5 && ABS(ExpectOffset-ExpectOffsetTmp)<8 && ABS(PixelLine[0].Position-(63.5+RoadWidth/2.0))<10)){
        Mode=21;
        //ExpectOffsetTmp-=Offset;
        return PixelLine[0].Position-(RoadWidth/2.0)-63.5;
      }
    else return ExpectOffsetTmp;
    }
  }else if(PixelStep==2){//
    if(PixelLine[0].Type==UP){//                                 ______\~~~~Num~~~~\______   - Left
      if((PixelLine[1].Position-PixelLine[0].Position)>RoadWidthThreshold){

        if(Mode==21 && ABS((PixelLine[1].Position-PixelLine[0].Position)-RoadWidth)>25)return ExpectOffsetTmp;
        if(ABS(PixelLine[1].Position-PixelLine[0].Position-65)>25){
          Mode=-50;
          State=40;
          return (PixelLine[1].Position+PixelLine[0].Position)/2.0-63.5+Offset;
        }
         RoadWidth=PixelLine[1].Position-PixelLine[0].Position;
         LeftLine=PixelLine[0].Position;
         RightLine=PixelLine[1].Position;
        if(Mode==21)Mode=0;
        Mode++;
        if(Mode>=20)Mode=20;
        return (PixelLine[1].Position+PixelLine[0].Position)/2.0-63.5;
      }else //                                                   _____\~\____
        return 0;
    }else{//
      if((PixelLine[1].Position-PixelLine[0].Position)<RoadWidthThreshold)//阈值！！！！！     ~~~~~~~~~~~~~\_\~~~~~~~~~
        return 0;
      else{
      if(PixelLine[0].Position>(127-PixelLine[1].Position))//     ~~~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
      else //                                                   ~~~\____\~~~~~~~~~~~~~~~~~~
        return (PixelLine[1].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else if(PixelStep==3){//
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(127-PixelLine[2].Position))//__\~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                 //                  ___\~~\__\~~~~~~~~~~~~~~~~
        return (PixelLine[2].Position+(RoadWidth/2.0)-63.5);
    }else{
      if((PixelLine[2].Position-PixelLine[1].Position)>PixelLine[0].Position)//~~~~\__\~~~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[1].Position)/2.0-63.5;
      else                              //                                  ~~~~~~~~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
    }
  }else if(PixelStep==4){
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(PixelLine[3].Position-PixelLine[2].Position))//__\~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                              //                        __\~~\__\~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[3].Position)/2.0-63.5; 
    }else{// 
      if((PixelLine[2].Position-PixelLine[1].Position)>RoadWidthThreshold)//                      ~~~\__\~~~~~~Num~~~~~~~\__\~~
        return (PixelLine[1].Position+PixelLine[2].Position)/2.0-63.5;
      else{
        if(PixelLine[0].Position>(127-PixelLine[3].Position))//                                  ~~~~~~~~~~~~~~\__\~~\__\~~~
          return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
        else                                    //                                                ~~~\__\~~\__\~~~~~~~~~~~~~~
          return (PixelLine[3].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else  return 0;
}
*/
uint8 StraighWayIndentification(uint8 ExpectOffset,uint8 cnt){
  static int8 Mode=0;
  static uint8 ExpectOffsetLast=0;
  if(ABS(ExpectOffset-ExpectOffsetLast)<7)Mode++;
  else Mode--;
  if(Mode>=20)Mode=20;
  if(Mode<=0)Mode=0;
  if(Mode>=cnt)return 1;
  
  else return 0;
}
uint8 TrendIdentification(uint8 RoadWidth,uint8 cnt){
  static uint8 RoadWidthLast=0;
  static uint8 Mode=0;
  if(RoadWidth>=RoadWidthLast){
    RoadWidthLast=RoadWidth;
    Mode++;
  }else Mode=0;
  if(Mode>=cnt)return 1;
  else return 0;
}
/*
float32 ModeIdentification(struct PixelLineStruct *PixelLine,uint8 PixelStep){
  int8 Mode=0;
  float32 ExpectOffsetTmp=0;
  static uint8 Cnt=0;
  if(PixelStep==1){//单跳变，丢线
    if(PixelLine[0].Type==UP){//                                 ____\~~~~~~
      //ExpectOffsetTmp=PixelLine[0].Position+(RoadWidth/2.0)-63.5;
      Cnt=0;
      if(Mode==1 && ABS(128-PixelLine[0].Position-RoadWidth)>15){
        Mode=0;
        State=0;
        return (PixelLine[0].Position+(RoadWidth/2.0)-63.5)+Offset;
      }
      return (PixelLine[0].Position+(RoadWidth/2.0)-63.5);
    }else{//                                                      ~~~~~\______
      Cnt=0;
      if(Mode==1 && ABS(PixelLine[0].Position-RoadWidth)>15){
        Mode=0;
        State=0;
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5)-Offset;
      }
      return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
    }
  }else if(PixelStep==2){//
    if(PixelLine[0].Type==UP){//                                 ______\~~~~Num~~~~\______   - Left
      if((PixelLine[1].Position-PixelLine[0].Position)>RoadWidthThreshold){
        ExpectOffsetTmp=(PixelLine[1].Position+PixelLine[0].Position)/2.0-63.5;
        RoadWidth=PixelLine[1].Position-PixelLine[0].Position;
        Cnt++;
        if(Cnt>=5)Cnt=5;
        if(ABS(PixelLine[1].Position-PixelLine[0].Position-RoadWidth)>10 && Cnt>=3){
          Mode=1;
          State=100;
          return ExpectOffsetTmp+Offset;
        }
        RoadWidth=PixelLine[1].Position-PixelLine[0].Position;
        return ExpectOffsetTmp;
      }else //                                                   _____\~\____
        return 0;
    }else{//
      if((PixelLine[1].Position-PixelLine[0].Position)<RoadWidthThreshold)//阈值！！！！！     ~~~~~~~~~~~~~\_\~~~~~~~~~
        return 0;
      else{
        if(PixelLine[0].Position>(127-PixelLine[1].Position)){//     ~~~~~~~~~~~~~~\___\~~
      if(Mode==1 && ABS(PixelLine[0].Position-RoadWidth)>10 ){
        State=0;
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5)-Offset;
      }
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
      }else{ //                                                   ~~~\____\~~~~~~~~~~~~~~~~~~
              if(Mode==1 && ABS(128-PixelLine[1].Position-RoadWidth)>10){
        Mode=0;
        State=0;
        return (PixelLine[1].Position+(RoadWidth/2.0)-63.5)+Offset;
      }
        return (PixelLine[1].Position+(RoadWidth/2.0)-63.5);
      }
      }
  }else if(PixelStep==3){//
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(127-PixelLine[2].Position))//__\~~~~~~~~~~~~\___\~~
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                 //                  ___\~~\__\~~~~~~~~~~~~~~~~
        return (PixelLine[2].Position+(RoadWidth/2.0)-63.5);
    }else{
      if((PixelLine[2].Position-PixelLine[1].Position)>PixelLine[0].Position)//~~~~\__\~~~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[1].Position)/2.0-63.5;
      else                              //                                  ~~~~~~~~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
    }
  }else if(PixelStep==4){
    if(PixelLine[0].Type==UP){
      if((PixelLine[1].Position-PixelLine[0].Position)>(PixelLine[3].Position-PixelLine[2].Position))//__\~~~~~~~~~\___\~~\__
        return (PixelLine[0].Position+PixelLine[1].Position)/2.0-63.5;
      else                                                              //                        __\~~\__\~~~~~~~~~~~~\__
        return (PixelLine[2].Position+PixelLine[3].Position)/2.0-63.5; 
    }else{// 
      if((PixelLine[2].Position-PixelLine[1].Position)>RoadWidthThreshold)//                      ~~~\__\~~~~~~Num~~~~~~~\__\~~
        return (PixelLine[1].Position+PixelLine[2].Position)/2.0-63.5;
      else{
        if(PixelLine[0].Position>(127-PixelLine[3].Position))//                                  ~~~~~~~~~~~~~~\__\~~\__\~~~
          return (PixelLine[0].Position-(RoadWidth/2.0)-63.5);
        else                                    //                                                ~~~\__\~~\__\~~~~~~~~~~~~~~
          return (PixelLine[3].Position+(RoadWidth/2.0)-63.5);
      }
    }
  }else  return 0;
}
*/
