#include "ControlCCD.h"
#include "common.h"

extern uint8 Pixel_1[128],Pixel_2[128];
uint8 Pixelfilter_1[128],Pixelfilter_2[128],PixelB_1[128],PixelB_2[128];
int8 leftline_1[2]={127},midline_1[2]={64},rightline_1[2]={0};
int8 leftline_2[2]={127},midline_2[2]={64},rightline_2[2]={0};
uint8 max_1,min_1,max_2,min_2;
uint8 loseline_1 = 0,loseline_2,oneflag_2,specialflag_2,lightflag_2 = 1,angle_2 = 0;

void CCDhandle(){
    uint8 i;
    //平滑处理
    for(i=1;i<127;i++){
        Pixelfilter_1[i] = (uint8)(0.15*Pixel_1[i-1]+0.7*Pixel_1[i]+0.15*Pixel_1[i+1]);
        Pixelfilter_2[i] = (uint8)(0.15*Pixel_2[i-1]+0.7*Pixel_2[i]+0.15*Pixel_2[i+1]);
    }
    //边界处理
    Pixelfilter_1[4] = Pixelfilter_1[5];   Pixelfilter_1[123] = Pixelfilter_1[122];
    Pixelfilter_1[3] = Pixelfilter_1[4];   Pixelfilter_1[124] = Pixelfilter_1[123];
    Pixelfilter_1[2] = Pixelfilter_1[3];   Pixelfilter_1[125] = Pixelfilter_1[124];
    Pixelfilter_1[1] = Pixelfilter_1[2];   Pixelfilter_1[126] = Pixelfilter_1[125];
    Pixelfilter_1[0] = Pixelfilter_1[1];   Pixelfilter_1[127] = Pixelfilter_1[126];

    Pixelfilter_2[4] = Pixelfilter_2[5];   Pixelfilter_2[123] = Pixelfilter_2[122];
    Pixelfilter_2[3] = Pixelfilter_2[4];   Pixelfilter_2[124] = Pixelfilter_2[123];
    Pixelfilter_2[2] = Pixelfilter_2[3];   Pixelfilter_2[125] = Pixelfilter_2[124];
    Pixelfilter_2[1] = Pixelfilter_2[2];   Pixelfilter_2[126] = Pixelfilter_2[125];
    Pixelfilter_2[0] = Pixelfilter_2[1];   Pixelfilter_2[127] = Pixelfilter_2[126];
    //寻找阈值(最大值最小值二值化)
    max_1 = min_1 = Pixelfilter_1[0];
    max_2 = min_2 = Pixelfilter_2[0];
    for(i=1;i<128;i++){
        if(Pixelfilter_1[i]>max_1)
          max_1 = Pixelfilter_1[i];
        if(Pixelfilter_1[i]<min_1)
          min_1 = Pixelfilter_1[i];
        if(Pixelfilter_2[i]>max_2)
          max_2 = Pixelfilter_2[i];
        if(Pixelfilter_2[i]<min_2)
          min_2 = Pixelfilter_2[i];
     
    }
    for(i=0;i<128;i++){
      PixelB_1[i] = (Pixelfilter_1[i] >= ((min_1+max_1)/2+0) ? 250:0);
      PixelB_2[i] = (Pixelfilter_2[i] >= ((min_2+max_2)/2+0) ? 250:0);
    }
     /*去除左边因反光出现的第一个小区域“亮”*/
 /*   uint8 black,blackflag,j;
    black = blackflag = 0;
    for(i=0;i<;i++){
      if(PixelB_2[i]==250){
        for(j=i;j<i+16;j++){     //16可调
          if(PixelB_2[j]==0){
            black=j;blackflag = 1;break;
          }
        }
      }
      if(blackflag)break;
    }
    if(blackflag){
      for(;i<=blackflag;i++)PixelB_2[i] = 0;
    }*/
    /*
    //丢线标记
    if(max_1-min_1<)
    */
    for(i=0;i<128;i++){
      if(Pixelfilter_2[i]<180)
        lightflag_2 = 0;
    }
    if(lightflag_2){
      for(i=0;i<128;i++){
        PixelB_2[i] = 250;
      }
     // angle_2 = 0;
    }
    else
      lightflag_2 = 1;
}
void Havemidline(void){
    uint8 i,leftedge_1,leftedge_2,rightedge_1,rightedge_2;
//CCD2
    //各类标志回零
    oneflag_2 = 0;specialflag_2 = 0;loseline_2 = 0;
    //边线初始化
    leftline_2[0] = leftline_2[1];
    midline_2[0] = midline_2[1];
    rightline_2[0] = rightline_2[1];
    leftline_2[1] = 0;
    midline_2[1] = 64;
    rightline_2[1] = 127;
    //左右跳边沿初始（反）标记
    leftedge_1 = 127;leftedge_2 = 127;
    rightedge_1 = 0;rightedge_2 = 0;
    for(i=1;i<127;i++)
    {
        if(PixelB_2[i]==PixelB_2[i-1] && PixelB_2[i]==0 && PixelB_2[i+1]==250 )
        {
            if( leftedge_1 == 127 )
                leftedge_1 = i;
            else
                leftedge_2 = i;
        }
        if ( leftedge_2 != 127 )
            break;
        //左线从左往右采，采到第二次就停止判断
    }
    for(i=126;i>0;i--)
    {
        if(PixelB_2[i]==PixelB_2[i+1] && PixelB_2[i]==0 && PixelB_2[i-1]==250 )
        {
            if( rightedge_1 == 0 )
                rightedge_1 = i;
            else
                rightedge_2 = i;
        }
        if ( rightedge_2 != 0 )
            break;
        //右线从右往左采，采到第二次就停止判断
    }
     /************************ CCD2、提取有用的跳边沿 ***********************/
    /* 左只有一跳变，右只有一跳变 ———— 正常赛道 */
    if ( leftedge_1 != 127 && leftedge_2 == 127 && rightedge_1 != 0 && rightedge_2 == 0 )
    {

        if((rightedge_1 - leftedge_1) >70)
          angle_2 = 0;
        leftline_2[1]  = leftedge_1;
        rightline_2[1] = rightedge_1;

    }

    /* 左只有一跳变，右无跳变     ———— 右丢线 */
    if ( leftedge_1 != 127 && leftedge_2 == 127 && rightedge_1 == 0 && rightedge_2 == 0 )
    {
        if(rightline_2[0] < 118)
        {
            angle_2 = 1;
        }
        else
        {
            leftline_2[1]  = leftedge_1;
            oneflag_2 = 1;
        }
    }

    /* 左无跳变，右只有一跳变     ———— 左丢线 */
    if ( leftedge_1 == 127 && leftedge_2 == 127 && rightedge_1 != 0 && rightedge_2 == 0 )
    {
        if(leftline_2[0] > 10)
        {
            angle_2 = 2;
        }
        else
        {
            rightline_2[1] = rightedge_1;
            oneflag_2 = 2;
        }
    }

    /* 左有两个跳变，右只有一跳变 ———— you扫其他 */
    if ( leftedge_1 != 127 && leftedge_2 != 127 && rightedge_1 != 0 && rightedge_2 == 0 )
    {
        leftline_2[1]  = leftedge_1;
        rightline_2[1] = rightedge_1;
    }

    /* 左只有一跳变，右有两个跳变 ———— zuo扫其他*/
    if ( leftedge_1 != 127 && leftedge_2 == 127 && rightedge_1 != 0 && rightedge_2 != 0 )
    {
        leftline_2[1]  = leftedge_1;
        rightline_2[1] = rightedge_1;
    }
    /* 左无跳变，右无跳变 ———— 完全丢线*/
    if ( leftedge_1 == 127 && leftedge_2 == 127 && rightedge_1 == 0 && rightedge_2 == 0 )
    {
        loseline_2 = 1;
    }
/*
    // 左有两个跳变，右有两个跳变 ———— 有障碍
    if ( leftedge_1 != 127 && leftedge_2 != 127 && rightedge_1 != 0 && rightedge_2 != 0 )
    {
        Flag_obs = 2;
        //障碍标志位

        // 左侧有障碍 
        if ( fabs( rightedge_2 - leftedge_1 ) < fabs( rightedge_1 - leftedge_2 ) )
        {
            leftline_2[1]  = leftedge_2 + Obs_Midline;
            rightline_2[1] = rightedge_1 + Obs_Midline;
        }

        // 右侧有障碍 
        if ( fabs( rightedge_2 - leftedge_1 ) > fabs( rightedge_1 - leftedge_2 ) )
        {
            leftline_2[1]  = leftedge_1 - Obs_Midline;
            rightline_2[1] = rightedge_2 - Obs_Midline;
        }
    }
*/
    /************************ CCD2、丢线处理 ***********************/
    /* 扫到外面，全丢线 */
    //遇到十字环路直行即可
    if (loseline_2 == 1)
    {
        midline_2[1]   = 64;//midline_2[0];
        leftline_2[1]  = 0;//leftline_2[0];
        rightline_2[1] = 127;//rightline_2[0];
        //继续采用上次的中线,左右线
    }

    /*丢线或扫到其他赛道，左线大于右线 */
    else
    {
        if ( leftline_2[1] > rightline_2[1] )
        {
            /* 左线丢失，右线为其他赛道 */
            if ( midline_2[0] < 64 )
            {
               // leftline_2[1] = leftline_2[0] + rightline_2[1] - rightline_2[0];//？？？？
                oneflag_2 = 2;
            }

            /* 右线丢失，左线为其他赛道 */
            if ( midline_2[0] > 64 )
            {
                //rightline_2[1]  = rightline_2[0] + leftline_2[1]- leftline_2[0];
                oneflag_2 = 1;
            }
        }
    }

    midline_2[1] = (leftline_2[1] + rightline_2[1])/2 + 0;
    if(oneflag_2==1)
      midline_2[1] = leftline_2[1] + 35;
    if(oneflag_2==2)
      midline_2[1] = rightline_2[1] - 35;
    if(angle_2==1)
      midline_2[1] = 124;
    if(angle_2==2)
      midline_2[1] = 0;
    if(oneflag_2 != 0||loseline_2 != 0)
      specialflag_2 = 1;
//CCD1
}


