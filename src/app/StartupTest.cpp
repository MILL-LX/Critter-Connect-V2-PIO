#include "actions/PeriodicAction.h"
#include "actions/MotorAction.h"
#include "devices/SoundPlayer.h"
#include "devices/SoundButton.h"
#include "devices/ApplicationDevices.h"

#include "StartupTest.h"

void startupTest()
{

  NeoPixel &neoPixel = ApplicationDevices::getInstance().getNeoPixel();
  neoPixel.setColor(NeoPixel::StateColor::TEST);

  Serial.println("Waiting for button press...");
  SoundButton &button = ApplicationDevices::getInstance().getButton();
  while (!button.isPressed())
    ;
   while (button.isPressed())
    ;


  // while (true)
  // {
  //   if (button.isPressed())
  //     Serial.println("Button PRESSED");
  //   else
  //     Serial.println("Button NOT PRESSED)");
  //   vTaskDelay(pdMS_TO_TICKS(10));
  // }

  Serial.println("Starting Startup Tests...");
  neoPixel.setColor(NeoPixel::StateColor::OK);

  // Every 10 seconds move motor1 for 3 seconds. Run until explicitly stopped.
  Motor &motor1 = ApplicationDevices::getInstance().getMotor1();
  PeriodicAction<MotorAction> periodicMotor1Action(10000, UINT32_MAX, 3000UL, motor1);
  periodicMotor1Action.start();

  // Every 10 seconds move motor2 for 3 seconds. Run until explicitly stopped.
  Motor &motor2 = ApplicationDevices::getInstance().getMotor2();
  PeriodicAction<MotorAction> periodicMotor2Action(10000, UINT32_MAX, 3000UL, motor2);
  periodicMotor2Action.start();

  // Play the test tone sound. Will Play until finished.
  SoundPlayer &soundPlayer = ApplicationDevices::getInstance().getSoundPlayer();
  SoundPlayer::Sound soundList[] = {
      SoundPlayer::Sound::TEST_TONE,
      SoundPlayer::Sound::SPECIES_FROG,
      SoundPlayer::Sound::SPECIES_PIGEON

  };

  for (const auto &sound : soundList)
  {
    soundPlayer.playSound(sound);
    while (soundPlayer.isPlaying())
    {
      // Wait for the sound to finish playing
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }

  // Let the tests run for 10 seconds
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
