#include <Arduino.h>

#include <FreeRTOS.h>
#include "task.h"

#include "GPSReceiverAction.h"
#include "app/SpeciesZone.h"

void debugDumpGPSData(GPSReceiver::GPSData data)
{
    Serial.println("--- GPS Data Dump ---");
    Serial.printf("Data Ready: %s\n", data.dataReady ? "Yes" : "No");

    Serial.printf("Location Valid: %s\n", data.locationValid ? "Yes" : "No");
    if (data.locationValid)
    {
        Serial.printf("  Latitude: %.6f\n", data.lat); // Use %.6f for float precision
        Serial.printf("  Longitude: %.6f\n", data.lon);
    }

    Serial.printf("Date Valid: %s\n", data.dateValid ? "Yes" : "No");
    if (data.dateValid)
    {
        Serial.printf("  Date: %04d-%02d-%02d\n", data.year, data.month, data.day); // Format YYYY-MM-DD
    }

    Serial.printf("Time Valid: %s\n", data.timeValid ? "Yes" : "No");
    if (data.timeValid)
    {
        Serial.printf("  Time: %02d:%02d:%02d.%02d\n", data.hour, data.minute, data.second, data.centisecond); // Format HH:MM:SS.CS
    }
    Serial.println("--- End GPS Data Dump ---");
}

// We don't need to update our location very frequently since
// the GPS receiver is with a person who is walking.
const ulong gpsCheckIntervalMillis = 30000;
void GPSReceiverAction::performAction()
{
    if (_is_running.load())
    {
        Serial.println("GPSReceiverAction already running, ignoring run().");
        return;
    }
    else
    {
        Serial.println("GPSReceiverAction starting...");
        _is_running.store(true); // Set the flag indicating it's running
    }

    long lastFound = 0;        // Initialize lastFound, perhaps with millis() if needed immediately
    while (_is_running.load()) // Check the atomic flag to allow stopping the loop
    {
        ulong currentMillis = millis();

        Serial.printf("\n\nChecking for location update at %lu...\n", currentMillis);
        GPSReceiver::GPSData gpsData = _gpsReceiver.readData();
        debugDumpGPSData(gpsData);

        if (gpsData.locationValid)
        {
            Serial.println("GPS Location Data Found, updating application state...");
            lastFound = currentMillis;

            processLocationUpdate(gpsData);
        }
        else
        {
            // Only print time since last found if a location *was* previously found
            if (lastFound > 0)
            {
                ulong timeSinceFound = currentMillis - lastFound;
                Serial.printf("GPS Location Data not found for %lums...\n", timeSinceFound);
            }
            else
            {
                Serial.println("GPS Location Data not found yet...");
            }
        }

        Serial.printf("Waiting %lu ms for next location check...\n", gpsCheckIntervalMillis);
        vTaskDelay(pdMS_TO_TICKS(gpsCheckIntervalMillis));
    }

    Serial.println("GPSReceiverAction stopping...");
}

void GPSReceiverAction::processLocationUpdate(GPSReceiver::GPSData gpsData)
{
    // Ensure gpsData.locationValid is true before using lat/lon
    if (!gpsData.locationValid)
    {
        Serial.println("Skipping proximity check due to invalid location data.");
        return;
    }

    SpeciesZone::Zone currentZone = SpeciesZone::zoneForLatLon(gpsData.lat, gpsData.lon);
    switch (currentZone)
    {
    case SpeciesZone::Zone::NON_SPECIES_ZONE:
        _periodicNeopixelAction->start();

        if (currentZone != _previousZone)
            Serial.println("Leaving species zone.");
        else
            Serial.println("Outside of all species zones.");
        break;
    case SpeciesZone::Zone::SPECIES_FROG_ZONE:
    case SpeciesZone::Zone::SPECIES_PIGEON_ZONE:
        _periodicNeopixelAction->stop(); //MOFIX - make this a hard stop
        _neoPixel.setColor(NeoPixel::StateColor::OK);

        if (currentZone != _previousZone)
            Serial.println("Entering %s zone.");
        else
            Serial.println("Inside %s zone.");
        break;
    default:
        // This case should ideally not be reached if the enum is handled correctly.
        Serial.println("Species Zone Status INVALID - Proximity check returned unexpected value.");
        _periodicNeopixelAction->stop();
        _neoPixel.setColor(NeoPixel::StateColor::WARN);
        break;
    }

    _previousZone = currentZone;
}
