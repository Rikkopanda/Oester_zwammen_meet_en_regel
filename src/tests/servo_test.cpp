#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <CAN.h>
#include <ESP32Servo.h>
// #include <ESP32PWM.h>

Servo ESC;     // create servo object to control the ESC

int potValue;  // value from the analog pin

#define ESC_PIN 26
#define POT_METER 13

void setup()
{
  Serial.begin(9600);

  // Set the pins
  // CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);

  ESC.attach(ESC_PIN, 1000, 2000);
  ESC.write(0);
  delay(2000);  
  // start the CAN bus at 500 kbps
  // if (!CAN.begin (500E3)) {
  //   Serial.printf ("Starting CAN failed!");
  //   while (1);
  // }
  // else {
  //   Serial.printf ("CAN Initialized");
  // }
}

void loop()
{
  int pot_value = analogRead(POT_METER);
  int mmotorSpeed = map(pot_value, 0, 4095, 0, 180);

  ESC.write(mmotorSpeed); 
  Serial.printf("%d\n",mmotorSpeed);
}
