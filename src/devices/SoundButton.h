#pragma once

#include <Arduino.h>

namespace SoundButtonDefs
{
    const int defaultPin = 6;
    const unsigned long defaultDebounceDelay = 50;
}

class SoundButton
{
public:
    SoundButton(int pin = SoundButtonDefs::defaultPin, unsigned long debounceDelay = SoundButtonDefs::defaultDebounceDelay) : _pin(pin),
                                                                                                       _debounceDelay(debounceDelay),
                                                                                                       _lastDebounceTime(0),
                                                                                                       _lastButtonState(HIGH),
                                                                                                       _buttonState(HIGH)
    {
        pinMode(_pin, INPUT_PULLUP);
    };

    bool isPressed()
    {
        int reading = digitalRead(_pin);

        if (reading != _lastButtonState)
        {
            _lastDebounceTime = millis();
        }

        if ((millis() - _lastDebounceTime) > _debounceDelay)
        {
            if (reading != _buttonState)
            {
                _buttonState = reading;
            }
        }

        _lastButtonState = reading;

        return (_buttonState == LOW);
    }

private:
    int _pin;
    unsigned long _debounceDelay;
    unsigned long _lastDebounceTime;
    int _lastButtonState;
    int _buttonState;
};
