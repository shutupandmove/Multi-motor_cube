#include <Wire.h>

int master_done = 0;
int slave_address = 2;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(master_done == 1){
    Serial.println("Master is ordering!");
    Wire.beginTransmission(slave_address);
    Wire.write(master_done);
    Wire.endTransmission();
    Serial.println("Master ends ordering!");
    master_done = 0;
    delay(500);    
  }else{
    //work
    Serial.println("Master is working!");
    pinMode(3,OUTPUT);
    digitalWrite(3,HIGH);
    delay(1000);
    digitalWrite(3,LOW);
    Serial.println("Master ends working!");
    master_done = 1;
  }
}
