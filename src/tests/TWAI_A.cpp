

#include <Arduino.h>
#include "config.h"
#include <CAN.h>
#include "driver/twai.h"
#include "driver/gpio.h"


const int MySerialRX = 16;
const int MySerialTX = 17;

// Configure message to transmit
twai_message_t message = {
    // Message ID and payload
    .identifier = 0x00AA,
    .data_length_code = 4,
    .data = {0,1,2,3}
};

void setup()
{
    Serial.begin(115200);  // Start the serial communication
    twai_general_config_t general_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_19, GPIO_NUM_18, TWAI_MODE_NORMAL);
    twai_timing_config_t timing_config = TWAI_TIMING_CONFIG_100KBITS();
    twai_filter_config_t filter_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    
    //Install TWAI driver
    if (twai_driver_install(&general_config, &timing_config, &filter_config) == ESP_OK) {
        Serial.printf("Driver installed\n");
    } else {
        Serial.printf("Failed to install driver\n");
        return;
    }

    //Start TWAI driver
    if (twai_start() == ESP_OK) {
        Serial.printf("Driver started\n");
    } else {
        Serial.printf("Failed to start driver\n");
        return;
    }
    message.extd = 1;              // Standard vs extended format
    message.rtr = 0;               // Data vs RTR frame
    message.ss = 0;                // Whether the message is single shot (i.e., does not repeat on error)
    message.self = 0;              // Whether the message is a self reception request (loopback)
    message.dlc_non_comp = 0;      // DLC is less than 8
}
int err;
uint8_t y = 0;
void loop()
{
    //Process received message
    Serial.printf("Transmit: ......\n");
    for (int x = 0; x < 4; x++)
    {
        message.data[x] = y;
        y++;
    }
    //Queue message for transmission
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        Serial.printf("Message queued for transmission\n");
    } else {
        Serial.printf("Failed to queue message for transmission: ERROR=%d\n", err);
    }
    //Process received message
    Serial.printf("Receiving: ......\n");

    //Wait for message to be received
    twai_message_t receive_message;
    Serial.println("receive ret: " + twai_receive(&receive_message, pdMS_TO_TICKS(10000)));

    // if (twai_receive(&receive_message, pdMS_TO_TICKS(10000)) == ESP_OK) {
    //     Serial.printf("Message received\n");
    // } else {
    //     Serial.printf("Failed to receive message: ERROR=%d\n", err);
    //     return;
    // }

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
