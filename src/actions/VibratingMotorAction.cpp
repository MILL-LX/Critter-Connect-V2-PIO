#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "VibratingMotorAction.h"

void VibratingMotorAction::performAction()
{
    // Convert duration directly from milliseconds to ticks
    TickType_t durationTicks = pdMS_TO_TICKS(_durationMillis);
    TickType_t endTicks = xTaskGetTickCount() + durationTicks;
    TickType_t pulseDurationTicks = pdMS_TO_TICKS(_pulseDurationMillis) / 2; // Divide by 2 for on/off pulse duration

    Serial.println("Performing VibratingMotorAction for " + String(_durationMillis) + " ms.");
    setActive(true);
    while (xTaskGetTickCount() < endTicks)
    {
        // Start the vibrating motor
        _vibratingMotor.start();
        vTaskDelay(pulseDurationTicks);
        // Stop the vibrating motor
        _vibratingMotor.stop();
        vTaskDelay(pulseDurationTicks);
    }
    setActive(false);
    Serial.println("Finished performing VibratingMotorAction.");
}

bool VibratingMotorAction::isActive()
{
    return _isActive.load();
}

void VibratingMotorAction::setActive(bool value)
{
    _isActive.store(value);
}