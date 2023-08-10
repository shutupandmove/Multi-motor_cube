#include "Sensor.h"
#include <Wire.h>

//读取编码器寄存器中数据
void Sensor :: ReadRawAngle(){
  //7:0 - bits
  Wire.beginTransmission(0x36); //connect to the sensor
  Wire.write(0x0D); //figure 21 - register map: Raw angle (7:0)
  Wire.endTransmission(); //end transmission
  Wire.requestFrom(0x36, 1); //request from the sensor
  
  while(Wire.available() == 0); //wait until it becomes available 
  lowbyte = Wire.read(); //Reading the data after the request
  //11:8 - 4 bits
  Wire.beginTransmission(0x36);
  Wire.write(0x0C); //figure 21 - register map: Raw angle (11:8)
  Wire.endTransmission();
  Wire.requestFrom(0x36, 1);
  
  while(Wire.available() == 0);  
  highbyte = Wire.read();
  //4 bits have to be shifted to its proper place as we want to build a 12-bit number
  highbyte = highbyte << 8; //shifting to left
  rawAngle_new = highbyte | lowbyte; //int is 16 bits (as well as the word)  
  rawAngle_sub = rawAngle_new - rawAngle_pre;
}

//检查编码器磁钢状态
void Sensor :: checkMagnetPresence(){
  //This function runs in the setup() and it locks the MCU until the magnet is not positioned properly
  while((magnetStatus & 32) != 32) //while the magnet is not adjusted to the proper distance - 32: MD = 1
  {
    magnetStatus = 0; //reset reading

    Wire.beginTransmission(0x36); //connect to the sensor
    Wire.write(0x0B); //figure 21 - register map: Status: MD ML MH
    Wire.endTransmission(); //end transmission
    Wire.requestFrom(0x36, 1); //request from the sensor

    while(Wire.available() == 0); //wait until it becomes available 
    magnetStatus = Wire.read(); //Reading the data after the request
  }
}

void Sensor :: renewprestatus(){
  rawAngle_pre = rawAngle_new;
}

int Sensor :: checkrotation(){
  if((rawAngle_sub >= -2584&&rawAngle_sub <= -1984)||(rawAngle_sub >= 1512&&rawAngle_sub <= 2112)){
    //面朝魔方面沿顺时针方向转90度恢复
    return 1;
  }
//  else if((rawAngle_sub >= 2084&&rawAngle_sub <= 2484)||(rawAngle_sub >= -2048&&rawAngle_sub <= -1648)){
//    //面朝魔方面沿逆时针方向转90度恢复
//    return 2;
//  }
  else if((rawAngle_sub >= -3402&&rawAngle_sub <= -2802)||(rawAngle_sub >= 2802&&rawAngle_sub <= 3402)||(rawAngle_sub >= -1294&&rawAngle_sub <= -694)||(rawAngle_sub >= 694&&rawAngle_sub <= 1294)){
    //面朝魔方面沿顺时针方向转180度恢复
    return 3;
  }else{
    //不旋转
    return 0;
  }
}
