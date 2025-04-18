#pragma once

#include <cstdint>

#include "Adafruit_NeoPixel.h"

#include "devices/NeoPixel.h"

class NeoPixelAction
{
public:
    NeoPixelAction(NeoPixel::Color on_color=NeoPixel::Color::ON)    : _on_color(on_color) {}
    void performAction(uint8_t actionDurationSeconds);

private:
    NeoPixel::Color _on_color;
    NeoPixel _neoPixel;
};
