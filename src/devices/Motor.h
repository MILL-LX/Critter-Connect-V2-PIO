#pragma once

#include <AccelStepper.h>

const int defaultEnablePin = 2;
const int defaultStepPin = 19;
const int defaultDirectionPin = 13;

const long stepsInFullRange = 14000; 
const long maxSpeed = 20000;
const long acceleration = 10000;

class Motor
{
public:
    Motor(int enablePin = defaultEnablePin, int stepPin = defaultStepPin, int directionPin = defaultDirectionPin) : _enablePin(enablePin),
                                                                                                                    _stepPin(stepPin),
                                                                                                                    _directionPin(directionPin),
                                                                                                                    _stepper(new AccelStepper(AccelStepper::DRIVER, _stepPin, _directionPin))
    {
        // Set the pin modes for the control pins
        pinMode(_enablePin, OUTPUT);
        pinMode(_stepPin, OUTPUT);
        pinMode(_directionPin, OUTPUT);
  

        // Disable the TMC2209 driver initially
        digitalWrite(_enablePin, HIGH);
        Serial.println("Motor driver initially disabled.");

        // Configure AccelStepper settings
        _stepper->setMaxSpeed(maxSpeed);     // Max speed in steps per second
        _stepper->setAcceleration(acceleration); // Acceleration in steps per second^2
        _stepper->setCurrentPosition(0);    // Set the current position to 0

        Serial.print("Initial position: ");
        Serial.println(_stepper->currentPosition());
        Serial.print("Target steps: +/- ");
        Serial.println(stepsInFullRange);
    };

private:
    int _enablePin;
    int _stepPin;
    int _directionPin;

    AccelStepper *_stepper;
};
