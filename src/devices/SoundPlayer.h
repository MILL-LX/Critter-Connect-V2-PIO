#pragma once

#include "FreeRTOS.h"
#include "task.h"

#include <SoftwareSerial.h>
#include <DFRobot_DF1201S.h>

const int defaultRxPin = 25;
const int defaultTxPin = 24;

class SoundPlayer
{
public:
    enum Sound
    {

        SPECIES_FROG = 1,
        SPECIES_PIGEON = 3,
        TEST_TONE = 5
    };

    void playSound(Sound sound)
    {
        int soundFileNum = static_cast<int>(sound);
        if (!_soundPlayer.isPlaying())
        {
            Serial.println("Starting to play sound file number " + String(soundFileNum));
            _soundPlayer.playFileNum(soundFileNum);
        }
        else
        {
            Serial.println("Skipping sound file number " + String(soundFileNum) + ".  A sound file is currently playing.");
        }
    }

    bool isPlaying()
    {
        return _soundPlayer.isPlaying();
    }

    SoundPlayer(int rxPin = defaultRxPin, int txPin = defaultTxPin) : _serial(rxPin, txPin) // Initialize the SoftwareSerial member
    {

        _serial.begin(115200);
        while (!_serial)
        {
            Serial.println("Waiting for serial");
            delay(500);
        }
        while (!_soundPlayer.begin(_serial))
        {
            Serial.println("SoundPlayer serial initialization failed, retrying...");
            delay(1000); // Retry if initialization fails
        }
        Serial.println("SoundPlayer Initialized!");        

        _soundPlayer.setVol(25);                  // Set volume 0-30 - set to max for deployment
        _soundPlayer.switchFunction(_soundPlayer.MUSIC); // Set MP3 module mode to music
        _soundPlayer.setPlayMode(_soundPlayer.SINGLE);   // Set single track play mode
        _soundPlayer.setPrompt(false);                   // Disable prompt sounds
    };

private:
    SoftwareSerial _serial;
    DFRobot_DF1201S _soundPlayer;
};
