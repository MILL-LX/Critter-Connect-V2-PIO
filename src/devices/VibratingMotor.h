#pragma once

#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

namespace VibratingMotorDefs {
    const int defaultPin = 26;
}


class VibratingMotor
{
public:


    VibratingMotor(int pin = VibratingMotorDefs::defaultPin) : _pin(pin) {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW); // Ensure the motor is off initially
    };

    void start() {
        digitalWrite(_pin, HIGH); // Turn on the motor
    }
    
    void stop() {
        digitalWrite(_pin, LOW); // Turn off the motor
    }

private:
    int _pin;
};
