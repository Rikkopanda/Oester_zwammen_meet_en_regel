#include <Arduino.h>
#include "config.h"
#include <CAN.h>

const int MySerialRX = 16;
const int MySerialTX = 17;

void setup()
{
  Serial.begin(115200);

  // Set the pins
  CAN.setPins (9, 10);

  // start the CAN bus at 500 kbps
  if (!CAN.begin(50E3)) {
    Serial.printf ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.printf ("CAN Initialized");
  }
}

void loop()
{
  canReceiver();
}

// HardwareSerial MySerial(1); // define a Serial for UART1


// void setup() 
// {
//     Serial.begin(115200);

// 	// initialize the Serial to the pins
//     MySerial.begin(50E3, MySerialRX, MySerialTX);
// }

// void loop() 
// {
//     uint8_t byteFromSerial = 0;

// 	// here we could use our MySerial normally
//     while (MySerial.available() > 0) {
//         byteFromSerial = MySerial.read();
//         // and whatever
//         Serial.printf("received : %d\n",  byteFromSerial);
//     }
// }





void canReceiver() {
  // try to parse packet
  int packetSize = CAN.parsePacket();
  Serial.printf("return packetSize: %d\n", packetSize);

  if (packetSize) {
    // received a packet
    Serial.print ("Received ");

    if (CAN.packetExtended()) {
      Serial.print ("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print ("RTR ");
    }

    Serial.print ("packet with id 0x");
    Serial.print (CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print (" and requested length ");
      Serial.println (CAN.packetDlc());
    } else {
      Serial.print (" and length ");
      Serial.println (packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print ((char) CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }
}