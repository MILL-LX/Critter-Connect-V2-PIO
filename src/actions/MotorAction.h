#pragma once

#include <cstdint>

#include "devices/ApplicationDevices.h"

class MotorAction
{
public:
    MotorAction(uint8_t iterations, Motor& motor) : _iterations(iterations), _motor(motor) {}

    void performAction();

private:
    uint8_t _iterations;
    Motor &_motor;
};
