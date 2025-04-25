#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "MotorAction.h"

const unsigned long pauseDuration = 500; // ms pause between movements

void MotorAction::performAction(uint32_t actionDurationMillis)
{
    TickType_t startTick = xTaskGetTickCount();
    // Convert duration directly from milliseconds to ticks
    TickType_t durationTicks = pdMS_TO_TICKS(actionDurationMillis);
    TickType_t endTick = startTick + durationTicks;

    const TickType_t delayIntervalTicks = pdMS_TO_TICKS(actionDurationMillis);

    Serial.println("Performing MotorAction for " + String(actionDurationMillis) + " ms.");

    Motor::Direction direction = Motor::Direction::FORWARD;
    while (xTaskGetTickCount() < endTick)
    {
        TickType_t currentTime = xTaskGetTickCount();
        TickType_t remainingTime = (endTick > currentTime) ? (endTick - currentTime) : 0; // Prevent underflow

        _motor.moveFullRangeInDirection(direction);
        direction = direction == Motor::Direction::FORWARD ? Motor::Direction::REVERSE : Motor::Direction::FORWARD;

        if (remainingTime == 0)
        {
            break; // Exit if duration already passed
        }
        else if (remainingTime < delayIntervalTicks)
        {
            vTaskDelay(remainingTime); // Delay for the exact remaining time
            break;                     // Exit after final delay
        }
        else
        {
            vTaskDelay(delayIntervalTicks); // Delay for the standard interval
        }
    }

    Serial.println("Finished performing MotorAction.");
}
