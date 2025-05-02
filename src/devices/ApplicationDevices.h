#pragma once

#include <Arduino.h>
#include <memory>
#include <stdexcept>

#include "devices/GPSReceiver.h"
#include "devices/Motor.h"
#include "devices/NeoPixel.h"
#include "devices/SoundButton.h"
#include "devices/SoundPlayer.h"

class ApplicationDevices
{
public:
    ApplicationDevices() = default;

    // Call this method once during your application's initialization phase
    // to ensure all devices are created upfront.
    void setup() {
        getButton();      // Initializes _button if null
        getGpsReceiver(); // Initializes _gpsReceiver if null
        getMotor();       // Initializes _motor if null
        getNeoPixel();    // Initializes _neoPixel if null
        getSoundPlayer(); // Initializes _soundPlayer if null
    }

    SoundButton& getButton() {
        if (!_button) {
            _button = std::make_unique<SoundButton>();
        }
        return *_button;
    }

    GPSReceiver& getGpsReceiver() {
        if (!_gpsReceiver) {
            // Remember to add constructor arguments here if needed by GPSReceiver
            _gpsReceiver = std::make_unique<GPSReceiver>();
        }
        return *_gpsReceiver;
    }

    Motor& getMotor() {
        if (!_motor) {
            // Remember to add constructor arguments here if needed by Motor
             _motor = std::make_unique<Motor>();
        }
        return *_motor;
    }

    NeoPixel& getNeoPixel() {
        if (!_neoPixel) {
            _neoPixel = std::make_unique<NeoPixel>();
        }
        return *_neoPixel;
    }

    SoundPlayer& getSoundPlayer() {
        if (!_soundPlayer) {
             // Remember to add constructor arguments here if needed by SoundPlayer
            _soundPlayer = std::make_unique<SoundPlayer>();
        }
        return *_soundPlayer;
    }

    ~ApplicationDevices() = default;

    ApplicationDevices(const ApplicationDevices&) = delete;
    ApplicationDevices& operator=(const ApplicationDevices&) = delete;

    ApplicationDevices(ApplicationDevices&&) = default;
    ApplicationDevices& operator=(ApplicationDevices&&) = default;

private:
    std::unique_ptr<SoundButton> _button = nullptr;
    std::unique_ptr<GPSReceiver> _gpsReceiver = nullptr;
    std::unique_ptr<Motor> _motor = nullptr;
    std::unique_ptr<NeoPixel> _neoPixel = nullptr;
    std::unique_ptr<SoundPlayer> _soundPlayer = nullptr;
};
