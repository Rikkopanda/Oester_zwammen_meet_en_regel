#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <CAN.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "Adafruit_BME280.h"
#include <CAN.h>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "TWAI_custom.hpp"


//wifi
WiFiClient    espClient;
const char*   ssid = SSID_MOBIEL;
const char*   password = PASSWORD_MOBIEL;

//mqtt
PubSubClient  client(espClient); // creates a pub sub client 
const char    *mqtt_broker = "test.mosquitto.org";

const char    moisture_topic[2][22] = {"moisture_meter_680_1", "moisture_meter_280_1"};

const char    *CO2_topic = "CO2_meter";
const char    temp_topic[2][22] = {"temp_meter_680_1", "temp_meter_280_1"};

//subscribe topics
const char    *pump_topic = "pump";
const char    *nevelaar_topic = "nevellaar";
const char    *lucht_aanvoer_topic = "lucht_aanvoer";

const char    *status_esp32_A = "status_esp32_A";
const char    *mqtt_username = NULL;
const char    *mqtt_password = NULL;
const int     mqtt_port = 1883;

uint32_t		time_interval;
int   			sensor_values[3];
char  			sensor_value_str[16];
int   			button_state;
int   			last_button_state;

Adafruit_BME680 bme_0x77; // I2C
Adafruit_BME280 bme_280_1;
// Adafruit_BME280 bme_280_2; // I2C

// Define the CO2 sensor serial interface
HardwareSerial sensorSerial(2); // RX, TX



// Initialize an array to store CO2 values
// uint16_t co2Values[CO2_BUFFER_SIZE];
int Co2Value;

const t_callback_func_entry callback_table[3] = {
			{pump_topic, pump_callback_action},
			{nevelaar_topic, nevelaar_callback_action},
			{lucht_aanvoer_topic, lucht_aanvoer_callback_action}
		};

void setup()
{
 	sensorSerial.begin(9600, SERIAL_8N1, 16, 17);
  
  Serial.begin(115200);

  // Set the pins
  // CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);

  // // start the CAN bus at 500 kbps
  // if (!CAN.begin(50E3)) {
  //   Serial.printf ("Starting CAN failed!");
  //   while (1);
  // }
  // else {
  //   Serial.printf ("CAN Initialized");
  // }
  // bme280_setup_and_init(&bme_280_2, "bme_280_2");
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(NEVELAAR_PIN, OUTPUT);
  pinMode(LUCHT_AANVOER_PIN, OUTPUT);

  configure_twai_can();
  // connect_wifi();
  // connect_broker();
  
  // client.publish(status_esp32_A, "Hi, I'm ESP32 A ^^");
  // client.subscribe(pump_topic);
  // client.subscribe(nevelaar_topic);
  // client.subscribe(lucht_aanvoer_topic);

  time_interval = 5000UL;
  // bme280_setup_and_init(&bme_280_1, "bme_280_1");
  // bme680_setup_and_init(&bme_0x77, "bme_0x77");
}

void loop()
{
  // canSender();
  // canReceiver();
  client.loop(); 
  // checks for new incoming messages from subscribed topics, excucutes callback funcion}
    // if (!Serial.available())
    //     return;
  if (Serial.available())
  {
      // Serial.read
      char buf[30];
      Serial.readBytes(buf, 30);
      send_can_frame();
  }
  if (check_interval() == true)
  {
    // activate_sensor();
    // read_co2_sensor_MHZ19C();
    // read_bme680_publish(&bme_0x77, std::string("bme_0x77"));
    // read_bme280_publish(&bme_280_1, std::string("bme_280_1"));

    // publish_int(moisture_topic, sensor_values[MOISTURE_I]);
    // publish_int(temp_topic, sensor_values[TEMP_SENSOR_I]);
    // publish_int(CO2_topic, sensor_values[CO2_SENSOR_I]);
    time_interval = millis() + 200UL;
    // Serial.printf("time: %ld\t time_interval %ld:\n", millis(), time_interval);
  }

  twai_message_t receive_message;
  int ret;

  ret = twai_receive(&receive_message, pdMS_TO_TICKS(1000));

  if (ret != ESP_OK)
      return;
  print_twai(ret, TRANSMIT_RECEIVE_START_STOP);

  if (receive_message.extd) {
      Serial.printf("Message is in Extended Format\n");
  } else {
      Serial.printf("Message is in Standard Format\n");
  }
  Serial.printf("ID is %d\n", receive_message.identifier);
  if (!(receive_message.rtr)) {
      for (int i = 0; i < receive_message.data_length_code; i++) {
          Serial.printf("Data byte %d = %d\n", i, receive_message.data[i]);
      }
  }
}

// void canSender() {
//   // send packet: id is 11 bits, packet can contain up to 8 bytes of data
//   Serial.print ("Sending packet ... ");

//   int ret = CAN.beginPacket (0x12);  //sets the ID and clears the transmit buffer

//   Serial.printf("return beginPacket: %d\n", ret);

//   if (ret == 1)
//     return;
//   // CAN.beginExtendedPacket(0xabcdef);
//   CAN.write ('1'); //write data to buffer. data is not sent until endPacket() is called.
//   CAN.write ('2');
//   CAN.write ('3');
//   CAN.write ('4');
//   CAN.write ('5');
//   CAN.write ('6');
//   CAN.write ('7');
//   CAN.write ('8');
//   CAN.endPacket();
//   Serial.printf("return endPacket: %d\n", ret);
//   //RTR packet with a requested data length
//   CAN.beginPacket (0x12, 3, true);
//   CAN.endPacket();
//   Serial.printf ("done");
//   delay (1000);
// }


/**
 * 
 * millis() starts at 0 upon startup, however the unsigned long where is stores
 * the time overflows at approx 50 days
 * 
 * time interval is set correctly again when this happens
*/
int check_overflow()
{
  if (time_interval > UINT64_MAX - (uint32_t)(10 * ONE_MINUTE) && millis() < UINT64_MAX / 2UL)
  {
    Serial.printf("overflow\n");
    return (true);
  }
  return (false);
}

/**
 * sensor only checks every minute
*/
int check_interval()
{
  if (check_overflow() == true)
    time_interval = millis() + 800UL;
  if (millis() > time_interval)
    return (true);
  else
    return (false);
}

/**
 * activates moisture meter circuit for 2 seconds
 * , last taken value will be used.
 * 
 * circuit gets deactivated so to not use unneccecary energy and wear out sensor
 * TURN_ON_MOISTURE_SENSOR_PIN is attached to a transistor switching the sensor
*/
int activate_sensor()
{
  digitalWrite(TURN_ON_MOISTURE_SENSOR_PIN, HIGH);

  // Serial.printf("TURN_ON_MOISTURE_SENSOR_PIN\n");

  // delay(300);
  sensor_values[MOISTURE_I] = analogRead(MOISTURE_PIN);
  sensor_values[CO2_SENSOR_I] = analogRead(CO2_SENSOR_PIN);
  sensor_values[TEMP_SENSOR_I] = analogRead(TEMP_SENSOR_PIN);

  // Serial.printf("value temp %d\n\n", sensor_values[TEMP_SENSOR_I]);
  // Serial.printf("value CO2 %d\n", sensor_values[CO2_SENSOR_I]);
  // Serial.printf("value moisture %d\n", sensor_values[MOISTURE_I]);

  digitalWrite(TURN_ON_MOISTURE_SENSOR_PIN, LOW);
  return (0);
}

// void ifbutton_pressed_send_serial_sig()
// {
//   if (button_state == LOW && last_button_state == LOW)
//   {
//     //do nothing
//   }
//   else if (button_state == HIGH && last_button_state == HIGH)
//   {
//     //do nothing, extra check so that digitalWrite() is not called unneccecarily
//   }
//   else if (button_state == LOW && last_button_state == HIGH)
//   {
//     client.publish(status_esp32_B, "field button off");
//     Serial.printf("turn on pump");
//   }
//   else if (button_state == HIGH && last_button_state == LOW)
//   {
//     client.publish(status_esp32_B, "field button on");
//     Serial.printf("turn off pump");
//   }
// }

/*
voor debuggen:

Serial.printf("moisture value: %d \n", potvalue);

idee:
// deep sleep arduino design

werkt niet in activate sensor...
  // while (millis() < measuring_time_slot && aaaa < measuring_time_slot)
  // {

*/
