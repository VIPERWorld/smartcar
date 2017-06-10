#ifndef _CONTROLPIXELPROCESS_H_
#define _CONTROLPIXELPROCESS_H_ 
#include "AllInclude.h"
#define ThresholdNearOffset 0
#define ThresholdForwardOffset 0
#define NearPixelHighThreshold 235
#define NearPixelLowThreshold 80
#define ForwardPixelHighThreshold 150
//#define RoadWidth 80
#define AllHighWidthThreshold 115
#define RoadWidthThreshold 10
#define CircleWidthThreshold 10
#define Offset 15
#define ABS(x) (x>=0?x:-x)
#define CNT 1
enum PixelLineType{
  UP,
  DOWN
};
struct PixelLineStruct{
  enum PixelLineType Type;
  uint8 Position;
};
int16 PixelForwardIndex(uint8 *Pixel);
uint8 PixelBinaryzation();
uint8 PixelIndex(uint8 Pixel[],struct PixelLineStruct *PixelLine,uint8 length);
float32 ModeIdentification(struct PixelLineStruct *PixelLine,uint8 PixelStep);
uint8 TrendIdentification(uint8 RoadWidth,uint8 cnt);
uint8 StraighWayIndentification(uint8 ExpectOffset,uint8 cnt);
#endif