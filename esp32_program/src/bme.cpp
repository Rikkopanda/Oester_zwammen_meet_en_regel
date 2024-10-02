/***
  Read Our Complete Guide: https://RandomNerdTutorials.com/esp32-bme680-sensor-arduino/
  Designed specifically to work with the Adafruit BME680 Breakout ----> http://www.adafruit.com/products/3660 These sensors use I2C or SPI to communicate, 2 or 4 pins are required to interface. Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit! Written by Limor Fried & Kevin Townsend for Adafruit Industries. BSD license, all text above must be included in any redistribution
***/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "Adafruit_BME280.h"
#include "config.h"

extern Adafruit_BME680 bme_680; // I2C
extern Adafruit_BME280 bme_280_1; // I2C
// extern Adafruit_BME280 bme_280_2; // I2C

/**
 *  Tries to initialize sensor
 * 
 *  Sets up oversampling and filter initialization
 */
void bme680_setup_and_init(Adafruit_BME680 *bme_sensor, std::string name)
{
  if (!bme_sensor->begin(0x77)) {
    Serial.print("Could not find a valid BME680 ");
    Serial.print(name.c_str());
    Serial.println("try to check wiring");
    while (1);
  }

  bme_sensor->setTemperatureOversampling(BME680_OS_8X);
  bme_sensor->setHumidityOversampling(BME680_OS_2X);
  bme_sensor->setPressureOversampling(BME680_OS_4X);
  bme_sensor->setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme_sensor->setGasHeater(320, 150); // 320*C for 150 ms
}

void bme280_setup_and_init(Adafruit_BME280 *bme_sensor, std::string name)
{
  if (!bme_sensor->begin(0x76)) {
    Serial.print("Could not find a valid BME280 ");
    Serial.print(name.c_str());
    Serial.println("try to check wiring");
    while (1);
  }
  else
    Serial.printf("succeeded finding: %s\n", name.c_str());

  // bme_sensor->getHumiditySensor();
  // bme_sensor->set();
  // bme_sensor->readPressure();
  // bme_sensor->readAltitude(SEALEVELPRESSURE_HPA);
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
  Serial.println(endTime);

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

  Serial.println();
}


void read_bme280_publish(Adafruit_BME280 *bme_sensor, std::string name)
{
  Serial.print(F("280!! \nTemperature = "));
  Serial.print(bme_sensor->readTemperature());
  Serial.print(F(" *C\n"));

  // Serial.print(F("Pressure = "));
  // Serial.print(bme_sensor->pressure / 100.0);
  // Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(bme_sensor->readHumidity());
  Serial.print(F(" %\n"));
  // send_can_frame(topic_can_ids[], bme_sensor->readTemperature());
  // send_can_frame();

  // publish_int(temp_topic[BME_680], bme_sensor->readTemperature());
  // publish_int(moisture_topic[BME_680], bme_sensor->readHumidity());
}

void read_bme680_publish(Adafruit_BME680 *bme_sensor, std::string name)
{
 // Tell BME680 to begin measurement.
  unsigned long endTime = bme_sensor->beginReading();
  if (endTime == 0) {
    Serial.printf("%s Failed to begin reading :(\n", name.c_str());
    return;
  }
  Serial.print(name.c_str());
  Serial.print(F(" 680!!\nreading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.println(endTime);

  // Serial.println(F("You can do other work during BME680 measurement."));
  // delay(50); // This represents parallel work.
  // There's no need to delay() until millis() >= endTime: bme.endReading()
  // takes care of that. It's okay for parallel work to take longer than
  // BME680's measurement time.

  // Obtain measurement results from BME680. Note that this operation isn't
  // instantaneous even if milli() >= endTime due to I2C/SPI latency.
  if (!bme_sensor->endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  // Serial.print(F("Reading completed at "));
  // Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(bme_sensor->temperature);
  Serial.print(F(" *C\n"));
  // publish_int(temp_topic[BME_280], bme_sensor->temperature);

  // Serial.print(F("Pressure = "));
  // Serial.print(bme_sensor->pressure / 100.0);
  // Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(bme_sensor->humidity);
  Serial.print(F(" %\n"));
  // publish_int(moisture_topic[BME_280], bme_sensor->humidity);

  // Serial.print(F("Gas = "));
  // Serial.print(bme_sensor->gas_resistance / 1000.0);
  // Serial.println(F(" KOhms"));

  // Serial.print(F("Approx. Altitude = "));
  // Serial.print(bme_sensor->readAltitude(SEALEVELPRESSURE_HPA));
  // Serial.println(F(" m"));

}


