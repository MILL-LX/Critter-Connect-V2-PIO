#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "NeoPixelAction.h"
#include "devices/NeoPixel.h"

void NeoPixelAction::performAction(uint32_t actionDurationMillis)
{
    TickType_t startTick = xTaskGetTickCount();
    // Convert duration directly from milliseconds to ticks
    TickType_t durationTicks = pdMS_TO_TICKS(actionDurationMillis);
    TickType_t endTick = startTick + durationTicks;

    const TickType_t delayIntervalTicks = pdMS_TO_TICKS(actionDurationMillis / 2); // Keep interval as is, or adjust if needed

    // Update log message if desired
    Serial.println("Performing NeoPixelAction for " + String(actionDurationMillis) + " ms.");
    bool on = true;
    while (xTaskGetTickCount() < endTick)
    {
        NeoPixel::StateColor color = on ? _on_color: NeoPixel::StateColor::OFF;
        on = !on;

        _neoPixel.setColor(color);

        TickType_t currentTime = xTaskGetTickCount();
        TickType_t remainingTime = (endTick > currentTime) ? (endTick - currentTime) : 0; // Prevent underflow

        if (remainingTime == 0) {
             break; // Exit if duration already passed
        } else if (remainingTime < delayIntervalTicks)
        {
            vTaskDelay(remainingTime); // Delay for the exact remaining time
            break; // Exit after final delay
        }
        else
        {
            vTaskDelay(delayIntervalTicks); // Delay for the standard interval
        }
    }

    _neoPixel.setColor(NeoPixel::StateColor::OFF); // Ensure LED is off at the end
    Serial.println("Finished performing NeoPixelAction.");
}
