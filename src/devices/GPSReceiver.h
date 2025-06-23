#pragma once

#include <Arduino.h> // Includes HardwareSerial.h or similar which defines SerialUART

extern SerialUART *defaultSerial;
const int defaultBaudRate = 9600;

class GPSReceiver
{
public:
    struct GPSData
    {
        bool dataReady = false; // Initialize members directly if possible (C++11)

        bool locationValid = false;
        float lat = 0.0f;
        float lon = 0.0f;

        bool dateValid = false;
        int year = 0;
        int month = 0;
        int day = 0;

        bool timeValid = false;
        int hour = 0;
        int minute = 0;
        int second = 0;
        int centisecond = 0;
    };

    // Constructor takes a pointer to SerialUART
    // Note: Removed 'const' as methods like begin(), available(), read() are non-const
    GPSReceiver(SerialUART *serial = defaultSerial, int baudRate = defaultBaudRate)
        : _serial(serial), // Initialize the pointer member
          _baudRate(baudRate)
    // _data is default initialized by its member initializers above
    {
        // Good practice: Check if the pointer is valid before using it
        if (_serial)
        {
            _serial->begin(_baudRate); // Use '->' for pointers
        }
        else
        {
            // Handle error - perhaps log or enter a safe state
            Serial.println("ERROR: GPSReceiver initialized with null serial port!");
        }
    }

    GPSData readData();
    void debugDumpGPSData();

private:
    SerialUART *_serial; // Member is a pointer
    int _baudRate;
    GPSData _data;

    void update();

    GPSReceiver::GPSData mockGpsData(GPSReceiver::GPSData gpsData, bool useMockData = false);
};
