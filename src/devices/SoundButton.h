#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

namespace SoundButtonDefs
{
    const int defaultPin = 4;
    const TickType_t defaultDebounceDelay = pdMS_TO_TICKS(50);
}

class SoundButton
{
public:
    SoundButton(int pin = SoundButtonDefs::defaultPin, TickType_t debounceDelay = SoundButtonDefs::defaultDebounceDelay) : _pin(pin),
                                                                                                                           _debounceDelay(debounceDelay)
    {
        pinMode(_pin, INPUT_PULLUP);
    };

    bool isPressed()
    {
        // Read button state
        int reading = digitalRead(_pin);
        
        // If pressed, wait for debounce period and confirm
        if (reading == LOW)
        {
            vTaskDelay(_debounceDelay);
            return (digitalRead(_pin) == LOW);
        }
        
        return false;
    }

private:
    int _pin;
    TickType_t _debounceDelay;
};