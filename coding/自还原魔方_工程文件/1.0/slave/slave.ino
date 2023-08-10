#include <Wire.h>
#include "Sensor.h"
#include "Stepmotor.h"

int my_address = 2;
int remember_or_recall = 0;//工作模式标志

Sensor sensor[2];//五六两组装置在二号板
StepMotor stepmotor[2];

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
  Wire.begin(my_address); 
  Serial.println("I2c communication is on!");
//电机初始化
  Serial.println("Initialize the motor five!");
  stepmotor[0].set_enpin(2,0);
  stepmotor[0].set_dirpin(3,0);//假定低电平时与编码器正方向同向
  stepmotor[0].set_steppin(4,0);
  Serial.println("Motor five has been initialized!");
  Serial.println("Initialize the motor six!");
  stepmotor[1].set_enpin(5,0);
  stepmotor[1].set_dirpin(6,0);//假定低电平时与编码器正方向同向
  stepmotor[1].set_steppin(7,0);
  Serial.println("Motor six has been initialized!");
//检测磁钢状态
  Serial.println("Finding the magnets!");
  for(int i = 0;i < 2;i ++){
    TCA9548A(i + 4);//六组装置的编码器共用一个I2c扩展板，五六组装置在扩展板上编号仍是末位，但在二号板中编号是首位
    sensor[i].checkMagnetPresence();
    //给编码器两个角度记录变量赋初值
    sensor[i].ReadRawAngle(); 
    sensor[i].renewprestatus();
    Serial.print("Magnet ");
    Serial.print(i+5);
    Serial.println(" found!");
  }
  Serial.println("Magnets are all found!");
  my_delay(200);
}

void loop() {

//等待1号板呼叫
  Serial.println("waiting for calling");
  Wire.onReceive(receiveEvent);
  Serial.println("receive a call");

  if(remember_or_recall == 0){//记忆模式，一次循环即记录一次旋转
//    Serial.println("m");
    for(int n = 0;n < 2;n ++){//正序遍历所有编码器
//      Serial.println("m");
      TCA9548A(n + 4);//用编码器之前都调用一下获得对应位置
//      Serial.println("m");
      sensor[n].ReadRawAngle(); 
//      Serial.println("m");
      switch(sensor[n].checkrotation()){
        case 1://存入沿编码器正方向转90度恢复
               stepmotor[n].remember(1);
               Serial.print(n+5);
               Serial.print("号电机");
               Serial.println("存入沿顺时针方向转90度");
               sensor[n].renewprestatus();//及时刷新编码器前角度值 
               break;
        case 2://存入沿编码器负方向转90度恢复
               stepmotor[n].remember(2);
               Serial.print(n+5);
               Serial.print("号电机");
               Serial.println("存入沿逆时针方向转90度");
               sensor[n].renewprestatus();//及时刷新编码器前角度值 
               break;
        case 3://存入沿编码器正方向转180度恢复
               stepmotor[n].remember(3);
               Serial.print(n+5);
               Serial.print("号电机");
               Serial.println("存入沿顺时针方向转180度");
               sensor[n].renewprestatus();//及时刷新编码器前角度值 
               break;
        case 0://存入不旋转
               stepmotor[n].remember(0);//不旋转也要记录，多个电机运作时需要
               Serial.print(n+5);
               Serial.print("号电机");
               Serial.println("存入不旋转");
      }
      Serial.println("m");
    }
    delay(5000);
  }else{//还原模式，一次循环恢复一步
    Serial.println("m");
    for(int m = 1;m > -1;m --){//逆序遍历所有电机
      
      if(stepmotor[m].checkmemory()){//必须检查堆栈是否为空
        
        switch(stepmotor[m].recall()){
          case 0:Serial.print(m+5);
                 Serial.print("号电机");
                 Serial.println("不旋转");
                 break;
          case 1:Serial.print(m+5);
                 Serial.print("号电机");
                 Serial.println("沿顺时针方向转90度");
                 stepmotor[m].dir_change(0);
                 stepmotor[m].motor_rot(200,10,10);
                 delay(5000);//每次电机转动都需要时间
                 break;
          case 2:Serial.print(m+5);
                 Serial.print("号电机");
                 Serial.println("沿逆时针方向转90度");
                 stepmotor[m].dir_change(1);
                 stepmotor[m].motor_rot(200,10,10);
                 delay(5000);//每次电机转动都需要时间
                 break;
          case 3:Serial.print(m+5);
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
        TCA9548A(m + 4);
        sensor[m].ReadRawAngle();//及时刷新编码器读取角度值 
        sensor[m].renewprestatus();//及时刷新编码器读取角度值 
      }
      
    }
    
  }

//工作完成呼叫一号板
  Serial.println("Knocking board 1");
  Wire.begin(my_address);
  Wire.onRequest(requestEvent);
  my_delay(100);
}

void receiveEvent(int howMany){
  int flag = 0;//一号板传呼接收标志
  while(!flag){
    while(Wire.available()>1){
      int knock = Wire.read();
      if(knock == 1){//记忆模式，一号板先工作
        remember_or_recall = 0;
      }else if(knock == 2){//还原模式，二号板先工作
        remember_or_recall = 1;
      }
      flag = 1;
    }
  }
}

void requestEvent(){
  Wire.write(1);
}

void my_delay(int dtime){
  unsigned long curt = millis(),pre = curt,sub = 0;
  while(sub < dtime){
    curt = millis();
    sub = curt - pre;
  }
}
