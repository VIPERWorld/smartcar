#include "ControlFilter.h"
/*************************/

/*****************************/
extern float32 Angle;
extern float32 AngleSpeed;
extern float32 GyroIntegral;
extern float32 TurnAngleSpeed;
extern float32 PhysicalZero;
extern int GyroStandard;
extern int TurnGyroStandard;
float32 TmpAngle=0;
/****for Kalman*****/
float32    AngleFilter=0;      //融合后得到的角度
float32  Gyro_x=0;

float32  Q_angle=0.001; //1--0.001 过程噪声的协方差//0.2//0.001
                        //数值越小，变化越慢
float32  Q_gyro =0.001;  //0.3--0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵,//0.1//0.05//0.4
                        //数值越小，变化越慢
float32  R_angle=0.5;    //0.01--0.5   测量噪声的协方差 既测量偏差 //0.5//0.4
                        //数值越小，变化越快
//float32  dt=0.005;       //0.001--0.005;为周期时间，对应5ms	                  
float32  C_0 = 1.0;
float32  Q_bias=0, Angle_err=0;
float32  PCt_0=0.0, PCt_1=0, E=0.0;
float32  K_0=0.0, K_1=0.0, t_0=0.0, t_1=0.0;
float32  Pdot[4] ={0,0,0,0};
float32  PP[2][2] = { { 1.0, 0 },{ 0, 1.0 } };
/****/
void GetAngle(){
  static int16 AngleArray[5]={0};
  //static int16 AngleSpeedArray[5]={0};
  int16 AngleTmp=0;
  int16 AngleSpeedTmp=0;
  //static float32 TmpAngle=0;
  //unsigned char  i=0;
  //AngleTmp=GetAccelZ();//+PhysicalZero;
  //for(i=1;i<=5;i++)AngleTmp+=-GetADC0AD12();//+PhysicalZero;;//+PhysicalZero;
  //AngleTmp/=5;
  AngleSpeedTmp=GetGyroZ()-GyroStandard;
  AngleTmp=GetAccelZ();//-PhysicalZero;
  //AngleSpeedTmp=GetADC0AD14()-GyroStandard;
  //for(i=1;i<=10;i++)AngleSpeedTmp+=GetADC0AD14()-GyroStandard;
  //AngleSpeedTmp/=10;
  TurnAngleSpeed=(float32)((GetGyroY()-TurnGyroStandard)/100.0);
  //TurnAngleSpeed=GetADC0AD13()-TurnGyroStandard;
  //AngleSpeedTmp=AngleSpeedAvg/5.0;
    /*
  AngleArray[0]=AngleArray[1];
  AngleArray[1]=AngleArray[2];
  AngleArray[2]=AngleArray[3];
  AngleArray[3]=AngleArray[4];
  AngleArray[4]=AngleTmp;
  AngleTmp=(int16)((AngleArray[0]+AngleArray[1]+AngleArray[2]+AngleArray[3]+AngleArray[4])*0.2);
    */
  if(AngleTmp>32760)AngleTmp=32760;
  else if(AngleTmp<-32760)AngleTmp=-32760;
  if(AngleSpeedTmp>32760)AngleSpeedTmp=32760;
  else if(AngleSpeedTmp<-32760)AngleSpeedTmp=-32760;
  if(TurnAngleSpeed>32760)TurnAngleSpeed=32760;
  else if(TurnAngleSpeed<-32760)TurnAngleSpeed=-32760;
  AngleSpeed=(float32)AngleSpeedTmp/100.0;
   /*
  AngleSpeedArray[0]=AngleSpeedArray[1];
  AngleSpeedArray[1]=AngleSpeedTmp;
  AngleSpeedTmp=(int16)((AngleSpeedArray[0]+AngleSpeedArray[1])*0.5);
  */
  GyroIntegral+=AngleSpeedTmp * dt * GyroCOE;
  //Angle=(int)AngleTmp;
  /****For Kalman******/
  
  KalmanFilter((float32)(AngleTmp),(float32)(AngleSpeedTmp * GyroCOE));
  Angle=AngleFilter/10.0;
  //GyroIntegral=(float32)AngleTmp;
  //AngleSpeed=(float32)AngleSpeedTmp;///10.0;
  //Angle=(float32)AngleTmp;
  
  //TmpAngle=K_Accel * AngleTmp + (1-K_Accel) * (TmpAngle + AngleSpeedTmp * GyroIntegralTime * GyroCOE);
  //Angle=TmpAngle/10.0;
  
  //AngleSpeed=(float32)AngleSpeedTmp/100.0;
  //AngleSpeed=Gyro_x/100.0;
}
void GetAngleSpeed(){
}

/*
void GetAngle(){
//  static float32 AngleArray[5]={0};
//  static float32 AngleSpeedArray[5]={0};
  int AngleTmp=0;
  int AngleSpeedTmp=0;
  unsigned char  i=0;
  //AngleTmp=GetAccelZ();//+PhysicalZero;
  //for(i=1;i<=5;i++)AngleTmp+=-GetADC0AD12();//+PhysicalZero;;//+PhysicalZero;
  //AngleTmp/=5;
  //AngleSpeedTmp=GetGyroZ()-GyroStandard;
  AngleTmp=-GetADC0AD12()+PhysicalZero;
  //AngleSpeedTmp=GetADC0AD14()-GyroStandard;
  for(i=1;i<=10;i++)AngleSpeedTmp+=GetADC0AD14()-GyroStandard;
  AngleSpeedTmp/=10;
  //TurnAngleSpeed=GetGyroY()-TurnGyroStandard;
  TurnAngleSpeed=GetADC0AD13()-TurnGyroStandard;
  //AngleSpeedTmp=AngleSpeedAvg/5.0;
  //if(AngleSpeedTmp>32760)AngleSpeedTmp=32760;
  //else if(AngleSpeedTmp<-32760)AngleSpeedTmp=-32760;
  GyroIntegral+=AngleSpeedTmp * dt * GyroCOE;
  //Angle=(int)AngleTmp;
 
  
  KalmanFilter((float32)(AngleTmp),(float32)(AngleSpeedTmp * GyroCOE));
  //Angle=(float32)AngleTmp;///10.0;
  Angle=AngleFilter;
  //GyroIntegral=(float32)AngleTmp;
  AngleSpeed=(float32)AngleSpeedTmp;///10.0;


}

*/


/******************/

/**************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void KalmanFilter(float32 Accel,float32 Gyro)		
{
    AngleFilter+=(Gyro - Q_bias) * dt;           //先验估计

    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
    Pdot[1]=- PP[1][1];
    Pdot[2]=- PP[1][1];
    Pdot[3]=Q_gyro;
            
    PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
    PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;
                    
    Angle_err =Accel-AngleFilter;     //zk-先验估计

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];
    
    PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;
            
    AngleFilter	+= K_0 * Angle_err;	 //后验估计
    Q_bias	+= K_1 * Angle_err;	 //后验估计
    Gyro_x   = Gyro - Q_bias;	         //输出值(后验估计)的微分=角速度    
}
/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
/*
void Yijielvbo(float angle_m, float gyro_m)
{
   angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * 0.005);
}
*/



