#ifndef _DOSTUFF_H
#define _DOSTUFF_H

#include <Arduino.h>

#define MOTOR D1
#define LED1 D2
#define LED2 D3
#define LED3 TX

static bool _initialized;
static bool _motorState;
static bool _ledState;

class LedRing
{

public:
static void initialize()
{
								pinMode(MOTOR, OUTPUT);
								pinMode(D3, OUTPUT);
								pinMode(LED2, OUTPUT);
								pinMode(LED3, OUTPUT);

								_motorState = false;
								_ledState = false;

								_initialized = true;
}

static void acitvateLedRing(bool state)
{
								if(!_initialized)
																initialize();

								if(state == true)
								{

																digitalWrite(LED1, HIGH);
																digitalWrite(LED2, HIGH);
																digitalWrite(LED3, HIGH);
																_ledState = true;
								}
								else
								{
																digitalWrite(LED1, LOW);
																digitalWrite(LED2, LOW);
																digitalWrite(LED3, LOW);
																_ledState = false;
								}
}

static void acitvateMotor(bool state)
{
								if(!_initialized)
																initialize();

								if(state == true)
								{
																digitalWrite(MOTOR, HIGH);
																_motorState = true;
								}
								else
								{
																digitalWrite(MOTOR, LOW);
																_motorState = false;
								}
}

static void toggleMotor()
{
								_motorState == false ? acitvateMotor(true) : acitvateMotor(false);
}

};

#endif
