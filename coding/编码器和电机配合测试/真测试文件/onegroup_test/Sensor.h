#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Arduino.h>

//AS5600编码器类设置为Sensor
class Sensor{
  public:
    void ReadRawAngle();//读取编码器寄存器中数据
    void checkMagnetPresence();//检查编码器磁钢状态
    void renewprestatus();//
    int checkrotation();
    int rawAngle_pre; //previously read raw angle 
    int rawAngle_new; //newly read raw angle
    int rawAngle_sub;//rawAngle_new - rawAngle_pre

  private:
    int magnetStatus = 0;//编码器磁钢状态
    int lowbyte; //raw angle 7:0
    word highbyte; //raw angle 7:0 and 11:8
//    int rawAngle_pre; //previously read raw angle 
//    int rawAngle_new; //newly read raw angle
//    int rawAngle_sub;//rawAngle_new - rawAngle_pre
};

//面朝魔方的面，顺时针转动编码器，角度值增大，此为编码器正方向

#endif
