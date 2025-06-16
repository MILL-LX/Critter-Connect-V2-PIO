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

  Serial.println("Starting Startup Tests...");
  neoPixel.setColor(NeoPixel::StateColor::OK);

  // Every 15 seconds move motor1 for 3 iterations. Run until explicitly stopped.
  Motor &motor1 = ApplicationDevices::getInstance().getMotor1();
  PeriodicAction<MotorAction> periodicMotor1Action(15000, UINT32_MAX, 3, motor1);
  periodicMotor1Action.start();

  // Every 15 seconds move motor1 for 3 iterations. Run until explicitly stopped.
  Motor &motor2 = ApplicationDevices::getInstance().getMotor2();
  PeriodicAction<MotorAction> periodicMotor2Action(15000, UINT32_MAX, 3, motor2);
  periodicMotor2Action.start();

  // Play the test tone sound. Will Play until finished.
  SoundPlayer &soundPlayer = ApplicationDevices::getInstance().getSoundPlayer();
  SoundPlayer::Sound soundList[] = {
      SoundPlayer::Sound::TEST_TONE,
      SoundPlayer::Sound::SPECIES_FROG,
      SoundPlayer::Sound::SPECIES_PIGEON};

  for (const auto &sound : soundList)
  {
    soundPlayer.playSound(sound);
    while (soundPlayer.isPlaying())
      vTaskDelay(pdMS_TO_TICKS(100));
  }

  // Stop all the actions
  Serial.println("Stopping test actions...");
  periodicMotor1Action.stop();
  periodicMotor2Action.stop();

  Serial.println("Waiting for test actions to stop...");
  vTaskDelay(pdMS_TO_TICKS(10000));
  neoPixel.setColor(NeoPixel::StateColor::OFF);

  Serial.println("Startup Tests completed.\n\n");
}
