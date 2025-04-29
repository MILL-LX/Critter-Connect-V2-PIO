#include <Arduino.h>

#include <FreeRTOS.h>
#include "task.h"

#include "Application.h"

// We don't need to update our location very frequently since
// the GPS receiver is with a person who is walking.
const ulong gpsCheckIntervalMillis = 1000;

void Application::run()
{
    if (_is_running.load())
    {
        Serial.println("Application already running, ignoring run().");
        return;
    }
    else {
        Serial.println("Application starting...");
    }

    while (true)
    {
        Serial.println("Checking for location update...");

        Serial.println("Waiting " + String(gpsCheckIntervalMillis) + "ms for next check interval...");
        vTaskDelay(pdMS_TO_TICKS(gpsCheckIntervalMillis));
    }
}