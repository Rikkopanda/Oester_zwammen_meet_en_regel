

#include <Arduino.h>
#include "config.h"
#include <CAN.h>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "TWAI_custom.hpp"


const int MySerialRX = 18;
const int MySerialTX = 19;

#define CAN_RX_GPIO 18
#define CAN_TX_GPIO 19

void configure_twai_can()
{
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
    //Reconfigure alerts to detect Error Passive and Bus-Off error states
    uint32_t alerts_to_enable = TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_OFF;
    if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
        printf("Alerts reconfigured\n");
    } else {
        printf("Failed to reconfigure alerts");
    }
}

void setup()
{
    Serial.begin(115200);  // Start the serial communication
    configure_twai_can();
}
int err;
uint8_t y = 0;
twai_status_info_t info;
int ret;

void send_can_frame()
{
    twai_clear_transmit_queue();
    // Configure message to transmit
    twai_message_t message = {
        // Message ID and payload
        .identifier = 0x00AA,
        .data_length_code = 4,
        .data = {0,1,2,3}
    };
    message.extd = 1;              // Standard vs extended format
    message.rtr = 0;               // Data vs RTR frame
    message.ss = 0;                // Whether the message is single shot (i.e., does not repeat on error)
    message.self = 0;              // Whether the message is a self reception request (loopback)
    message.dlc_non_comp = 0;      // DLC is less than 8
    // delay(100);
    twai_get_status_info(&info);
    // Serial.printf("Status : %d\n", info.state);
    print_twai(info.state, TWAI_STATUS);

    //Block indefinitely until an alert occurs
    uint32_t alerts_triggered;
    twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(100));
    //Process received message
    if (info.state == TWAI_STATE_BUS_OFF)
        twai_initiate_recovery();
    else if (info.state == TWAI_STATE_STOPPED)
    {
        if (twai_start() == ESP_OK) {
            Serial.printf("Driver started\n");
        } else {
            Serial.printf("Failed to start driver\n");
            return;
        }
    }
    Serial.printf("Transmit: ......\n");
    for (int x = 0; x < 4; x++)
    {
        message.data[x] = y;
        y++;
    }
    //Queue message for transmission
    ret = twai_transmit(&message, pdMS_TO_TICKS(1000));
    print_twai(ret, TRANSMIT_RECEIVE_START_STOP);
}


void loop()
{
    y = 0;
    // if (!Serial.available())
    //     return;
    if (Serial.available())
    {
        // Serial.read
        char buf[30];
        Serial.readBytes(buf, 30);

        send_can_frame();
    }
    // if (ret != ESP_OK)
    //     return;

    //Process received message
    // Serial.printf("Receiving: ......\n");

    //Wait for message to be received
    twai_message_t receive_message;

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
