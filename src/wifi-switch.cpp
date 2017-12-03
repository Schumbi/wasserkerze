#include "wifi-switch.h"
#include <Arduino.h>


/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "strip.h"
#include "../../wlan.conf"

// Update these with values suitable for your network.

int value = 0;

#define MOTOR D0
#define LED1 D5
#define LED2 D6
#define LED3 D8


void update_leds();
void blink();

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);

  // LED Strip acitvate
  Serial.println("Strip");
  CLedStrip* strip = CLedStrip::getStrip_ptr();
  strip->init();
  strip->getConf().ctr = 0;
  strip->getConf().max = 100;
  strip->getConf().min = 0;
  strip->getConf().period = 5;
  strip->switch_program(1);
  Serial.println("Strip end");
}

void loop()
{
  update_leds();
  //delay(100);
}

void update_leds()
{
	// update led pattern / program
	CLedStrip::getStrip_ptr()->update();
}

static bool onOff = false;

void blink()
{
  if(onOff == true)
  {
    digitalWrite(MOTOR, HIGH);
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
