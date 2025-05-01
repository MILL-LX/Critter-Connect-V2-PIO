#include <Arduino.h>

#include <FreeRTOS.h>
#include "task.h"

#include "GPSReceiverAction.h"
#include "app/SpeciesProximityChecker.h"

// We don't need to update our location very frequently since
// the GPS receiver is with a person who is walking.
const ulong gpsCheckIntervalMillis = 10000;

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

        Serial.printf("\n\nChecking for location update at %u...\n", currentMillis);
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
            Serial.printf("GPS Location Data not found for %ums...\n", timeSinceFound);
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
        Serial.println("Outside of all species zones.");
        break;
    case SpeciesProximityChecker::IN_ZONE_SPECIES_1:
        Serial.println("Inside Species 1 zone.");
        break;
    case SpeciesProximityChecker::IN_ZONE_SPECIES_2:
        Serial.println("Inside Species 2 zone.");
        break;
    default:
        Serial.println("Species Zone Status INVALID");
        break;
    }
}