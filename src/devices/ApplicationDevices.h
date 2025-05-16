#pragma once

#include <Arduino.h>
#include <memory>
#include <stdexcept> // Keep for potential future use, though not strictly needed now

#include "devices/GPSReceiver.h"
#include "devices/Motor.h"
#include "devices/NeoPixel.h"
#include "devices/SoundButton.h"
#include "devices/SoundPlayer.h"

class ApplicationDevices
{
public:
    // Delete copy constructor and assignment operator
    ApplicationDevices(const ApplicationDevices &) = delete;
    ApplicationDevices &operator=(const ApplicationDevices &) = delete;

    // Delete move constructor and assignment operator (optional but good practice for singletons)
    ApplicationDevices(ApplicationDevices &&) = delete;
    ApplicationDevices &operator=(ApplicationDevices &&) = delete;

    // Public static method to get the single instance
    static ApplicationDevices &getInstance()
    {
        // Use a static local variable to ensure thread-safe initialization
        // on first call (guaranteed by C++11 standard and later)
        static ApplicationDevices instance;
        return instance;
    }

    // Call this method once during your application's initialization phase
    // to ensure all devices are created upfront.
    void setup()
    {
        // Access members directly within the class
        getButton();      // Initializes _button if null
        getGpsReceiver(); // Initializes _gpsReceiver if null
        getMotor1();      // Initializes _motor if null
        getNeoPixel();    // Initializes _neoPixel if null
        getSoundPlayer(); // Initializes _soundPlayer if null
    }

    // --- Accessor methods ---
    SoundButton &getButton()
    {
        if (!_button)
        {
            _button = std::make_unique<SoundButton>();
        }
        return *_button;
    }

    GPSReceiver &getGpsReceiver()
    {
        if (!_gpsReceiver)
        {
            // Remember to add constructor arguments here if needed by GPSReceiver
            _gpsReceiver = std::make_unique<GPSReceiver>();
        }
        return *_gpsReceiver;
    }

    Motor &getMotor1()
    {
        if (!_motor1)
        {
            // Remember to add constructor arguments here if needed by Motor
            _motor1 = std::make_unique<Motor>(motor1EnablePin, motor1StepPin, motor1DirectionPin);
        }
        return *_motor1;
    }

    Motor &getMotor2()
    {
        if (!_motor2)
        {
            // Remember to add constructor arguments here if needed by Motor
            _motor2 = std::make_unique<Motor>(motor2EnablePin, motor2StepPin, motor2DirectionPin);
        }
        return *_motor2;
    }
    NeoPixel &getNeoPixel()
    {
        if (!_neoPixel)
        {
            _neoPixel = std::make_unique<NeoPixel>();
        }
        return *_neoPixel;
    }

    SoundPlayer &getSoundPlayer()
    {
        if (!_soundPlayer)
        {
            // Remember to add constructor arguments here if needed by SoundPlayer
            _soundPlayer = std::make_unique<SoundPlayer>();
        }
        return *_soundPlayer;
    }

private:
    // Make the constructor private
    ApplicationDevices() = default;
    // Destructor can remain defaulted or be private as well if needed
    ~ApplicationDevices() = default;

    // --- Member variables remain the same ---
    std::unique_ptr<SoundButton> _button = nullptr;
    std::unique_ptr<GPSReceiver> _gpsReceiver = nullptr;
    std::unique_ptr<Motor> _motor1 = nullptr;
    std::unique_ptr<Motor> _motor2 = nullptr;
    std::unique_ptr<NeoPixel> _neoPixel = nullptr;
    std::unique_ptr<SoundPlayer> _soundPlayer = nullptr;

    // Note: No static instance variable needed here because we use a static local in getInstance()
};
