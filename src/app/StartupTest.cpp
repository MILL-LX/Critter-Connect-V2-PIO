#include "actions/PeriodicAction.h"
#include "actions/MotorAction.h"
#include "devices/SoundPlayer.h"
#include "devices/NeoPixel.h"

#include "StartupTest.h"

void startupTest() {
    Serial.println("Starting Startup Tests...");
  
    // Set the NeoPixel to the warning color for the duration of the test
    NeoPixel neoPixel;
    neoPixel.setColor(NeoPixel::StateColor::TEST);
  
    // Every 10 seconds move the motor for 10 seconds. Run until explicitly stopped.
    PeriodicAction<MotorAction> periodicMotorAction(10000UL, UINT32_MAX, 10000UL);
    periodicMotorAction.start();

    // Play the test tone sound. Will Play until finished.
    SoundPlayer soundPlayer;
    soundPlayer.playSound(SoundPlayer::Sound::TEST_TONE);
  
    // Let the tests run for 9 seconds
    vTaskDelay(pdMS_TO_TICKS(9000));
  
    // Stop all the actions
    Serial.println("Stopping test actions...");
    periodicMotorAction.stop();
  
    // Wait for the actions to stop
    // MOFIX - maybe add a boolean to say whether an action is still running instead of using a delay
    Serial.println("Waiting for test actions to stop...");
    vTaskDelay(pdMS_TO_TICKS(1000));
  
    Serial.println("Startup Tests completed.");
  }
