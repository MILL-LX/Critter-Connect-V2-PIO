#pragma once

#include <SoftwareSerial.h>
#include <DFRobot_DF1201S.h>

const int defaultRxPin = 25;
const int defaultTxPin = 24;

class SoundPlayer
{
public:
    enum Sound
    {
        SPECIES_1,
        SPECIES_2,
        TEST_TONE
    };

    void playSound(Sound sound)
    {
        String soundFile = _soundFiles[sound];

        if (!_soundPlayer.isPlaying()) {
           Serial.println("Starting to play " + soundFile);
            _soundPlayer.playSpecFile(soundFile);        
        } else {
            Serial.println("Skipping " + soundFile + ".  A sound file is currently playing.");          
        }
    }


    SoundPlayer(int rxPin = defaultRxPin, int txPin = defaultTxPin):
    _serial(rxPin, txPin), // Initialize the SoftwareSerial member
    _soundPlayer()         // Initialize the DFRobot member
    {

        _serial.begin(115200);
        while(!Serial)
        {
            Serial.println("Waiting for serial");
            delay(500);
        }
        while (!_soundPlayer.begin(_serial))
        {
            Serial.println("SoundPlayer serial initialization failed. Check the wire connection!");
            delay(1000); // Retry if initialization fails
        }

        _soundPlayer.setVol(/*30*/ 5);                    // MOFIX Set volume 0-30 - set to max for deployment
        _soundPlayer.switchFunction(_soundPlayer.MUSIC); // Set MP3 module mode to music
        _soundPlayer.setPlayMode(_soundPlayer.SINGLE);   // Set single track play mode
        _soundPlayer.setPrompt(false);                    // Disable prompt sounds
    };

private:
SoftwareSerial _serial;
DFRobot_DF1201S _soundPlayer;
    String _soundFiles[3] = {"species-1.mp3", "species-2.mp3", "test-tone.mp3"};

};
