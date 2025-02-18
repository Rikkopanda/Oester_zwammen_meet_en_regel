#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "Adafruit_BME280.h"
#include "driver/twai.h"
#include "driver/gpio.h"
#include "TWAI_custom.hpp"

uint32_t		time_interval;
int   			sensor_values[3];
char  			sensor_value_str[16];
int   			button_state;
int   			last_button_state;

//CAN communication
const uint8_t topic_can_ids[3] = {144, 145, 146};

//commands
const uint8_t topic_can_command_ids[3] = {147, 148, 149};

enum e_commands
{
  NEVELAAR,
  LUCHTAANVOER,
  AIRCO
};

Adafruit_BME680 bme_680; // I2C
// Adafruit_BME280 bme_280_1;
// Adafruit_BME280 bme_280_2; // I2C

// Define the CO2 sensor serial interface
HardwareSerial sensorSerial(2); // RX, TX



// Initialize an array to store CO2 values
// uint16_t co2Values[CO2_BUFFER_SIZE];
int Co2Value;

const t_callback_func_entry callback_table[3] = {
			{topic_can_command_ids[NEVELAAR], nevelaar_callback_action},
			{topic_can_command_ids[LUCHTAANVOER], lucht_aanvoer_callback_action},
			{topic_can_command_ids[AIRCO], airco_callback_action}
		};

void setup()
{
 	sensorSerial.begin(9600, SERIAL_8N1, 16, 17);
  
  Serial.begin(115200);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(NEVELAAR_PIN, OUTPUT);
  pinMode(LUCHT_AANVOER_PIN, OUTPUT);

  configure_twai_can();
  time_interval = 5000UL;
  bme680_setup_and_init(&bme_680, "bme_0x77");

}

void loop()
{
  // client.loop(); 
  if (Serial.available())
  {
      // Serial.read
      char buf[30];
      Serial.readBytes(buf, 30);
      // send_can_frame();
  }
  if (check_interval() == true)
  {
    Serial.println("hello check interval starting to print");
    activate_sensor();
    int Co2_value = read_co2_sensor_MHZ19C();
    int humidity = bme_680.readHumidity();
    int temp = bme_680.readTemperature();
    send_can_frame(topic_can_ids[CO2_SENSOR_I], &Co2_value);
    send_can_frame(topic_can_ids[MOISTURE_I], &humidity);
    send_can_frame(topic_can_ids[TEMP_SENSOR_I], &temp);
    time_interval = millis() + 200UL;
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
  callback(receive_message);
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
  // digitalWrite(TURN_ON_MOISTURE_SENSOR_PIN, HIGH);

  // Serial.printf("TURN_ON_MOISTURE_SENSOR_PIN\n");

  delay(300);
  sensor_values[MOISTURE_I] = analogRead(MOISTURE_PIN);
  sensor_values[CO2_SENSOR_I] = analogRead(CO2_SENSOR_PIN);
  sensor_values[TEMP_SENSOR_I] = analogRead(TEMP_SENSOR_PIN);

  // Serial.printf("value temp %d\n\n", sensor_values[TEMP_SENSOR_I]);
  // Serial.printf("value CO2 %d\n", sensor_values[CO2_SENSOR_I]);
  // Serial.printf("value moisture %d\n", sensor_values[MOISTURE_I]);

  // digitalWrite(TURN_ON_MOISTURE_SENSOR_PIN, LOW);
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
