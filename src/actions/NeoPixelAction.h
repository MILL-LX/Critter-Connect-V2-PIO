#pragma once

#include <cstdint>

#include "Adafruit_NeoPixel.h"

#include "devices/NeoPixel.h"

class NeoPixelAction
{
public:
    NeoPixelAction(NeoPixel::StateColor on_color)    : _on_color(on_color) {}
    void performAction(uint32_t actionDurationMillis);

private:
    NeoPixel::StateColor _on_color;
    NeoPixel _neoPixel;
};
