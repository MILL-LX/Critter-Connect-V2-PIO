#pragma once

#include "FreeRTOS.h"
#include "task.h"

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
    enum Direction
    {
        FORWARD,
        REVERSE
    };

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
        disable();
        Serial.println("Motor driver initially disabled.");

        // Configure AccelStepper settings
        _stepper->setMaxSpeed(maxSpeed);         // Max speed in steps per second
        _stepper->setAcceleration(acceleration); // Acceleration in steps per second^2
        _stepper->setCurrentPosition(0);         // Set the current position to 0

        Serial.print("Initial position: ");
        Serial.println(_stepper->currentPosition());
        Serial.print("Target steps: +/- ");
        Serial.println(stepsInFullRange);
    };

    void enable()
    {
        Serial.println("Enabling motor driver.");
        digitalWrite(_enablePin, LOW);
        delay(5);
    }

    void disable()
    {
        Serial.println("Disabling motor driver.");
        digitalWrite(_enablePin, HIGH);
        delay(5);
    }

    void moveFullRangeInDirection(Direction direction) {
        enable();

        long motorPositiion = direction == FORWARD ? stepsInFullRange : 0;
        _stepper->moveTo(motorPositiion);

        while(true) {
            if (_stepper->distanceToGo() == 0) {
                // Check if the stepper is truly done by checking current position
                // This is a more robust check after run() has potentially completed the move
                if (_stepper->currentPosition() == motorPositiion) {
                   Serial.println("Reached motor position target.");
                   break;
                }
             }

             // Move the motor
             _stepper->run();

            // Don't monopolize the cpu
            vTaskDelay(1);
        }

        disable();
    }

private:
    int _enablePin;
    int _stepPin;
    int _directionPin;

    AccelStepper *_stepper;
};
