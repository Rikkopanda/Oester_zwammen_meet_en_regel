

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
SoftwareSerial sensorSerial(2, 3); // RX, TX

// Define the number of bytes in a data packet
#define DATA_PACKET_SIZE 16

// Define the size of the CO2 buffer in “element-seconds”
// The CO2 sensor sends data every second,
// and the CO2_BUFFER_SIZE determines the number of measurements stored.
// It is also used as the interval (in seconds) for printing the average value.
#define CO2_BUFFER_SIZE 60
unsigned long intervalPrintTime = CO2_BUFFER_SIZE * 1000UL;

// Initialize an array to store CO2 values
uint16_t co2Values[CO2_BUFFER_SIZE];
// Variable to keep track of the current index in the buffer
int co2Index = 0;
// Variable to keep track of the number of valid measurements in the buffer
int validMeasurements = 0;
// Variable to keep track of the last print time
unsigned long lastPrintTime = 0;

void setup() {
// Start the serial communication with the CO2 sensor
sensorSerial.begin(9600);
Serial.begin(9600); // You can change this baud rate based on your needs
}

void printAverage() {
// Calculate the average of the valid CO2 values in the buffer
uint32_t sum = 0;
for (int i = 0; i < validMeasurements; i++) {
sum += co2Values[i];
}
uint16_t average = static_cast(sum / validMeasurements);

// Print the average CO2 concentration
Serial.print(“Average CO2 Concentration: “);
Serial.println(average);
}

void loop() {
if (sensorSerial.available() >= DATA_PACKET_SIZE) {
// Read the data packet from the CO2 sensor
uint8_t dataPacket[DATA_PACKET_SIZE];
sensorSerial.readBytes(dataPacket, DATA_PACKET_SIZE);

// Check if the data packet has a valid header
if (dataPacket[0] == 0x42 && dataPacket[1] == 0x4D) {
// Extract CO2 concentration from the data packet
uint16_t co2Concentration = (dataPacket[6] << 8) | dataPacket[7];

// Verify the checksum
uint8_t checksum = 0;
for (int i = 0; i = intervalPrintTime) || (millis() < lastPrintTime)) {
printAverage();
lastPrintTime = millis();
}
} else {
// Checksum error
Serial.printf("Checksum error!");
}
} else {
// Invalid header
Serial.printf("Invalid data packet header!");
}
}