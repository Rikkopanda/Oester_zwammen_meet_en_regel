/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TWAI.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 17:02:28 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/08/10 17:40:22 by rikverhoeve      ###   ########.fr       */
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
