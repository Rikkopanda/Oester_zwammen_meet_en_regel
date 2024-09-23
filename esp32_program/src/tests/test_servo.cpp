
#include <Arduino.h>
#include <esp32-hal.h>
#include <esp32-hal-ledc.h>
#include "driver/ledc.h"
#include "hal/ledc_types.h"

#define LEDC_CHANNEL_0 0
#define PP_PLUS 14
#define PD_PLUS 12
#define ENABLE 13
#define LIMIT_01_REAR 26
#define LIMIT_02_FORWARD 25
#define LIN_DIST_PER_2PI_RAD 75
#define LEDC_TEST_DUTY 127
volatile bool forwardLimitTriggered = false;
volatile bool backwardLimitTriggered = false;
volatile bool emergency_stop_hit = false;

//Interrupt function for the forward limit switch
void rearLimitHit ()
{
	// digitalWrite(ENABLE, LOW);
	// tone(PP_PLUS, 0);
	// Serial.printf("rearLimitHit limit switch interrupt!!\n");
	backwardLimitTriggered = true;
	
}

//Interrupt function for the forward limit switch
void forwardLimitHit ()
{
	// digitalWrite(ENABLE, LOW);
	// tone(PP_PLUS, 0);

	// Serial.printf("forwardLimitHit limit switch interrupt!!\n");
	// if (backwardLimitTriggered != true)
	forwardLimitTriggered = true;
}

// ledc_timer_config_t ledc_timer;
// ledc_channel_config_t ledc_channel;
//Interrupt function for the forward limit switch
void emergency_stop ()
{
	// digitalWrite(ENABLE, LOW);
	// tone(PP_PLUS, 0);
	// tone(PP_PLUS, 0);
	// ledcWriteTone(LEDC_CHANNEL_0, 0);
	// Serial.printf("ledc_set_freq return value: %d \n", (int)ret);
	// ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, 0);
	// ledc_timer.freq_hz = 0; 
	// ledc_timer_config(&ledc_timer);
	// Serial.printf("forwardLimitHit limit switch interrupt!!\n");
	// if (backwardLimitTriggered != true)
	emergency_stop_hit = true;
}
const byte ledPin = 12;
const uint32_t freq = 170000; // 1.7Mhz
const uint8_t ledChannel = 0;
const uint8_t resolution = 3;
void setup()
{
	pinMode(PD_PLUS, OUTPUT);
	pinMode(PP_PLUS, OUTPUT);
	pinMode(ENABLE, OUTPUT);
	pinMode(LIMIT_01_REAR, OUTPUT);
	pinMode(LIMIT_02_FORWARD, INPUT_PULLUP);

	// pinMode(LIMIT_SENSOR_L, OUTPUT);
	// pinMode(LIMIT_SENSOR_L, INPUT);
	digitalWrite(ENABLE, LOW);

	digitalWrite(LIMIT_01_REAR, HIGH);
	attachInterrupt(digitalPinToInterrupt(LIMIT_01_REAR), rearLimitHit, FALLING);
	attachInterrupt(digitalPinToInterrupt(LIMIT_02_FORWARD), forwardLimitHit, RISING);
	attachInterrupt(digitalPinToInterrupt(ENABLE), emergency_stop, FALLING);
	
	// ledcSetup(LEDC_CHANNEL_0, 0, 1);
	// ledcAttachPin(PP_PLUS, LEDC_CHANNEL_0);
	
	// ledc_timer_config();

	// Define LED PWM timer configuration
	// ledc_timer.duty_resolution = LEDC_TIMER_3_BIT; // PWM duty resolution is 13 bits
	// ledc_timer.freq_hz = 1000;                      // PWM frequency is 5000Hz
	// ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;   // Use high-speed mode
	// ledc_timer.timer_num = LEDC_TIMER_0;            // Use timer 0
	// ledc_timer.clk_cfg = LEDC_AUTO_CLK;

	// Serial.printf("ledc_timer_config() return: %d\n", ledc_timer_config(&ledc_timer)); // Configure PWM timer
	// Define LED PWM channel configuration
	// ledc_channel.channel    = (ledc_channel_t)LEDC_CHANNEL_0; // PWM channel 0
	// ledc_channel.duty       = 4;            // Initial duty cycle is 127, 50% of 255(full)
	// ledc_channel.gpio_num   = PP_PLUS;             // GPIO pin is 18
	// ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE; // Use high-speed mode
	// ledc_channel.timer_sel  = LEDC_TIMER_0;    // Use timer 0
	// Serial.printf("ledc_channel_config() return: %d\n", ledc_channel_config(&ledc_channel)); // Configure PWM channel
	// ledc_fade_func_install(ESP_INTR_FLAG_LEVEL4);

	Serial.println("Configuring PWM");
	ledcAttachPin(PP_PLUS, ledChannel);
	ledcSetup(ledChannel, freq, resolution);
	ledcWrite(ledChannel, 4);
	Serial.begin(9600);
	Serial.printf("Starting up ......\n");
	Serial.printf("Goodmorning\n");
	// attachInterrupt(digitalPinToInterrupt(forwardLimitSwitchPin), forwardLimitHit, FALLING);
}

int sign = 1;
uint previous_l_hit = -1;

#define DATA_PACKET_SIZE 15

void loop()
{
	// if (sign == 1)
	// {
	// 	Serial.println("Counter-clockwise rotation");
	// 	sign = 0;
	// }
	// else
	// {
	// 	Serial.println("Clockwise rotation");
	// 	sign = 1;
	// }
	// digitalWrite(ENABLE, HIGH);
	// digitalWrite(PD_PLUS, sign);
	// Serial.println("1Khz tone");
	// tone(PP_PLUS, 1*1000);
	// delay(2000);
	// Serial.println("10Khz tone");
	// tone(PP_PLUS, 10*1000);
	// delay(2000);
	// Serial.println("30Khz tone");
	// tone(PP_PLUS, 30*1000);
	// delay(2000);
	// Serial.println("80Khz tone");
	// tone(PP_PLUS, 60*1000);
	// delay(2000);
	// Serial.println("0Khz tone");
	// tone(PP_PLUS, 0);
	// delay(2000);
	// uint l_pin =digitalRead(LIMIT_01);
	
	// if (l_pin == HIGH && previous_l_hit != l_pin)
	// {
	// 	previous_l_hit = l_pin;
	// 	Serial.printf("object gone...\n");
	// }
	// else if (l_pin == LOW && previous_l_hit != l_pin)
	// {
	// 	previous_l_hit = l_pin;
	// 	// tone(PP_PLUS, 0);
	// 	Serial.printf("limit switch detect object!!\n");
	// }
	if (forwardLimitTriggered == true)
	{
		delay(30);
		if (digitalRead(LIMIT_02_FORWARD) == HIGH)
		{
			digitalWrite(ENABLE, LOW);
			Serial.printf("forwardLimitTriggered\n");
		}
		forwardLimitTriggered = false;
	}
	if (backwardLimitTriggered == true)
	{
		// delay(20);
		if (digitalRead(LIMIT_01_REAR) == LOW)
		{
			digitalWrite(ENABLE, LOW);
			Serial.printf("backwardLimitTriggered\n");
			backwardLimitTriggered = false;
		}
	}
	// if (emergency_stop_hit == true)
	// {
	// 	delay(20);
	// 	Serial.printf("emergency_stop hit1 \n");

	// 	// if (digitalRead(ENABLE) == HIGH)
	// 	// {
	// 		digitalWrite(ENABLE, LOW);
	// 		Serial.printf("emergency_stop hit\n");
	// 		tone(PP_PLUS, 0);
	// 		emergency_stop_hit = false;
	// 	// }
	// 	// else
	// 	// 	emergency_stop_hit = false;
	// }
	if (Serial.available() > 0)
	{
		char dataPacket[DATA_PACKET_SIZE + 1];
		int size_read = Serial.readBytes(dataPacket, DATA_PACKET_SIZE);
		dataPacket[size_read] = '\0';
		Serial.printf("Raw data: %s\n", dataPacket);
		// if (strncmp(dataPacket, "o_high", 2) == 0)
		// {
		// 	digitalWrite(LIMIT_SENSOR_OUT, HIGH);
		// }
		// else if (strncmp(dataPacket, "o_low", 2) == 0)
		// {
		// 	digitalWrite(LIMIT_SENSOR_OUT, LOW);
		// }
		if (strncmp(dataPacket, "cw", 2) == 0)
		{
			digitalWrite(PD_PLUS, LOW);
			digitalWrite(ENABLE, HIGH);
			char *speed = strdup(&dataPacket[2]);
			int speed_int = atoi(speed);
			// tone(PP_PLUS, 0);
			Serial.printf("YES CW SPEED: %s kHz\n", speed);
			Serial.printf("int: %d kHz\n", speed_int);
			// tone(PP_PLUS, speed_int * 1000);
			// ledcWriteTone(LEDC_CHANNEL_0, speed_int * 1000); 
			// analogWriteFrequency(speed_int * 1000);
			// esp_err_t ret = ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, speed_int * 1000);
			// Serial.printf("ledc_set_duty %d\n", ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)LEDC_CHANNEL_0, 4, ledc_get_hpoint(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)LEDC_CHANNEL_0)));
			// Serial.printf("ledc_set_duty ", ledc_set_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)LEDC_CHANNEL_0, 127));
			// ledc_update_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)LEDC_CHANNEL_0);
			// ledc_channel_config(&ledc_channel);
			ledcSetup(ledChannel, speed_int * 1000, resolution);
			ledcWrite(ledChannel, 4);
			// ledcWrite(PP_PLUS, 127);
			// ESP_FAIL
			// ESP_ERR_INVALID_ARG

			Serial.printf("ledc_set_freq return value: %d, get_duty: %d, get_freq: %d high point %d\n",
				(int)7777, ledc_get_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)LEDC_CHANNEL_0),
				ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0),
				ledc_get_hpoint(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)LEDC_CHANNEL_0));
			// ledc_timer.freq_hz = speed_int * 1000; 
			free(speed);
		}
		else if (strncmp(dataPacket, "ccw", 3) == 0)
		{
			digitalWrite(PD_PLUS, HIGH);
			digitalWrite(ENABLE, HIGH);
			char *speed = strdup(&dataPacket[3]);
			Serial.printf("YES CCW SPEED: %s kHz\n", speed);
			int speed_int = atoi(speed);
			Serial.printf("int: %d kHz\n", speed_int);
			// esp_err_t ret = ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, speed_int * 1000);
			// ledc_channel_config(&ledc_channel);
			ledcSetup(ledChannel, speed_int * 1000, resolution);
			ledcWrite(ledChannel, 4);
			Serial.printf("ledc_set_freq return value: %d, get_duty: %d, get_freq: %d\n",
				(int)777, ledc_get_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)LEDC_CHANNEL_0),
				ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));			// ledc_timer.freq_hz = speed_int * 1000; 
			// ledc_timer_config(&ledc_timer);
			// tone(PP_PLUS, speed_int * 1000);
			// ledcWriteTone(LEDC_CHANNEL_0, speed_int * 1000); 
			free(speed);
		}
		else
		{
			Serial.printf("STOPPING\n");
			digitalWrite(ENABLE, LOW);
			// ledc_timer.freq_hz = 0; 
			// esp_err_t ret = ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, 0);
			// ledc_timer_config(&ledc_timer);
			// ledc_channel_config(&ledc_channel);
			// tone(PP_PLUS, 0);
			// ledc_timer_config(&new_ledc_timer);
			// ledcWriteTone(LEDC_CHANNEL_0, 0); 
		}
	}

	// digitalWrite(ENABLE, LOW);
	// delay(5000);
}
