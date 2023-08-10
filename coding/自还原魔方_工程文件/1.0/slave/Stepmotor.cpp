#include "Stepmotor.h"
#include <StackArray.h>

//使能引脚设置函数
void StepMotor :: set_enpin(int user_enpin,int pin_state){
  enpin = user_enpin;
  pinMode(enpin,OUTPUT);
  if(pin_state == 0){
    digitalWrite(enpin,LOW);
  }else if(pin_state == 1){
    digitalWrite(enpin,HIGH);
  }else{
    Serial.println("Wrong pinstate input for enpin!");//串口报错，串口需要提前开启
  }
}

//方向引脚设置函数
void StepMotor :: set_dirpin(int user_dirpin,int pin_state){
  dirpin = user_dirpin;
  pinMode(dirpin,OUTPUT);
  if(pin_state == 0){
    digitalWrite(dirpin,LOW);
  }else if(pin_state == 1){
    digitalWrite(dirpin,HIGH);
  }else{
    Serial.println("Wrong pinstate input for dirpin!");//串口报错，串口需要提前开启
  }
}

//步进引脚设置函数
void StepMotor :: set_steppin(int user_steppin,int pin_state){
  steppin = user_steppin;
  pinMode(steppin,OUTPUT);
  if(pin_state == 0){
    digitalWrite(steppin,LOW);
  }else if(pin_state == 1){
    digitalWrite(steppin,HIGH);
  }else{
    Serial.println("Wrong pinstate input for steppin!");//串口报错，串口需要提前开启
  }
}

//电机转动控制函数
void StepMotor :: motor_rot(int user_pulsenum,int time_1,int time_2){
  for(int pulsenum = user_pulsenum;pulsenum > 0;pulsenum--){
    digitalWrite(steppin,HIGH);
    delay(time_1);
    digitalWrite(steppin,LOW);
    delay(time_2);
  }   
}

//转向设置
void StepMotor :: dir_change(int pin_state){
  if(pin_state == 0){
    digitalWrite(dirpin,LOW);
  }else{
    digitalWrite(dirpin,HIGH);
  }
}

//操作数入栈
void StepMotor :: remember(int user_input){
  memory.push(user_input);
}

//操作数出栈
int StepMotor :: recall(){
  int result = memory.pop();
  return result;
}

//堆栈检查
int StepMotor :: checkmemory(){
  return memory.isEmpty() == 0;
}
