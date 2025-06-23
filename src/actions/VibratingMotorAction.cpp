#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "VibratingMotorAction.h"

void VibratingMotorAction::performAction()
{
    // Convert duration directly from milliseconds to ticks
    TickType_t durationTicks = pdMS_TO_TICKS(_durationMillis);

    Serial.println("Performing VibratingMotorAction for " + String(_durationMillis) + " ms.");
    setActive(true);
    
    _vibratingMotor.start();
    vTaskDelay(pdMS_TO_TICKS(durationTicks));
    _vibratingMotor.stop();

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