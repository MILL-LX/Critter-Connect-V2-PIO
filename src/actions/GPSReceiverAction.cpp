#include <Arduino.h>

#include <FreeRTOS.h>
#include "task.h"

#include "GPSReceiverAction.h"
#include "app/SpeciesZone.h"

#include "devices/ApplicationDevices.h"

const ulong gpsCheckIntervalMillis = 60000;
void GPSReceiverAction::performAction()
{
    if (isActive())
    {
        Serial.println("GPSReceiverAction already running, ignoring performAction().");
        return;
    }
    else
    {
        Serial.println("GPSReceiverAction starting...");
        _neoPixel.setColor(NeoPixel::StateColor::START);
        setActive(true);
    }

    long lastFound = 0; // Initialize lastFound, perhaps with millis() if needed immediately
    while (isActive())
    {
        ulong currentMillis = millis();

        Serial.printf("\n\nChecking for location update at %lu...\n", currentMillis);
        GPSReceiver::GPSData gpsData = _gpsReceiver.readData();

        if (gpsData.locationValid)
        {
            // MODEBUG Serial.println("GPS Location Data Found, updating application state...");
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

        // MODEBUG Serial.printf("Waiting %lu ms for next location check...\n", gpsCheckIntervalMillis);
        vTaskDelay(pdMS_TO_TICKS(gpsCheckIntervalMillis));
    }

    Serial.println("GPSReceiverAction stopping...");
}

void GPSReceiverAction::stopAllActions()
{
    Serial.println("Stopping all periodic actions...");
    _periodicNeopixelAction->stop();

    _soundButtonAction_frog->stop();
    _soundButtonAction_pigeon->stop();

    _periodicVibratingMotorAction_long->stop();
    _periodicVibratingMotorAction_frog->stop();
    _periodicVibratingMotorAction_pigeon->stop();

    _periodicMotorAction_frog_long->stop();
    _periodicMotorAction_frog_short->stop();
    _periodicMotorAction_pigeon_long->stop();
    _periodicMotorAction_pigeon_short->stop();
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
        if (currentZone != _previousZone)
        {
            Serial.println("Entering non-species zone.");

            stopAllActions();

            _neoPixel.setColor(NeoPixel::StateColor::OFF);
            _periodicNeopixelAction->start();

            if (_previousZone == SpeciesZone::Zone::UNKNOWN_ZONE)
            {
                Serial.println("Skipping motor actions when application first starts...");
            }
            else
            {
                Serial.println("Left a species zone...");

                Serial.println("Starting long periodic motor actions...");
                _periodicVibratingMotorAction_long->start();
                _periodicMotorAction_frog_long->start();
                _periodicMotorAction_pigeon_long->start();
                Serial.println("Long periodic motor actions started.");
            }
        }
        else
        {
            Serial.println("Still in non-species zone");
        }
        break;
    case SpeciesZone::Zone::SPECIES_FROG_ZONE:
    case SpeciesZone::Zone::SPECIES_PIGEON_ZONE:
        if (currentZone != _previousZone)
        {
            Serial.printf("Entering %s zone.\n", (currentZone == SpeciesZone::Zone::SPECIES_FROG_ZONE) ? "Frog" : "Pigeon");

            stopAllActions();

            _neoPixel.setColor(NeoPixel::StateColor::OK);

            Serial.println("Starting periodic motor actions.");
            if (currentZone == SpeciesZone::Zone::SPECIES_FROG_ZONE)
            {
                _soundButtonAction_frog->start();
                _periodicMotorAction_frog_short->start();
                _periodicVibratingMotorAction_frog->start();
            }
            else if (currentZone == SpeciesZone::Zone::SPECIES_PIGEON_ZONE)
            {
                _soundButtonAction_pigeon->start();
                _periodicMotorAction_pigeon_short->start();
                _periodicVibratingMotorAction_frog->start();
            }
        }
        else
        {
            Serial.printf("Still in %s zone.\n", (currentZone == SpeciesZone::Zone::SPECIES_FROG_ZONE) ? "Frog" : "Pigeon");
        }
        break;

    default:
        Serial.println("Species Zone Status INVALID - Proximity check returned unexpected value.");
        stopAllActions();
        _neoPixel.setColor(NeoPixel::StateColor::WARN);
        break;
    }

    _previousZone = currentZone;
}

bool GPSReceiverAction::isActive()
{
    return _isActive.load();
}
void GPSReceiverAction::setActive(bool value)
{
    _isActive.store(value);
}
