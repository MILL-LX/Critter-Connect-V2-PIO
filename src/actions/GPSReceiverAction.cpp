#include <Arduino.h>

#include <FreeRTOS.h>
#include "task.h"

#include "GPSReceiverAction.h"
#include "app/SpeciesZone.h"

#include "devices/ApplicationDevices.h"

const ulong gpsCheckIntervalMillis = 100;
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
        setActive(true);
    }

    long lastFound = 0;        // Initialize lastFound, perhaps with millis() if needed immediately
    while (isActive())
    {
        ulong currentMillis = millis();

        Serial.printf("\n\nChecking for location update at %lu...\n", currentMillis);
        GPSReceiver::GPSData gpsData = _gpsReceiver.readData();

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
        _periodicNeopixelAction->stop();
        while(_periodicNeopixelAction->isActive())
        {
            Serial.println("Waiting for Periodic NeoPixel Action to stop...");
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        _neoPixel.setColor(NeoPixel::StateColor::OK);

        if (ApplicationDevices::getInstance().getButton().isPressed())
        {
            Serial.println("Button pressed, playing sound.");
            if (!ApplicationDevices::getInstance().getSoundPlayer().isPlaying())
            {
                SoundPlayer::Sound sound = (currentZone == SpeciesZone::Zone::SPECIES_FROG_ZONE) ? SoundPlayer::Sound::SPECIES_FROG : SoundPlayer::Sound::SPECIES_PIGEON;
                ApplicationDevices::getInstance().getSoundPlayer().playSound(SoundPlayer::Sound::SPECIES_FROG);
            }
        }
        else
        {
            Serial.println("Button not pressed, not playing sound.");
        }

        if (currentZone != _previousZone)
            Serial.println("Entering %s zone.");
        else
            Serial.println("Inside %s zone.");
        break;
    default:
        Serial.println("Species Zone Status INVALID - Proximity check returned unexpected value.");
        _periodicNeopixelAction->stop();
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