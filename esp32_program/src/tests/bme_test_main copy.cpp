/***
  Read Our Complete Guide: https://RandomNerdTutorials.com/esp32-bme680-sensor-arduino/
  Designed specifically to work with the Adafruit BME680 Breakout ----> http://www.adafruit.com/products/3660 These sensors use I2C or SPI to communicate, 2 or 4 pins are required to interface. Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit! Written by Limor Fried & Kevin Townsend for Adafruit Industries. BSD license, all text above must be included in any redistribution
***/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

/*#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme_0x77; // I2C
Adafruit_BME680 bme_0x76; // I2C

//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("BME680 async test"));

  bme_setup_and_init(&bme_0x76, "bme_0x76");
  bme_setup_and_init(&bme_0x77, "bme_0x77");
}

void read_bme(Adafruit_BME680 *bme_sensor, std::string name)
{
 // Tell BME680 to begin measurement.
  unsigned long endTime = bme_sensor->beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  Serial.print(name.c_str());
  Serial.print(F(" reading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.printf("%d\n", endTime);

  Serial.println(F("You can do other work during BME680 measurement."));
  delay(50); // This represents parallel work.
  // There's no need to delay() until millis() >= endTime: bme.endReading()
  // takes care of that. It's okay for parallel work to take longer than
  // BME680's measurement time.

  // Obtain measurement results from BME680. Note that this operation isn't
  // instantaneous even if milli() >= endTime due to I2C/SPI latency.
  if (!bme_sensor->endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(bme_sensor->temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(bme_sensor->pressure / 100.0);
  Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(bme_sensor->humidity);
  Serial.println(F(" %"));

  Serial.print(F("Gas = "));
  Serial.print(bme_sensor->gas_resistance / 1000.0);
  Serial.println(F(" KOhms"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(bme_sensor->readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(F(" m"));

  Serial.println("\n");
}

void loop() {

  read_bme(&bme_0x77, std::string("bme_0x77"));
  read_bme(&bme_0x76, std::string("bme_0x76"));
  delay(2000);
}
