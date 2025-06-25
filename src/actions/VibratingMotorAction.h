#pragma once

#include <cstdint>
#include <atomic>

#include "devices/ApplicationDevices.h"

class VibratingMotorAction
{
public:
    VibratingMotorAction(uint32_t durationMillis, uint32_t pulseDurationMillis, VibratingMotor vibratingMotor) : _durationMillis(durationMillis),
                                                                                                                 _pulseDurationMillis(pulseDurationMillis),
                                                                                                                 _vibratingMotor(vibratingMotor)
    {
    }

    void performAction();
    bool isActive();

private:
    uint32_t _durationMillis;
    uint32_t _pulseDurationMillis;
    VibratingMotor _vibratingMotor;
    std::atomic<bool> _isActive{false};

    void setActive(bool value);
};
