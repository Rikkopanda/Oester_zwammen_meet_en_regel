
#include "config.h"
#include <Arduino.h>


// Define the CO2 sensor serial interface
HardwareSerial sensorSerial(2); // RX, TX

const uint8_t sendCommandPacket[9] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};


void setup()
{
	Serial.begin(115200);
 	sensorSerial.begin(9600, SERIAL_8N1, 16, 17);

	for (int i = 0; i <= 8; i++)
		sensorSerial.write(sendCommandPacket[i]);
	

}