#pragma once

#include <cstdint>

#include "devices/ApplicationDevices.h"

class NeoPixelAction
{
public:
    // Initialize the reference _neoPixel in the initializer list
    NeoPixelAction(uint32_t durationMillis, NeoPixel::StateColor on_color)
        : _durationMillis(durationMillis),
          _on_color(on_color),
          _neoPixel(ApplicationDevices::getInstance().getNeoPixel()){}

    void performAction();

private:
    uint32_t _durationMillis;
    NeoPixel::StateColor _on_color;
    NeoPixel& _neoPixel; // Reference member
};
