#include "wifi-switch.h"
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "strip.h"
#include "../../wlan.conf"

// Update these with values suitable for your network.

int value = 0;

#define MOTOR D1
#define LED1 D2
#define LED2 D3
#define LED3 TX


void update_leds();
void blink();

void setup() {
								pinMode(MOTOR, OUTPUT);
								pinMode(LED1, OUTPUT);
								pinMode(LED2, OUTPUT);
								pinMode(LED3, OUTPUT);
								pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
								Serial.begin(115200);

								// LED Strip acitvate
								Serial.println("Strip");
								CLedStrip* strip = CLedStrip::getStrip_ptr();
								strip->init();
								strip->getConf().ctr = 0;
								strip->getConf().max = 100;
								strip->getConf().min = 0;
								strip->getConf().period = 10;
								strip->switch_program(1);
								Serial.println("Strip end");
}

void loop()
{
								update_leds();
								delay(10);
								blink();
}

void update_leds()
{
								// update led pattern / program
								CLedStrip::getStrip_ptr()->update();
}

static bool onOff = false;
static int blinkCtr = 0;

void blink()
{
								if(blinkCtr == 100)
								{

																blinkCtr = 0;
																if(onOff == true)
																{
																								//digitalWrite(MOTOR, HIGH);
																								digitalWrite(LED1, HIGH);
																								digitalWrite(LED2, HIGH);
																								digitalWrite(LED3, HIGH);
																								onOff = false;
																}
																else
																{
																								digitalWrite(MOTOR, LOW);
																								digitalWrite(LED1, LOW);
																								digitalWrite(LED2, LOW);
																								digitalWrite(LED3, LOW);
																								onOff = true;
																}
								}
								blinkCtr++;
}
