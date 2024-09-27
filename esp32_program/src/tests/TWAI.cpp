/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TWAI.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:02:28 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/09/27 18:55:20 by rikverhoeve      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Arduino.h>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "TWAI_custom.hpp"

void print_status(int code)
{
    Serial.print("twai status func:");
    switch (code)
    {
        case TWAI_STATE_STOPPED:             /**< Stopped state. The TWAI controller will not participate in any TWAI bus activities */
            Serial.println("TWAI_STATE_STOPPED");
            break;
        case TWAI_STATE_RUNNING:             /**< Running state. The TWAI controller can transmit and receive messages */
            Serial.println("TWAI_STATE_RUNNING");
            break;
        case TWAI_STATE_BUS_OFF:             /**< Bus-off state. The TWAI controller cannot participate in bus activities until it has recovered */
            Serial.println("TWAI_STATE_BUS_OFF");
            break;
        case TWAI_STATE_RECOVERING:          /**< Recovering state. The TWAI controller is undergoing bus recovery */
            Serial.println("TWAI_STATE_RECOVERING");
            break;
        default:
            Serial.println("unrecognized");
            break;
    }
}

void print_other(int code)
{
    Serial.print("twai other func:");
    switch (code)
    {
        case ESP_OK:          
            Serial.println("ESP_OK");
            break;
        case ESP_ERR_TIMEOUT:          /**< Recovering state. The TWAI controller is undergoing bus recovery */
            Serial.println("ESP_ERR_TIMEOUT");
            break;
        case ESP_ERR_INVALID_ARG:          /**< Recovering state. The TWAI controller is undergoing bus recovery */
            Serial.println("TWAI_STATE_RECOVERING");
            break;
        case ESP_ERR_INVALID_STATE:          /**< Recovering state. The TWAI controller is undergoing bus recovery */
            Serial.println("TWAI_STATE_RECOVERING");
            break;
        case ESP_ERR_NOT_SUPPORTED:          /**< Recovering state. The TWAI controller is undergoing bus recovery */
            Serial.println("TWAI_STATE_RECOVERING");
            break; 
        case ESP_FAIL:          /**< Recovering state. The TWAI controller is undergoing bus recovery */
            Serial.println("TWAI_STATE_RECOVERING");
            break; 
        default:
            Serial.println("unrecognized");
            break;
    }
}


void print_twai(int code, t_code_type type)
{
    switch (type)
    {
        case TWAI_STATUS:
            print_status(code);
            break;
        case TRANSMIT_RECEIVE_START_STOP:
            print_other(code);
            break;
        default:
            break;
    }
}

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

int err;
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
    //Queue message for transmission
    ret = twai_transmit(&message, pdMS_TO_TICKS(1000));
    print_twai(ret, TRANSMIT_RECEIVE_START_STOP);
}