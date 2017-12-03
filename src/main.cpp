#include "main.hpp"
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "strip.hpp"
#include "ledring.hpp"
#include "../../wlan.conf"

#ifndef LEDUPDATETIME
	#define LEDUPDATETIME 10 //ms
#endif

#ifndef MOTORPERIOD
	#define MOTORPERIOD 6000L
#endif

void setup() {

								LedRing::initialize();

								pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
								Serial.begin(115200);

								// LED Strip acitvate
								Serial.println("Strip");
								CLedStrip* strip = CLedStrip::getStrip_ptr();
								strip->init();
								strip->getConf().ctr = 0;
								strip->getConf().max = 5;
								strip->getConf().min = 0;
								strip->getConf().period = 10;
								strip->switch_program(1);
								Serial.println("Strip end");

								LedRing::acitvateLedRing(true);
								LedRing::acitvateMotor(true);
}

static uint32_t motorCtr = 0;

void loop()
{
								// update led pattern / program
								CLedStrip::getStrip_ptr()->update();
								delay(LEDUPDATETIME);

								if(motorCtr == MOTORPERIOD)
								{
																LedRing::toggleMotor();
																motorCtr = 0;
								}
								motorCtr++;
}
