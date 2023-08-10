#include <Wire.h>
#include "Sensor.h"
#include "Stepmotor.h"

int slave_done = 1;//二号板工作状态标记
int slave_address = 2;//二号板I2C通讯地址
int remember_or_recall = 0;//工作模式标志

Sensor sensor[4];//一号板定义装置四组，剩下两组在二号板
StepMotor stepmotor[4];

//I2C扩展用到的函数。在触及同I2C地址固件的调用前调用该函数即可，注意在扩展版上的序号与编码器在程序中的序号并不对应。
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

void setup() {
//串口通讯  
  Serial.begin(115200);
  Serial.println("Serial begin!");
//I2C通讯  
  Wire.begin();
  Serial.println("I2c communication is on!");
  delay(100);
//电机初始化
  Serial.println("Initialize the motor one!");
  stepmotor[0].set_enpin(2,0);
  stepmotor[0].set_dirpin(3,0);//假定低电平时与编码器正方向同向
  stepmotor[0].set_steppin(4,0);
  Serial.println("Motor one has been initialized!");
  delay(100);
  Serial.println("Initialize the motor two!");
  stepmotor[1].set_enpin(5,0);
  stepmotor[1].set_dirpin(6,0);//假定低电平时与编码器正方向同向
  stepmotor[1].set_steppin(7,0);
  Serial.println("Motor two has been initialized!");
  delay(100);
  Serial.println("Initialize the motor three!");
  stepmotor[2].set_enpin(8,0);
  stepmotor[2].set_dirpin(9,0);//假定低电平时与编码器正方向同向
  stepmotor[2].set_steppin(10,0);
  Serial.println("Motor three has been initialized!");
  delay(100);
  Serial.println("Initialize the motor four!");
  stepmotor[3].set_enpin(11,0);
  stepmotor[3].set_dirpin(12,0);//假定低电平时与编码器正方向同向
  stepmotor[3].set_steppin(13,0);
  Serial.println("Motor four has been initialized!");
  delay(100);
//检测磁钢状态
  Serial.println("Finding the magnets!");
  for(int i = 0;i < 2;i ++){
    TCA9548A(i);
    sensor[i].checkMagnetPresence();
    //给编码器两个角度记录变量赋初值
    sensor[i].ReadRawAngle(); 
    sensor[i].renewprestatus();
    Serial.print("Magnet ");
    Serial.print(i+1);
    Serial.println(" found!");
    delay(100);
  }
  Serial.println("Magnets are all found!");
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
      for(int i = 0;i < 4;i ++){
        TCA9548A(i);
        //给编码器两个角度记录变量赋初值
        sensor[i].ReadRawAngle(); 
        sensor[i].renewprestatus();//及时刷新编码器读取角度值 
      }
    }else{//无效输入。工作模式不变
      Serial.println("Invalid input!");
    }
  }

//工作模式判断    
  if(remember_or_recall == 0){//记忆模式，一次循环即记录一次旋转
    for(int n = 0;n < 2;n ++){//正序遍历所有编码器
      TCA9548A(n);//用编码器之前都调用一下获得对应位置
      delay(100);
      sensor[n].ReadRawAngle(); 
      delay(100);
      switch(sensor[n].checkrotation()){
        case 1://存入沿编码器正方向转90度恢复
               stepmotor[n].remember(1);
               Serial.print(n+1);
               Serial.print("号电机");
               Serial.println("存入沿顺时针方向转90度");
               sensor[n].renewprestatus();//及时刷新编码器前角度值 
               break;
        case 2://存入沿编码器负方向转90度恢复
               stepmotor[n].remember(2);
               Serial.print(n+1);
               Serial.print("号电机");
               Serial.println("存入沿逆时针方向转90度");
               sensor[n].renewprestatus();//及时刷新编码器前角度值 
               break;
        case 3://存入沿编码器正方向转180度恢复
               stepmotor[n].remember(3);
               Serial.print(n+1);
               Serial.print("号电机");
               Serial.println("存入沿顺时针方向转180度");
               sensor[n].renewprestatus();//及时刷新编码器前角度值 
               break;
        case 0://存入不旋转
               stepmotor[n].remember(0);//不旋转也要记录，多个电机运作时需要
               Serial.print(n+1);
               Serial.print("号电机");
               Serial.println("存入不旋转");
      }
    }
    
    //呼叫2号板并等待回应，完成整轮遍历
    Serial.println("Calling board 2");
    slave_done = 0;
    Wire.beginTransmission(slave_address); 
    Wire.write(1); //记忆模式
    Wire.endTransmission(); 
    delay(100);
    Serial.println("Waiting for a knock");
    while(!slave_done){  
      Wire.requestFrom(slave_address,1);
      while (Wire.available()){
        int knock = Wire.read(); 
        if(knock == 1) slave_done = 1;
      } 
    }
    Serial.println("receives a knock");
  }else{//还原模式，一次循环恢复一步
    //呼叫2号板并等待回应，等待完成整轮遍历
    Wire.beginTransmission(slave_address); 
    Wire.write(2); //还原模式
    Wire.endTransmission(); 
    slave_done = 0;
    while(!slave_done){ 
      Wire.requestFrom(slave_address,1);
      while (Wire.available()){
        int knock = Wire.read(); 
        if(knock == 1) slave_done = 1;
      } 
    }
    for(int m = 3;m > -1;m --){//逆序遍历所有电机
      
      if(stepmotor[m].checkmemory()){//必须检查堆栈是否为空
        
        switch(stepmotor[m].recall()){
          case 0:Serial.print(m+1);
                 Serial.print("号电机");
                 Serial.println("不旋转");
                 break;
          case 1:Serial.print(m+1);
                 Serial.print("号电机");
                 Serial.println("沿顺时针方向转90度");
                 stepmotor[m].dir_change(0);
                 stepmotor[m].motor_rot(200,10,10);
                 delay(5000);//每次电机转动都需要时间
                 break;
          case 2:Serial.print(m+1);
                 Serial.print("号电机");
                 Serial.println("沿逆时针方向转90度");
                 stepmotor[m].dir_change(1);
                 stepmotor[m].motor_rot(200,10,10);
                 delay(5000);//每次电机转动都需要时间
                 break;
          case 3:Serial.print(m+1);
                 Serial.print("号电机");
                 Serial.println("沿顺时针方向转180度");
                 stepmotor[m].dir_change(0);
                 stepmotor[m].motor_rot(400,10,10);
                 delay(5000);//每次电机转动都需要时间
                 break;
        } 
        
      }else{//堆栈为空，强制转入记忆模式
        Serial.println("The cube has been reset!");
        Serial.println("Remember the change of the cube!");
        remember_or_recall = 0;
        TCA9548A(m);
        sensor[m].ReadRawAngle();//及时刷新编码器读取角度值 
        sensor[m].renewprestatus();//及时刷新编码器读取角度值 
      }
      
    }
  } 
}
