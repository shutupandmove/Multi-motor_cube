#include <Wire.h>

int slave_ready = 1;
int my_address = 2;
int master_order = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(my_address);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Slave is ready!");
  Wire.onReceive(receiveEvent);
  delay(500);
  if(slave_ready == 0){
    Serial.println("Slave is working!");
    pinMode(3,OUTPUT);
    digitalWrite(3,HIGH);
    delay(1000);
    digitalWrite(3,LOW);
    Serial.println("Slave ends working!");
    slave_ready == 1;
  }
  
}

void receiveEvent(int howMany){
  Serial.println("Slave is reading!");
  while(0 < Wire.available()){
    master_order = Wire.read();
  }
  Serial.println("Slave ends reading!");
  if(master_order == 1){
    Serial.println("Slave receives an order!");
    slave_ready = 0;
  }
}
