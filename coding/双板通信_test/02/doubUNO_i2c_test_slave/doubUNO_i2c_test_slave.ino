#include <Wire.h>

int my_address = 2;
//int remember_or_recall = 0;//工作模式标志
//int called_or_not = 1;
int finish_or_not; 

void setup() {
//串口通讯  
  Serial.begin(115200);
  Serial.println("Serial begin!");
//I2C通讯  
  Wire.begin(my_address); 
  Serial.println("I2c communication is on!");
//声明接收反应函数和请求反应函数
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}  

void loop() {

}

void requestEvent() {
  while(finish_or_not){
    delay(10);
  }
  Serial.println("slave knocks back");
  Wire.write(1);
}

void receiveEvent(int howMany){
//  int flag = 0;
//  while(!flag){
  byte call;
  while(Wire.available()>0){
    Serial.println("being called now");
    call = Wire.read();
  }
    if(call == 1){//记忆模式，一号板先工作
      finish_or_not = 1;//工作时不能动
      Serial.println("sensor 5-6 finish");
      finish_or_not = 0;
      Serial.println("slave finish work");
//      remember_or_recall = 0;
    }else if(call == 2){//还原模式，二号板先工作
      finish_or_not = 1;//工作时不能动
      Serial.println("motor 6-5 finish");
      finish_or_not = 0;
      Serial.println("slave finish work");
//      remember_or_recall = 1;
    }
  
//    called_or_not == 0;
  
//  }
}
