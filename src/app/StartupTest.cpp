#include "actions/PeriodicAction.h"
#include "actions/MotorAction.h"
#include "devices/SoundPlayer.h"
#include "devices/ApplicationDevices.h"

#include "StartupTest.h"

void startupTest()
{
  Serial.println("Starting Startup Tests...");

  // Set the NeoPixel to the warning color for the duration of the test
  NeoPixel &neoPixel = ApplicationDevices::getInstance().getNeoPixel();
  neoPixel.setColor(NeoPixel::StateColor::TEST);

  // Every 10 seconds move motor1 for 10 seconds. Run until explicitly stopped.
  Motor &motor1 = ApplicationDevices::getInstance().getMotor1();
  PeriodicAction<MotorAction> periodicMotor1Action(10000UL, UINT32_MAX, 10000UL, motor1);
  periodicMotor1Action.start();

  // Every 10 seconds move motor2 for 10 seconds. Run until explicitly stopped.
  Motor &motor2 = ApplicationDevices::getInstance().getMotor2();
  PeriodicAction<MotorAction> periodicMotor2Action(10000UL, UINT32_MAX, 10000UL, motor2);
  periodicMotor2Action.start();

  // Play the test tone sound. Will Play until finished.
  SoundPlayer &soundPlayer = ApplicationDevices::getInstance().getSoundPlayer();
  soundPlayer.playSound(SoundPlayer::Sound::TEST_TONE);

  // Let the tests run for 8 seconds
  vTaskDelay(pdMS_TO_TICKS(10000));

  // Stop all the actions
  Serial.println("Stopping test actions...");
  periodicMotor1Action.stop();
  periodicMotor2Action.stop();

  neoPixel.setColor(NeoPixel::StateColor::OFF);
  // Wait for the actions to stop
  // MOFIX - maybe add a boolean to say whether an action is still running instead of using a delay
  Serial.println("Waiting for test actions to stop...");
  vTaskDelay(pdMS_TO_TICKS(10000));

  Serial.println("Startup Tests completed.\n\n");
}
