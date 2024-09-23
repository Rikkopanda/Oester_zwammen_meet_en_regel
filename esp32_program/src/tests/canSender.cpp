#include <Arduino.h>
#include "config.h"
#include <CAN.h>

const int MySerialRX = 16;
const int MySerialTX = 17;

void setup()
{
  Serial.begin(115200);

  // Set the pins
  CAN.setPins (MySerialRX, MySerialTX);

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
    canSender();
}

// HardwareSerial MySerial(1); // define a Serial for UART1



// void setup() 
// {
//     Serial.begin(115200);

// 	// initialize the Serial to the pins
//     MySerial.begin(50E3, MySerialRX, MySerialTX);
// }
// int i = 0;
// void loop() 
// {
//     uint8_t byteFromSerial = 0;

// 	// here we could use our MySerial normally
//     MySerial.printf("Hello : %d\n",  i);
//     delay(500);
//     i++;
// }

void canSender() {
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  Serial.print ("Sending packet ... ");

  int ret = CAN.beginPacket (0x12);  //sets the ID and clears the transmit buffer

  Serial.printf("return beginPacket: %d\n", ret);
  
  if (ret == 1)
    return;
  
  // CAN.beginExtendedPacket(0xabcdef);
  CAN.write ('1'); //write data to buffer. data is not sent until endPacket() is called.
  CAN.write ('2');
  CAN.write ('3');
  CAN.write ('4');
  CAN.write ('5');
  CAN.write ('6');
  CAN.write ('7');
  CAN.write ('8');
  CAN.endPacket();
  Serial.printf("return endPacket: %d\n", ret);

  //RTR packet with a requested data length
  CAN.beginPacket (0x12, 3, true);
  CAN.endPacket();

  Serial.printf ("done");

  delay (1000);
}
