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
#include <TickerScheduler.h>''

#include "strip.h"
#include "../../wlan.conf"

// Update these with values suitable for your network.

void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);

const char* ssid = MAKELIGHT_SSID;
const char* password = MAKELIGHT_PASS;
const char* mqtt_server = "192.168.3.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

#define LED_TICK 4
#define MOTOR D0
#define LED1 D5
#define LED2 D6
#define LED3 D8


void update_leds();
void blink();
void update_mqtt_status();

TickerScheduler ticker(5);

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  ticker.add(0, LED_TICK, update_leds);
  ticker.add(1, 1000, update_mqtt_status);
  ticker.add(2, 1000, blink);


  // LED Strip acitvate
  Serial.println("Strip");
  CLedStrip* strip = CLedStrip::getStrip_ptr();
  strip->init();
  strip->getConf().ctr = 5;
  strip->getConf().min = 0;
  strip->getConf().period = 5;
  strip->switch_program(1);
  Serial.println("Strip end");

}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, size_t length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (size_t i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/home/test", "hello world");
      // ... and resubscribe
      client.subscribe("/home/makelight");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  ticker.update();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
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

void update_mqtt_status()
{
    long now = millis();
    if (now - lastMsg > 2000) {
      lastMsg = now;
      ++value;
      snprintf (msg, 75, "hello world #%d", value);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("/home/test", msg);
    }
}
