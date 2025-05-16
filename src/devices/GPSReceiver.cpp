#include <TinyGPSPlus.h>

#include "devices/GPSReceiver.h"

SerialUART *defaultSerial = &Serial1;
TinyGPSPlus gps;

void GPSReceiver::update()
{
    char reading[256];
    size_t bytesRead = 0;

    Serial.println("Checking for GPS data...");
    while (_serial->available() > 0)
    {
        char c = _serial->read();

        reading[bytesRead++] = c;
        //Serial.print(".");
        Serial.write(c);



        if (gps.encode(c))
        {
            GPSData tempData;

            Serial.print("\nGPS data successfully decoded\n");
            Serial.write((const uint8_t *)reading, bytesRead);
            bytesRead = 0;

            tempData.locationValid = gps.location.isValid();
            if (tempData.locationValid)
            {
                tempData.lat = gps.location.lat();
                tempData.lon = gps.location.lng();
            }

            tempData.dateValid = gps.date.isValid();
            if (tempData.dateValid)
            {
                tempData.year = gps.date.year();
                tempData.month = gps.date.month();
                tempData.day = gps.date.day();
            }

            tempData.timeValid = gps.time.isValid();
            if (tempData.timeValid)
            {
                tempData.hour = gps.time.hour();
                tempData.minute = gps.time.minute();
                tempData.second = gps.time.second();
                tempData.centisecond = gps.time.centisecond();
            }

            tempData.dataReady = (tempData.locationValid || tempData.dateValid || tempData.timeValid);
            _data = tempData;

            Serial.println("\nGPS Data updated, returning...\n");
            return;
        }
    }

    Serial.println("\nProcessed all available GPS Data, returning without updating...\n");
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

