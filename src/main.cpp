#include "main.hpp"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "strip.hpp"
#include "ledring.hpp"
#include "conf.hpp"
#include "../pwd.hpp"
#include "../../wlan.conf"


const char* myssid = MAKELIGHT_SSID;
const char* mypass = MAKELIGHT_PASS;

const char* host = WEBNAME;
const char* update_path=UPDATEPATH;
const char* update_username=UPDATE_USERNAME;
const char* update_passwort=UPDATE_PASSWORT;
const int updater_port=UPDATE_PORT;

#ifndef LEDUPDATETIME
	#define LEDUPDATETIME 10 //ms
#endif

#ifndef MOTORPERIOD
	#define MOTORPERIOD 3000L
#endif

#ifndef MOTORPERIODON
	#define MOTORPERIODON 5
#endif
#ifndef MOTORPERIODOFF
	#define MOTORPERIODOFF 1
#endif

ESP8266WebServer httpServer(updater_port);
ESP8266HTTPUpdateServer httpUpdater;

void setup_wifi();
void setup_OTA();
void updateMotor();

void setup() {

								LedRing::initialize();

								pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
								Serial.begin(115200);

								// LED Strip acitvate
								Serial.println("Strip");
								CLedStrip* strip = CLedStrip::getStrip_ptr();
								strip->init();
								strip->getConf().ctr = 0;
								strip->getConf().period = 1;
								strip->getConf().max = 180; //RgbColor(ORANGRED).B;
								strip->getConf().min = 0;
								strip->switch_program(2);
								Serial.println("Strip end");

								LedRing::acitvateLedRing(true);
								LedRing::acitvateMotor(true);

								setup_wifi();
								setup_OTA();
}


void loop()
{
								//update led pattern / program
								CLedStrip::getStrip_ptr()->update();
								httpServer.handleClient();
								delay(LEDUPDATETIME);
								updateMotor();
}

static uint32_t motorCtr = 0;
static bool justStarted = true;
void updateMotor()
{
								if(justStarted == false)
								{

																if(motorCtr < ((double)MOTORPERIOD * (double)MOTORPERIODOFF / (double)MOTORPERIODON))
																{
																								LedRing::acitvateMotor(true);
																								Serial.print("Motor ON ");
																}
																else
																{
																								LedRing::acitvateMotor(false);
																								Serial.print("Motor OFF");
																}
																if(motorCtr >= MOTORPERIOD)
																{
																								motorCtr = 0;
																}
								}
								else
								{
																if(motorCtr >= MOTORPERIOD * 2)
																{
																								justStarted = false;
																}

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

								WiFi.mode(WIFI_AP);
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
								httpServer.begin();
								MDNS.addService("http", "tcp", updater_port);
								Serial.println("OTA setup finished!");
}
