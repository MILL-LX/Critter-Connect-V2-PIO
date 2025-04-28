#include <TinyGPSPlus.h>

#include "GPSReceiver.h"

TinyGPSPlus gps;

void GPSReceiver::update()
{
    // MOFIX - Lock the data structure

    // Read all available characters
    while (_serial.available() > 0)
    {
        char c = _serial.read();

        // When we have a fully formed reading, lock our data snapshot and parse the data into it.
        if (gps.encode(c))
        {
            if (_data.locationValid = gps.location.isValid())
            {
                _data.lat = gps.location.lat();
                _data.lon = gps.location.lng();
            }

            if (_data.dateValid = gps.date.isValid())
            {
                _data.year = gps.date.year();
                _data.month = gps.date.month();
                _data.day = gps.date.day();
            }

            if (_data.timeValid = gps.time.isValid())
            {
                _data.year = gps.time.hour();
                _data.minute = gps.time.minute();
                _data.second = gps.time.second();
                _data.centisecond = gps.time.centisecond();
            }

            _data.dataReady = (_data.locationValid || _data.dateValid || _data.timeValid);
        }
    }

    // MOFIX - Unlock the data structure
}

GPSData GPSReceiver::readData()
{
    // MOFIX - Lock the data structure
    return _data;
    // MOFIX - Unlock the data structure
}