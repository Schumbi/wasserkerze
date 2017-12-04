#include "main.hpp"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "strip.hpp"
#include "ledring.hpp"
#include "../../wlan.conf"

const char* myssid = MAKELIGHT_SSID;
const char* mypass = MAKELIGHT_PASS;

const char* host = "wasserkerze";
const char* update_path="/firmware";
const char* update_username="wasserkerze";
const char* update_passwort="DasIstDasPasswortDerWasserkerze.";
const int updater_port=80;

#ifndef LEDUPDATETIME
	#define LEDUPDATETIME 10 //ms
#endif

#ifndef MOTORPERIOD
	#define MOTORPERIOD 6000L
#endif

ESP8266WebServer httpServer(updater_port);
ESP8266HTTPUpdateServer httpUpdater;

void setup_wifi();
void setup_OTA();

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

								setup_wifi();

								setup_OTA();
}

static uint32_t motorCtr = 0;

void loop()
{
								//update led pattern / program
								CLedStrip::getStrip_ptr()->update();
								httpServer.handleClient();
								delay(LEDUPDATETIME);
								//
								if(motorCtr == MOTORPERIOD)
								{
																if(LedRing::toggleMotor())
																{
																								Serial.print("Motor ON ");
																}
																else
																{
																								Serial.print("Motor OFF");
																}
																motorCtr = 0;
																Serial.println("");
								}
								motorCtr++;

}

void setup_wifi()
{
								digitalWrite(BUILTIN_LED, LOW);
								delay(10);
								// We start by connecting to a WiFi network
								Serial.println();
								Serial.print("Connecting to ");
								Serial.println(myssid);

								WiFi.mode(WIFI_AP_STA);
								WiFi.begin(myssid, mypass);

								while (WiFi.waitForConnectResult() != WL_CONNECTED)
								{
																digitalWrite(BUILTIN_LED, LOW);
																WiFi.begin(myssid, mypass);
																Serial.print(".");
																digitalWrite(BUILTIN_LED, HIGH);
								}
								Serial.println("");
								Serial.println("WiFi connected");
								Serial.println("IP address: ");
								Serial.println(WiFi.localIP());
								digitalWrite(BUILTIN_LED, HIGH);
								delay(100);
								setup_OTA();

}

void setup_OTA()
{
								digitalWrite(BUILTIN_LED, LOW);

								MDNS.begin(host);
								httpUpdater.setup(&httpServer, update_path, update_username, update_passwort);
								//httpUpdater.setup(&httpServer, "/firmware");
								httpServer.begin();
								MDNS.addService("http", "tcp", updater_port);
								Serial.println("OTA setup finished!");
}
