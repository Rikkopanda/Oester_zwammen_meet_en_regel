
#include "config.h"
#include <PubSubClient.h>

extern char sensor_value_str[16];
extern PubSubClient  client; // creates a pub sub client 

void publish_int(const char *topic, int val)
{
    itoa(val, sensor_value_str, 10);
    client.publish(topic, sensor_value_str);
}
