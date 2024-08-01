#include <WiFi.h>
#include <PubSubClient.h>
#include <config.h>


void pump_callback_action(String &data)
{
	if (data == "true")
		digitalWrite(PUMP_PIN, HIGH);
	else if(data == "false")
		digitalWrite(PUMP_PIN, LOW);
  Serial.printf("setting pin %d to %s\n", PUMP_PIN, data);
}

void nevelaar_callback_action(String &data)
{
  if (data == "true")
    digitalWrite(NEVELAAR_PIN, HIGH);
  else if(data == "false")
    digitalWrite(NEVELAAR_PIN, LOW);
  Serial.printf("setting pin %d to %s\n", NEVELAAR_PIN, data);
}

void lucht_aanvoer_callback_action(String &data)
{
	if (data == "true")
		digitalWrite(LUCHT_AANVOER_PIN, HIGH);
	else if(data == "false")
		digitalWrite(LUCHT_AANVOER_PIN, LOW);
  Serial.printf("setting pin %d to %s\n", LUCHT_AANVOER_PIN, data);
}
/**
 * 
 * gets called whenever message is received in a subscribed topic
 * 
 * Sets the pump on or off based on message
*/
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  String data = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println();
  Serial.println("-----------------------");

  for (int i = 0; i < 3; i++)
  {
    if (String(callback_table[i].topic) == String(topic))
      callback_table[i].callback_func_ptr(data);
  }
}
