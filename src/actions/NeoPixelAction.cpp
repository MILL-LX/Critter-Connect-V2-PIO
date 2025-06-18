#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "NeoPixelAction.h"

void NeoPixelAction::performAction()
{
    TickType_t startTick = xTaskGetTickCount();
    
    // Convert duration directly from milliseconds to ticks
    TickType_t durationTicks = pdMS_TO_TICKS(_durationMillis);
    TickType_t endTick = startTick + durationTicks;

    const TickType_t delayIntervalTicks = durationTicks / 2;

    Serial.println("Performing NeoPixelAction for " + String(_durationMillis) + " ms.");
    bool on = true;
    setActive(true);
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
    setActive(false);

    _neoPixel.setColor(NeoPixel::StateColor::OFF); // Ensure LED is off at the end
    Serial.println("Finished performing NeoPixelAction.");
}

bool NeoPixelAction::isActive()
{
    return _isActive.load();
}

void NeoPixelAction::setActive(bool value)
{
    _isActive.store(value);
}