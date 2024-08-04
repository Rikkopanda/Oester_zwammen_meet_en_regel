/***
  Read Our Complete Guide: https://RandomNerdTutorials.com/esp32-bme680-sensor-arduino/
  Designed specifically to work with the Adafruit BME680 Breakout ----> http://www.adafruit.com/products/3660 These sensors use I2C or SPI to communicate, 2 or 4 pins are required to interface. Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit! Written by Limor Fried & Kevin Townsend for Adafruit Industries. BSD license, all text above must be included in any redistribution
***/
#include <Arduino.h>
void setup() {
  Serial.begin(115200);
  pinMode(26, OUTPUT);
}

void loop()
{
  digitalWrite(26, HIGH); // sets the digital pin 13 on
  delay(5000);            // waits for a second
  digitalWrite(26, LOW);  // sets the digital pin 13 off
  delay(5000);  
}
