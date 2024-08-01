#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <CAN.h>
#include "MHZ19_uart.h"
#include <SoftwareSerial.h>
#include <cstdint>

/**
HC8 sensor code for arduino Uno. I wrote this with help from OG python script,
from translated chinese docs, with some help from GPT.
By default in active output mode, sensor will produce CO2 value every second.
This script will store last 60 values and each minute will calculate the average.

Data format in “active output” mode:

The output format is 16BYTE.
Data header: BYTE0 = 0X42; BYTE1=4D
BYTE6 data is high, BYTE7 data is low, indicating CO2 concentration.
BYTE15, data checksum. BYTE15= BYTE0+BYTE1+…….+BYTE13;

Example: 42 4D 0C 51 09 A2 07 2B 01 35 05 81 20 08 20 AD;
CO2 concentration = BYTE6 X 256 + BYTE7 = 07X256 + 2B = 1853;
**/

// Define the CO2 sensor serial interface
HardwareSerial sensorSerial(2); // RX, TX

// Define the number of bytes in a data packet
#define DATA_PACKET_SIZE 16

// Define the size of the CO2 buffer in “element-seconds”
// The CO2 sensor sends data every second,
// and the CO2_BUFFER_SIZE determines the number of measurements stored.
// It is also used as the interval (in seconds) for printing the average value.
#define CO2_BUFFER_SIZE 60

// Initialize an array to store CO2 values
uint16_t co2Values[CO2_BUFFER_SIZE];


void setup()
{
	// Start the serial communication with the CO2 sensor
	sensorSerial.begin(9600, SERIAL_8N1, 16, 17);
//   uart2.begin(9600, SERIAL_8N1, 16, 17);

	Serial.begin(115200); // You can change this baud rate based on your needs
	Serial.println("start");
}

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
			Serial.println();
			int Co2Value = ((int)dataPacket[6] << 8) | (int)dataPacket[7];
			Serial.printf("CO2 (ppm): %d\n", Co2Value);
		}
		else
			Serial.println("Invalid data packet header!");
	}
}

void loop()
{
	read_co2_sensor();
}