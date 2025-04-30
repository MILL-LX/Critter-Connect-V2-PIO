#include <Arduino.h>

#include <FreeRTOS.h>
#include "task.h"

#include "GPSReceiverAction.h"
#include "app/SpeciesProximityChecker.h"

// We don't need to update our location very frequently since
// the GPS receiver is with a person who is walking.
const ulong gpsCheckIntervalMillis = 60000;

GPSReceiver *gpsReceiver = nullptr;

void GPSReceiverAction::performAction()
{
    if (_is_running.load())
    {
        Serial.println("Application already running, ignoring run().");
        return;
    }
    else
    {
        Serial.println("Application starting...");
        gpsReceiver = new GPSReceiver();
    }

    long lastFound = 0;
    while (true)
    {
        long currentMillis = millis();

        Serial.printf("Checking for location update at %u...\n", currentMillis);
        GPSReceiver::GPSData gpsData = gpsReceiver->readData();
        if (gpsData.locationValid)
        {
            Serial.println("GPS Location Data Found, upddating application state...");
            lastFound = currentMillis;

            processLocationUpdate(gpsData);
        }
        else
        {
            long timeSinceFound = currentMillis - lastFound;
            Serial.printf("GPS Data not found for %ums...", timeSinceFound);
        }

        Serial.printf("Waiting %ums for next location check...\n", gpsCheckIntervalMillis);
        vTaskDelay(pdMS_TO_TICKS(gpsCheckIntervalMillis));
    }
}

SpeciesProximityChecker checker;
void GPSReceiverAction::processLocationUpdate(GPSReceiver::GPSData gpsData)
{
    switch (checker.checkProximity(gpsData.lat, gpsData.lon))
    {
    case SpeciesProximityChecker::OUTSIDE_ZONES:
        break;
    case SpeciesProximityChecker::IN_ZONE_SPECIES_1:
        break;
    case SpeciesProximityChecker::IN_ZONE_SPECIES_2:
        break;
    }
}