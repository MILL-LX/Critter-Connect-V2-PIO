#include "FreeRTOS.h"
#include "task.h"

#include <TinyGPSPlus.h>

#include "devices/GPSReceiver.h"

TinyGPSPlus gps;

void GPSReceiver::update()
{
    while (_serial.available() > 0)
    {
        char c = _serial.read();

        if (gps.encode(c))
        {
            GPSData tempData;

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

            taskENTER_CRITICAL();
            _data = tempData;
            taskEXIT_CRITICAL();
        }
    }
}

GPSData GPSReceiver::readData()
{
    GPSData copy;

    taskENTER_CRITICAL();
    copy = _data;
    taskEXIT_CRITICAL();
    
    return copy;
}
