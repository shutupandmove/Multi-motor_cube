#include <Wire.h>
#include "Sensor.h"

Sensor sensor[2];

void setup() {
  Serial.begin(9600);
  Serial.println("Serial begin!");
  
  Wire.begin();
  Serial.println("I2C communication begin!");

  TCA9548A(0);
  sensor[0].checkMagnetPresence();
  Serial.println("The magnet of sensor one has been found!");
  TCA9548A(1);
  sensor[1].checkMagnetPresence();
  Serial.println("The magnet of sensor two has been found!");
}

void loop() {

}

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}
