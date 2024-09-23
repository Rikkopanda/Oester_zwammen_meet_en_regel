#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <CAN.h>
#include "MHZ19_uart.h"
#include <SoftwareSerial.h>

#define TXD0 1
#define RXD0 3

// SoftwareSerial ss(RXD0, TXD0);
// HardwareSerial uart2(2);
// MHZ19_uart mhz;

// void setup()
// {
//   Serial.begin(115200);

//   // Set the pins
//   // CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);

//   // start the CAN bus at 500 kbps
//   // if (!CAN.begin (50E3)) {
//   //   Serial.println ("Starting CAN failed!");
//   //   while (1);
//   // }
//   // else {
//   //   Serial.println ("CAN Initialized");
//   // }
//   uart2.begin(9600, SERIAL_8N1, 16, 17);
//   mhz.begin(uart2);                                // *Serial(Stream) reference must be passed to library begin().
//   mhz.setAutoCalibration(true);  
// }

// void loop()
// {
//   // if (uar2.ab)
//   // MHZ19_RESULT response = mhz.retrieveData();
//   // if (response == MHZ19_RESULT_OK)
//   // {
//   //   Serial.print(F("CO2: "));
//   //   Serial.println(mhz.getCO2());
//   //   Serial.print(F("Min CO2: "));
//   //   Serial.println(mhz.getMinCO2());
//   //   Serial.print(F("Temperature: "));
//   //   Serial.println(mhz.getTemperature());
//   //   Serial.print(F("Accuracy: "));
//   //   Serial.println(mhz.getAccuracy());
//   // }
//   // else
//   // {
//   //   Serial.print(F("Error, code: "));
//   //   Serial.println(response);
//   // }
//   Serial.println("helllo");
//   delay(500);

//   // canSender();
//   // canReceiver();
// }

// const int rx_pin = 16; //Serial rx pin no
// const int tx_pin = 17; //Serial tx pin no

// MHZ19_uart mhz19;

// /*----------------------------------------------------------
//     MH-Z19 CO2 sensor  setup
//   ----------------------------------------------------------*/
// void setup()
// {
//   Serial.begin(9600);
//   mhz19.begin(rx_pin, tx_pin);
//   mhz19.setAutoCalibration(false);

//   Serial.println("MH-Z19 is warming up now.");
//   delay(10 * 1000); //
// }

// /*----------------------------------------------------------
//     MH-Z19 CO2 sensor  loop
//   ----------------------------------------------------------*/
// void loop()
// {
//   int co2ppm = mhz19.getCO2PPM();
//   int temp = mhz19.getTemperature();

//   Serial.print("co2: ");
//   Serial.println(co2ppm);
//   Serial.print("temp: ");
//   Serial.println(temp);

//   delay(5000);
// }

/*----------------------------------------------------------
    MH-Z19 CO2 sensor  SAMPLE
  ----------------------------------------------------------*/

// #include "MHZ19.h"

// const int rx_pin = 16; //Serial rx pin no
// const int tx_pin = 17; //Serial tx pin no

// const int pwmpin = 14;

// MHZ19 *mhz19_uart = new MHZ19(rx_pin,tx_pin);
// MHZ19 *mhz19_pwm = new MHZ19(pwmpin);

// /*----------------------------------------------------------
//     MH-Z19 CO2 sensor  setup
//   ----------------------------------------------------------*/
// void setup()
// {
//     Serial.begin(115200);
//     mhz19_uart->begin(rx_pin, tx_pin);
//     mhz19_uart->setAutoCalibration(false);
//     delay(4000); // Issue #14
//     Serial.print("MH-Z19 now warming up...  status:");
//     Serial.println(mhz19_uart->getStatus());
//     delay(1000);
// }

// /*----------------------------------------------------------
//     MH-Z19 CO2 sensor  loop
//   ----------------------------------------------------------*/
// void loop()
// {
//     measurement_t m = mhz19_uart->getMeasurement();

//     Serial.print("co2: ");
//     Serial.println(m.co2_ppm);
//     Serial.print("temp: ");
//     Serial.println(m.temperature);

//     // int co2ppm = mhz19_pwm->getPpmPwm();
//     // Serial.print("co2: ");
//     // Serial.println(co2ppm);
    
//     delay(5000);
// }

// void canReceiver() {
//   // try to parse packet
//   int packetSize = CAN.parsePacket();
//   Serial.printf("return packetSize: %d\n", packetSize);

//   if (packetSize) {
//     // received a packet
//     Serial.print ("Received ");

//     if (CAN.packetExtended()) {
//       Serial.print ("extended ");
//     }

//     if (CAN.packetRtr()) {
//       // Remote transmission request, packet contains no data
//       Serial.print ("RTR ");
//     }

//     Serial.print ("packet with id 0x");
//     Serial.print (CAN.packetId(), HEX);

//     if (CAN.packetRtr()) {
//       Serial.print (" and requested length ");
//       Serial.println (CAN.packetDlc());
//     } else {
//       Serial.print (" and length ");
//       Serial.println (packetSize);

//       // only print packet data for non-RTR packets
//       while (CAN.available()) {
//         Serial.print ((char) CAN.read());
//       }
//       Serial.println();
//     }

//     Serial.println();
//   }
// }