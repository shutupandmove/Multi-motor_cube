#include <Wire.h>

int slave_done = 1;//二号板工作状态标记
int slave_address = 2;//二号板I2C通讯地址
int remember_or_recall = 0;//工作模式标志

void setup() {
//串口通讯  
  Serial.begin(115200);
  Serial.println("Serial begin!");
//I2C通讯  
  Wire.begin();
  Serial.println("I2c communication is on!");
  delay(100);
}

void loop() {
//工作模式写入检测。串口输入reset还原魔方，输入remember记忆变形，无效输入、不输入则不运作
  if(Serial.available()>0){
    String str;
    while(Serial.available()>0){
       str = Serial.readStringUntil('\n');
    }
    if(str == "reset"){//转还原模式
      Serial.println("Reset the cube!");
      remember_or_recall = 1;
    }else if(str == "remember"){//转记忆模式
      Serial.println("Remember the change of the cube!");
      remember_or_recall = 0;
    }else{//无效输入。工作模式不变
      Serial.println("Invalid input!");
    }
  }

  if(remember_or_recall == 0){
    //1-4sensor
    Serial.println("sensor 1-4 check");
    delay(5000);
    //call slave
    Serial.println("call slave");
    Wire.beginTransmission(slave_address); 
    Wire.write(1); //记忆模式
    Wire.endTransmission();
    //wait for slave to finish
    slave_done = 0;
    Wire.requestFrom(slave_address,1);
    while (Wire.available()){
      byte knock = Wire.read(); 
      if(knock == 1){
        slave_done = 1;
      }
    }
    
    Serial.println("slave finished");
  }else{
    //call slave
    Serial.println("call slave");
    delay(5000);
    Wire.beginTransmission(slave_address); 
    Wire.write(2); //还原模式
    Wire.endTransmission(); 
    delay(5000);
    //wait for slave to finish
    slave_done = 0;
    Wire.requestFrom(slave_address,1);
    while (Wire.available()){
      byte knock = Wire.read(); 
      if(knock == 1){
        slave_done = 1;
      }
    }
    delay(5000); 
    Serial.println("slave finished"); 
    delay(5000);
    //motor 4-1
    Serial.println("motor 4-1 finish");
    delay(5000);
  }
}
