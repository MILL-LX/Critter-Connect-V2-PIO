#pragma once

#include <cstdint>
#include <atomic>

#include "devices/ApplicationDevices.h"

class MotorAction
{
public:
    MotorAction(uint8_t iterations, Motor &motor) : _iterations(iterations), _motor(motor) {}

    void performAction();
    bool isActive();
    void setActive(bool value);

private:
    uint8_t _iterations;
    Motor &_motor;
    std::atomic<bool> _isActive{false};
};
