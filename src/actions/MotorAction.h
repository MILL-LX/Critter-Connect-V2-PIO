#pragma once

#include <cstdint>

#include "devices/ApplicationDevices.h"

class MotorAction
{
public:
    MotorAction(uint32_t durationMillis, Motor& motor) : _durationMillis(durationMillis), _motor(motor) {}

    void performAction();

private:
    uint32_t _durationMillis;
    Motor &_motor;
};
