#include <Arduino.h>
#include "config.h"
#include <CAN.h>

// HardwareSerial Serial2(1); // define a Serial for UART1
const int MySerialRX = 16;
const int MySerialTX = 17;

void setup() {

  CAN.setPins (MySerialRX, MySerialTX);

  // start the CAN bus at 500 kbps
  if (!CAN.begin(50E3)) {
    Serial.printf ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.printf ("CAN Initialized");
  }

  Serial.begin(115200);  // Start the serial communication
  // Serial2.begin(115200, SERIAL_8N1, MySerialRX, MySerialTX);  // UART2 with RX on GPIO 16 and TX on GPIO 17
  delay(1000);  // Give some time for setup
}

void loop() {
  if (Serial.available()) {
    int incomingByte = Serial.read();
    Serial2.write(incomingByte);  // Send data from Serial to Serial2
  }

  if (Serial2.available()) {
    int incomingByte = Serial2.read();
    Serial.print("byte: ");
    Serial.write(incomingByte);  // Send data from Serial2 back to Serial
  }
}
// void setup() {
//   pinMode(17, OUTPUT);
// }

// void loop() {
//   digitalWrite(17, HIGH);   // Turn the LED on
//   delay(1000);                       // Wait for a second
//   digitalWrite(17, LOW);    // Turn the LED off
//   delay(1000);                       // Wait for a second
// }
