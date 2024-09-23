#include <Arduino.h>
#include <WiFi.h>
// #include <PubSubClient.h>
#include "config.h"
// #include <CAN.h>
// #include <ESP32Servo.h>
// #include <ESP32PWM.h>
#include "esp32-hal-ledc.h"
#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "esp32-hal-ledc.h"
// #include <ESP32Servo.h>


int potValue;  // value from the analog pin

#define POT_METER 25

#define KHz_1 1000
#define MHz_1 1000000

#define IN3_PIN 14
#define IN4_PIN 27
#define ENB_PIN 26 // PWM



const byte ledPin = 12;
const uint32_t freq = 1700000; // 1.7Mhz
const uint8_t ledChannel = 0;
const uint8_t resolution = 3;
#define ENABLE 13


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Configuring PWM");
  ledcAttachPin(ledPin, ledChannel);
  ledcSetup(ledChannel, freq, resolution);
  ledcWrite(ledChannel, 4);
}

void loop() {}


// void setup()
// {
//   Serial.begin(9600);

//   analogWriteFrequency(5*KHz_1); 
//   // ledcAttach(ESC_PIN, 10*1000, 8);
//   pinMode(IN3_PIN, OUTPUT);
//   pinMode(IN4_PIN, OUTPUT);
//   digitalWrite(IN3_PIN, LOW);
//   digitalWrite(IN4_PIN, LOW);
// }

// void loop()
// {
//   int pot_value = analogRead(POT_METER);
//   int mmotorSpeed = map(pot_value, 0, 4095, 0, 255);
//   // Serial.printf("pot %d\n",pot_value);

//   // ESC.write(pot_value); 
//   // ledcWrite(ESC_PIN, mmotorSpeed);

//   analogWrite(ENB_PIN, mmotorSpeed);
//   digitalWrite(IN3_PIN, HIGH);
//   digitalWrite(IN4_PIN, LOW);
//   Serial.printf("%d\n",mmotorSpeed);
// }
