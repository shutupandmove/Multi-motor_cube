#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_

#include <Arduino.h>
#include <StackArray.h>

//步进电机类设置为StepMotor
class StepMotor{
  public:
    void set_enpin(int user_enpin,int pin_state);//设置使能引脚在UNO板上的端口和状态
    void set_dirpin(int user_dirpin,int pin_state);//设置方向引脚
    void set_steppin(int user_steppin,int pin_state);//设置步进引脚
    void motor_rot(int user_pulsenum,int time_1,int time_2);//设置电机转动的脉冲信号，控制转动角度和速度
    void dir_change(int pin_state);//改变转向
    void remember(int user_input);//用户操作入栈
    int recall();//用户操作出栈
    int checkmemory();//检查堆栈是否为空
    
  private:
  //tmc2225驱动板需要UNO板直接输入使能呢个、方向和步进信号，此处变量做控制用
    int enpin;//使能引脚 LOW_ON HIGH_OFF
    int dirpin;//方向引脚 change direction
    int steppin;//步进引脚 HIGH > LOW:1 step
    StackArray <int> memory;
};

//驱动板电流调节在右手边，电机线从左往右按黄黑红白排列，面朝魔方一面，则dir引脚接低电平时电机顺时针旋转

#endif
