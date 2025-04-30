#pragma once

#include <cstdint>

#include "Adafruit_NeoPixel.h"

#include "devices/NeoPixel.h"

class NeoPixelAction
{
public:
    NeoPixelAction(uint32_t durationMillis, NeoPixel::StateColor on_color)    :_durationMillis(durationMillis), _on_color(on_color) {}
    void performAction();

private:
    uint32_t _durationMillis;
    NeoPixel::StateColor _on_color;
    NeoPixel _neoPixel;
};
