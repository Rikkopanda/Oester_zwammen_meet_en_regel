
#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <string>
#include <cstddef>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "TWAI_custom.hpp"

//subscribe topics
// extern const char    *temp_topic;
// extern const char    *moisture_topic;

extern uint32_t		 time_interval;
extern int   		 potvalue;
extern char  		 potvalue_str[16];
extern int   		 button_state;
extern int   		 last_button_state;

typedef struct s_callback_func_entry
{
	uint8_t can_frame_id;
	void (*callback_func_ptr)(twai_message_t &message);
} t_callback_func_entry;

extern const t_callback_func_entry callback_table[3];

#define ONE_MINUTE 60 * 1000
#define TWO_SECONDS 2000

#define TX_GPIO_NUM   4  // Connects to CTX
#define RX_GPIO_NUM   5  // Connects to CRX

enum e_outputs
{
  PUMP_PIN = 25,
  NEVELAAR_PIN = 26,
  LUCHT_AANVOER_PIN = 27,
  LED_PIN_17 = 17,
  LED_PIN_18 = 18,
  LED_PIN_21 = 21,
  LED_PIN_22 = 22,
  TURN_ON_MOISTURE_SENSOR_PIN = 19,
  TEST_LED_PIN = 24
};

enum e_inputs
{
  CO2_SENSOR_PIN = 32,
  MOISTURE_PIN = 34,
  TEMP_SENSOR_PIN = 35
};

enum e_bme_sensor_indexes
{
  BME_680,
  BME_280
};

enum e_sensor_array_indexes
{
	CO2_SENSOR_I,
	MOISTURE_I,
	TEMP_SENSOR_I
};
//------wifi-------
// #define       SSID_MOBIEL "V30_9620"
// #define       PASSWORD_MOBIEL "schildpad"

void callback(twai_message_t &message);
void airco_callback_action(twai_message_t &message);
void nevelaar_callback_action(twai_message_t &message);
void lucht_aanvoer_callback_action(twai_message_t &message);

int check_overflow();
int check_interval();
int activate_sensor();
void publish_int(const char *topic, int val);

//------CH8_sensor_co2-------
// Define the number of bytes in a data packet
#define DATA_PACKET_SIZE 16
#define CO2_BUFFER_SIZE 60

class HardwareSerial;
void  read_co2_sensor_CH8();
int   read_co2_sensor_MHZ19C();
extern HardwareSerial sensorSerial; // RX, TX


//------CAN-------
void canSender();
void canReceiver();

//------bme_sensor-------
#define SEALEVELPRESSURE_HPA (1013.25)
class Adafruit_BME680;
class Adafruit_BME280;

void bme680_setup_and_init(Adafruit_BME680 *bme_sensor, std::string name);
void bme280_setup_and_init(Adafruit_BME280 *bme_sensor, std::string name);
void read_bme680_publish(Adafruit_BME680 *bme_sensor, std::string name);
void read_bme280_publish(Adafruit_BME280 *bme_sensor, std::string name);

#endif
