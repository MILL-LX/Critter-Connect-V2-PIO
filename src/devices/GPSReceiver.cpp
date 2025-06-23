#include <TinyGPSPlus.h>

#include "devices/GPSReceiver.h"

SerialUART *defaultSerial = &Serial1;
TinyGPSPlus gps;

void GPSReceiver::update()
{
    // MODEBUG If using simulated data, override the real GPS data
    bool useSimulatedData = true; // Set to true to use mock data for testing
    if (useSimulatedData)
    {
        _data = simulatedGpsData(_data);
        return;
    }

    Serial.println("Checking for GPS Location data...");
    _data.locationValid = false;
    while (!_data.locationValid)
    {
        while (_serial->available() > 0)
        {
            char c = _serial->read();
            // MODEBUG Uncomment the next line to see raw GPS data
            // Serial.write(c);
            if (gps.encode(c))
            {
                Serial.print("\nGPS data successfully decoded\n");

                GPSData tempData;

                tempData.locationValid = gps.location.isValid();
                if (tempData.locationValid)
                {
                    Serial.println("Location Data VALID");
                    tempData.lat = gps.location.lat();
                    tempData.lon = gps.location.lng();
                    Serial.printf("  Latitude: %.6f\n", tempData.lat);
                    Serial.printf("  Longitude: %.6f\n", tempData.lon);
                }

                tempData.dateValid = gps.date.isValid();
                if (tempData.dateValid)
                {
                    Serial.println("Date Data VALID");
                    tempData.year = gps.date.year();
                    tempData.month = gps.date.month();
                    tempData.day = gps.date.day();
                    Serial.printf("  Date: %04d-%02d-%02d\n", tempData.year, tempData.month, tempData.day);
                }

                tempData.timeValid = gps.time.isValid();
                if (tempData.timeValid)
                {
                    Serial.println("Time Data VALID");
                    tempData.hour = gps.time.hour();
                    tempData.minute = gps.time.minute();
                    tempData.second = gps.time.second();
                    tempData.centisecond = gps.time.centisecond();
                    Serial.printf("  Time: %02d:%02d:%02d.%02d\n", tempData.hour, tempData.minute, tempData.second, tempData.centisecond);
                }

                tempData.dataReady = (tempData.locationValid || tempData.dateValid || tempData.timeValid);
                _data = tempData;
            }
        }
    }

    Serial.println("Found Location data, returning...");
}

GPSReceiver::GPSData GPSReceiver::readData()
{
    update();

    return _data;
}

void GPSReceiver::debugDumpGPSData()
{
    Serial.println("--- GPS Data Dump ---");
    Serial.printf("Data Ready: %s\n", _data.dataReady ? "Yes" : "No");

    Serial.printf("Location Valid: %s\n", _data.locationValid ? "Yes" : "No");
    if (_data.locationValid)
    {
        Serial.printf("  Latitude: %.6f\n", _data.lat); // Use %.6f for float precision
        Serial.printf("  Longitude: %.6f\n", _data.lon);
    }

    Serial.printf("Date Valid: %s\n", _data.dateValid ? "Yes" : "No");
    if (_data.dateValid)
    {
        Serial.printf("  Date: %04d-%02d-%02d\n", _data.year, _data.month, _data.day); // Format YYYY-MM-DD
    }

    Serial.printf("Time Valid: %s\n", _data.timeValid ? "Yes" : "No");
    if (_data.timeValid)
    {
        Serial.printf("  Time: %02d:%02d:%02d.%02d\n", _data.hour, _data.minute, _data.second, _data.centisecond); // Format HH:MM:SS.CS
    }
    Serial.println("--- End GPS Data Dump ---");
}

GPSReceiver::GPSData GPSReceiver::simulatedGpsData(GPSReceiver::GPSData gpsData)
{
    GPSReceiver::GPSData mockGgpsData = gpsData;
    if (millis() < 60000) // spend a minute in a non-species zone
    {
        Serial.println("Simulating location in non-species zone for testing purposes.");
        mockGgpsData.lat = 1.0;
        mockGgpsData.lon = 1.0;
    }
    else if (millis() < 200000) // spend a little over 2 minutes in a species 1 zone
    {
        Serial.println("Simulating location in species zone 1 for testing purposes.");
        mockGgpsData.lat = 32.6585412143;
        mockGgpsData.lon = -16.8685332416;
    }
    else if (millis() < 260000) // spend a minute in a non-species zone
    {
        Serial.println("Simulating location in non-species zone for testing purposes.");
        mockGgpsData.lat = 1.0;
        mockGgpsData.lon = 1.0;
    }
    else if (millis() < 400000) // spend a little over 2 minutes in a species 2 zone
    {
        Serial.println("Simulating location in species zone 2 for testing purposes.");
        mockGgpsData.lat = 32.662040384700205;
        mockGgpsData.lon = -16.868402420468072;
    }
    else if (millis() < 460000) // spend a minute in a non-species zone
    {
        Serial.println("Simulating location in non-species zone for testing purposes.");
        mockGgpsData.lat = 1.0;
        mockGgpsData.lon = 1.0;
    }
    else if (millis() < 600000) // spend a little over 2 minutes in a species 1 zone
    {
        Serial.println("Simulating location in species zone 1 for testing purposes.");
        mockGgpsData.lat = 32.6585412143;
        mockGgpsData.lon = -16.8685332416;
    }
    else
    {
        Serial.println("Simulation complete, using real GPS data.");
        return gpsData; // Return original data if simulation is complete
    }

    mockGgpsData.dataReady = true; // Simulate data being ready
    mockGgpsData.locationValid = true;
    return mockGgpsData;
}