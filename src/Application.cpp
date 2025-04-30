#include <Arduino.h>

#include <FreeRTOS.h>
#include "task.h"

#include "Application.h"
#include "devices/GPSReceiver.h"

// We don't need to update our location very frequently since
// the GPS receiver is with a person who is walking.
const ulong gpsCheckIntervalMillis = 1000;

GPSReceiver *gpsReceiver = nullptr;

void Application::run()
{
    if (_is_running.load())
    {
        Serial.println("Application already running, ignoring run().");
        return;
    }
    else {
        Serial.println("Application starting...");
        gpsReceiver = new GPSReceiver();
    }

    long lastFound = 0;
    while (true)
    {
        long currentMillis = millis();

        Serial.printf("Checking for location update at %u...\n", currentMillis);
        GPSReceiver::GPSData gpsData = gpsReceiver->readData();
        if(gpsData.dataReady)
        {
            Serial.println("GPS Data Found, upddating application state...");
            lastFound = currentMillis;
        }
        else
        {
            Serial.println("GPS Data Not Found...");
            long timeSinceFound = currentMillis - lastFound;

            if(timeSinceFound > 10000)
                Serial.println("It's been too long since we got a location update.");
        }

        Serial.printf("Waiting %ums for next location check...\n", gpsCheckIntervalMillis);
        vTaskDelay(pdMS_TO_TICKS(gpsCheckIntervalMillis));
    }
}