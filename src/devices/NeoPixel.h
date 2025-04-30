#pragma once

#include <Adafruit_NeoPixel.h>

const int defaultPin = 16;
const int defaultNumPixels = 1;

class NeoPixel
{
public:
    enum StateColor
    {
        OFF = 0x000000,
        TEST = 0x00ff00,
        WARN = 0xff0000,
        OK = 0xffffff
    };

    void setColor(StateColor color)
    {
        _strip->setPixelColor(0, color);
        _strip->show();
    }


    NeoPixel(int pin = defaultPin, int numPixels = defaultNumPixels) : _strip(new Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800)) {
        _strip->begin();
        setColor(OFF);
    };

    ~NeoPixel() {
        if (_strip) {
            setColor(StateColor::OFF);
            delete _strip;
            _strip = nullptr;
        }
    }

private:
    Adafruit_NeoPixel *_strip;
};
