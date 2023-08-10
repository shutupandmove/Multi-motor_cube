#include "Stepmotor.h"

StepMotor motor_2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Initialize the motor!");
  motor_2.set_enpin(5,0);
  motor_2.set_dirpin(6,0);
  motor_2.set_steppin(7,0);
  Serial.println("Motor has been initialized!");
  delay(1000);
}

void loop() {
//工作模式写入检测。串口输入reset还原魔方，输入remember记忆变形，无效输入、不输入则不运作
//  if(Serial.available()>0){
//    String str;
//    while(Serial.available()>0){
//       str = Serial.readStringUntil('\n');
//    }
//    if(str == "cw"){//转还原模式
//      motor_2.set_dirpin(3,0);
//      Serial.println("Rotate clockwise!");
//      motor_2.motor_rot(200,5,5);
//      delay(2000);
//    }else if(str == "ccw"){//转记忆模式
//      motor_2.set_dirpin(3,1);
//      Serial.println("Rotate counterclockwise!");
//      motor_2.motor_rot(200,5,5);
//      delay(2000);
//    }else{//无效输入。工作模式不变
//      Serial.println("Invalid input!");
//    }
//  }

motor_2.motor_rot(1305,10,10);
delay(3000);
}
