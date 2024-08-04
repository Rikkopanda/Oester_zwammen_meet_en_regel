#include "config.h"
#include <Arduino.h>

void read_co2_sensor()
{
	if (sensorSerial.available() >= 0)
	{
		uint8_t dataPacket[DATA_PACKET_SIZE];
		sensorSerial.readBytes(dataPacket, DATA_PACKET_SIZE);
		Serial.print("Raw data: ");
		for (int i = 0; i < DATA_PACKET_SIZE; i++)
		{
			Serial.printf("%02X ", dataPacket[i]);
		}
		if ((dataPacket[0] == 0x42 && dataPacket[1] == 0x4D))
		{
			int Co2Value = ((int)dataPacket[6] << 8) | (int)dataPacket[7];
			Serial.printf("CO2 (ppm): %d\n", Co2Value);
      		publish_int(CO2_topic, Co2Value);
		}
		else
			Serial.printf("Invalid data packet header!\n");
	}
}
