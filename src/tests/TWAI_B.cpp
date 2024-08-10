

#include <Arduino.h>
#include "config.h"
#include <CAN.h>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "TWAI_custom.hpp"

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
        //Reconfigure alerts to detect Error Passive and Bus-Off error states
    uint32_t alerts_to_enable = TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_OFF;
    if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
        printf("Alerts reconfigured\n");
    } else {
        printf("Failed to reconfigure alerts");
    }


}

int ret;
twai_status_info_t info;

void loop()
{
    twai_clear_receive_queue();
    // delay(100);

    twai_get_status_info(&info);
    // Serial.printf("Status : %d\n", info.state);
    print_twai(info.state, TWAI_STATUS);

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
    //Process received message
    //Queue message for transmission
    // if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    //     Serial.printf("Message queued for transmission\n");
    // } else {
    //     Serial.printf("Failed to queue message for transmission: ERROR=%d\n", err);
    // }
    //Process received message
    Serial.printf("Receiving: ......\n");

    //Wait for message to be received
    twai_message_t receive_message;


    // Serial.println("receive ret: " + twai_receive(&receive_message, pdMS_TO_TICKS(10000)));
    

    ret = twai_receive(&receive_message, pdMS_TO_TICKS(1000));

    print_twai(ret, TRANSMIT_RECEIVE_START_STOP);
    // if (ret != ESP_OK)
    //     return;

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
