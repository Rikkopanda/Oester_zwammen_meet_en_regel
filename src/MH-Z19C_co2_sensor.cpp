
#include <Arduino.h>
#include "MHZ19_uart.h"
#include <SoftwareSerial.h>
#include <cstdint>
#include "config.h"

extern int Co2Value;

char getCheckSum_MHZ19C(uint8_t packet[DATA_PACKET_SIZE])
{
	char i, checksum;
	for( i = 1; i < 8; i++)
	{
		checksum += packet[i];
	}
	checksum = 0xff - checksum;
	checksum += 1;
	return checksum;
}

const uint8_t sendCommandPacket[9] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};


/**
 * CO2 concentration = (HIGH * 256) + LOW
 * HIGH = byte2, LOW is byte3
 */
void read_co2_sensor_MHZ19C()
{
	for (int i = 0; i <= 8; i++)
		sensorSerial.write(sendCommandPacket[i]);
	delay(10);
	if (sensorSerial.available() >= 0)
	{
		uint8_t dataPacket[DATA_PACKET_SIZE];
		sensorSerial.readBytes(dataPacket, DATA_PACKET_SIZE);
		Serial.print("Raw data: ");
		for (int i = 0; i < DATA_PACKET_SIZE; i++)
		{
			Serial.printf("%02X ", dataPacket[i]);
		}
		if (getCheckSum_MHZ19C(dataPacket) == dataPacket[8])
			Serial.printf("\nchecksum ok\n");
		else
		{
			Serial.printf("\nchecksum is not ok\n");
			return ;
		}
		// if ((dataPacket[0] == 0x42 && dataPacket[1] == 0x4D))
		// {
		Co2Value = ((int)dataPacket[2] * 256) + (int)dataPacket[3];
		Serial.printf("CO2 (ppm): %d\n", Co2Value);
			publish_int(CO2_topic, Co2Value);

		// }
		// else
		// 	Serial.printf("Invalid data packet header!\n");

	}
}
