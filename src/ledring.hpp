#ifndef DOSTUFF_H
#define DOSTUFF_H

#include <Arduino.h>

#define MOTOR D0
#define LED1 D1 // R
#define LED2 D2 // B
#define LED3 D3 // G

static bool _initialized;
static bool _motorState;
static bool _ledState;

class LedRing
{

public:
    static void initialize()
    {
        pinMode(MOTOR, OUTPUT);
        pinMode(LED1, OUTPUT);
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

    static bool toggleMotor()
    {
        _motorState == false ? acitvateMotor(true) : acitvateMotor(false);
        return _motorState;
    }

};

#endif
