#include <Wire.h>
#include "Sensor.h"
#include "Stepmotor.h"

int remember_or_recall = 0;//工作模式标志

Sensor my_sensor;
StepMotor my_stepmotor;

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial begin!");
  
  Wire.begin();  
  Serial.println("I2c communication is on!");

  Serial.println("Initialize the motor!");
  my_stepmotor.set_enpin(2,0);
  my_stepmotor.set_dirpin(3,0);//假定低电平时与编码器正方向同向
  my_stepmotor.set_steppin(4,0);
  Serial.println("Motor has been initialized!");

  Serial.println("Finding the magnet!");
  TCA9548A(2);
  my_sensor.checkMagnetPresence();
  Serial.println("Magnet is found!");
  TCA9548A(2);
  my_sensor.ReadRawAngle(); 
  my_sensor.renewprestatus();
}

void loop() {
  //串口输入reset还原魔方，输入remember记忆变形，无效输入、不输入则不运作
  if(Serial.available()>0){
    String str;
    while(Serial.available()>0){
       str = Serial.readStringUntil('\n');
    }
    if(str == "reset"){
      Serial.println("Reset the cube!");
      remember_or_recall = 1;
    }else if(str == "remember"){
      Serial.println("Remember the change of the cube!");
      remember_or_recall = 0;
      my_sensor.ReadRawAngle();//及时刷新编码器读取角度值 
      my_sensor.renewprestatus();//及时刷新编码器读取角度值 
    }else{
      Serial.println("Invalid input!");
    }
  }
  //根据变量remember_or_recall的状态选择工作方式
  if(remember_or_recall == 0){//记忆模式，一次循环即记录一次旋转
    TCA9548A(0);
    my_sensor.ReadRawAngle(); 
    Serial.print("new = ");
    Serial.println(my_sensor.rawAngle_new);
    Serial.print("pre = ");
    Serial.println(my_sensor.rawAngle_pre);
    Serial.print("sub = ");
    Serial.println(my_sensor.rawAngle_sub);
    switch(my_sensor.checkrotation()){
      case 1://存入沿编码器正方向转90度恢复
             my_stepmotor.remember(1);
             Serial.println("存入沿顺时针方向转90度");
             my_sensor.renewprestatus();//及时刷新编码器前角度值 
             break;
      case 2://存入沿编码器负方向转90度恢复
             my_stepmotor.remember(2);
             Serial.println("存入沿逆时针方向转90度");
             my_sensor.renewprestatus();//及时刷新编码器前角度值 
             break;
      case 3://存入沿编码器正方向转180度恢复
             my_stepmotor.remember(3);
             Serial.println("存入沿顺时针方向转180度");
             my_sensor.renewprestatus();//及时刷新编码器前角度值 
             break;
      case 0://存入不旋转
             my_stepmotor.remember(0);//不旋转也要记录，多个电机运作时需要
             Serial.println("存入不旋转");
    }
    
    delay(5000);
    
  }else{//还原模式，一次循环恢复一步
    if(my_stepmotor.checkmemory()){//必须检查堆栈是否为空
      switch(my_stepmotor.recall()){
        case 0:Serial.println("不旋转");
               break;
        case 1:Serial.println("沿顺时针方向转90度");
               my_stepmotor.set_dirpin(3,0);
               my_stepmotor.motor_rot(200,10,10);
               break;
        case 2:Serial.println("沿逆时针方向转90度");
               my_stepmotor.set_dirpin(3,1);
               my_stepmotor.motor_rot(200,10,10);
               break;
        case 3:Serial.println("沿顺时针方向转1800度");
               my_stepmotor.set_dirpin(3,0);
               my_stepmotor.motor_rot(400,10,10);
               break;
      }
    }else{//堆栈为空，强制转入记忆模式
      Serial.println("The cube has been reset!");
      Serial.println("Remember the change of the cube!");
      remember_or_recall = 0;
      TCA9548A(0);
      my_sensor.ReadRawAngle();//及时刷新编码器读取角度值 
      my_sensor.renewprestatus();//及时刷新编码器读取角度值 
    }
    delay(5000);//电机转动需要时间
  }
}
