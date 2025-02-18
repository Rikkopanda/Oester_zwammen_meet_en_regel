/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Arduino.h>

void setup() {
  Wire.begin(21, 22);
  Serial.begin(115200);
  Serial.printf("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.printf("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.printf("No I2C devices found\n");
  }
  else {
    Serial.printf("done\n");
  }
  delay(5000);          
}
