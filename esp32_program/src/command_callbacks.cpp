#include <config.h>
#include <Arduino.h>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "TWAI_custom.hpp"

void airco_callback_action(twai_message_t &message)
{
	// if (message.data[0] == true)
	// 	digitalWrite(PUMP_PIN, HIGH);
	// else if(message.data[0] == false)
	// 	digitalWrite(PUMP_PIN, LOW);
  Serial.printf("setting pin %d to %d\n", PUMP_PIN, message.data[0]);
}

void nevelaar_callback_action(twai_message_t &message)
{
  if (message.data[0] == true)
    digitalWrite(NEVELAAR_PIN, HIGH);
  else if(message.data[0] == false)
    digitalWrite(NEVELAAR_PIN, LOW);
  Serial.printf("setting pin %d to %d\n", NEVELAAR_PIN, message.data[0]);
}

void lucht_aanvoer_callback_action(twai_message_t &message)
{
	if (message.data[0] == true)
		digitalWrite(LUCHT_AANVOER_PIN, HIGH);
	else if(message.data[0] == false)
		digitalWrite(LUCHT_AANVOER_PIN, LOW);
  Serial.printf("setting pin %d to %d\n", LUCHT_AANVOER_PIN, message.data[0]);
}

/**
 * 
 * gets called whenever message is received in a subscribed topic
 * 
 * Sets the pump on or off based on message
*/
void callback(twai_message_t &message)
{
  Serial.print("Message arrived in can id: ");
  Serial.println(message.identifier);

  for (int i = 0; i < message.data_length_code; i++) {
    Serial.print((char)message.data[i]);
  }
  Serial.println();
  Serial.println("-----------------------");

  for (int i = 0; i < 3; i++)
  {
    if (callback_table[i].can_frame_id == message.identifier)
      callback_table[i].callback_func_ptr(message);
  }
}
