
#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <string>
#include <cstddef>

//subscribe topics
extern const char    *pump_topic;
extern const char    *nevelaar_topic;
extern const char    *lucht_aanvoer_topic;
//to publish topics
extern const char    *CO2_topic;
extern const char    *temp_topic;
extern const char    *moisture_topic;

extern const char    *status_esp32_A;
extern const char    *mqtt_username;
extern const char    *mqtt_password;
extern const int     mqtt_port;

extern uint32_t		 time_interval;
extern int   		 potvalue;
extern char  		 potvalue_str[16];
extern int   		 button_state;
extern int   		 last_button_state;


typedef struct s_callback_func_entry
{
	const char *topic;
	void (*callback_func_ptr)(std::string &data);
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

enum e_sensor_array_indexes
{
	CO2_SENSOR_I,
	MOISTURE_I,
	TEMP_SENSOR_I
};
//------wifi-------
#define       SSID_MOBIEL "V30_9620"
#define       PASSWORD_MOBIEL "hoi1hoi1"
void 	  connect_broker();
void 	  connect_wifi();
std::string	get_wifi_status(int status);


void callback(char *topic, uint8_t *payload, unsigned int length);
void pump_callback_action(std::string &data);
void nevelaar_callback_action(std::string &data);
void lucht_aanvoer_callback_action(std::string &data);

int check_overflow();
int check_interval();
int activate_sensor();
void publish_int(const char *topic, int val);

//------CH8_sensor_co2-------
// Define the number of bytes in a data packet
#define DATA_PACKET_SIZE 16
#define CO2_BUFFER_SIZE 60
class HardwareSerial;
void read_co2_sensor();
extern HardwareSerial sensorSerial; // RX, TX


//------CAN-------
void canSender();
void canReceiver();

//------bme_sensor-------
#define SEALEVELPRESSURE_HPA (1013.25)
class Adafruit_BME680;

void bme_setup_and_init(Adafruit_BME680 *bme_sensor);
void read_bme_publish(Adafruit_BME680 *bme_sensor, std::string name);




#endif