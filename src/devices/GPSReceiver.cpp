#include <TinyGPSPlus.h>

#include "devices/GPSReceiver.h"

SerialUART *defaultSerial = &Serial1;
TinyGPSPlus gps;

void GPSReceiver::update()
{
    char reading[64];
    size_t bytesRead = 0;

    Serial.print("Checking for GPS data...");
    while (_serial->available() > 0)
    {
        char c = _serial->read();

        reading[bytesRead++] = c;
        Serial.print(".");

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
