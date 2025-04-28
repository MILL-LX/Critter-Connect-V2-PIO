#pragma once

#include <Arduino.h>

// These are the RX/TX pins on the Adafruit Feather Pico and they correspond to Serial1
// These definitions currently serve as documentation only.
const int defaultRXPin = 0;
const int defaultTXPin = 1;
const SerialUART defaultSerial = Serial1;

const int defaultBaudRate = 9600;

struct GPSData {
    bool dataReady;

    bool locationValid;
    float lat;
    float lon;

    bool dateValid;
    int year;
    int month;
    int day;

    bool timeValid;
    int hour;
    int minute;
    int second;
    int centisecond;
};

class GPSReceiver
{
public:
    GPSReceiver(SerialUART serial = defaultSerial, int baudRate = defaultBaudRate) : _serial(serial)
    {
        _serial.begin(baudRate);
        _data.dataReady = false;
    }

    GPSData readData();

private:
    SerialUART  _serial;
    GPSData     _data;

    void update();
};
