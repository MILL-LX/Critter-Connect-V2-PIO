#include "actions/PeriodicAction.h"
#include "actions/MotorAction.h"
#include "devices/SoundPlayer.h"
#include "devices/SoundButton.h"
#include "devices/ApplicationDevices.h"

#include "StartupTest.h"

uint32_t evenOdd = 0;
void startupTest()
{

  NeoPixel &neoPixel = ApplicationDevices::getInstance().getNeoPixel();
  neoPixel.setColor(NeoPixel::StateColor::TEST);

  Serial.println("Waiting for sound button press...");
  SoundButton &button = ApplicationDevices::getInstance().getButton();
  while (!button.isPressed())
    ;
  while (button.isPressed())
    ;

  Serial.println("Starting Startup Tests...");
  neoPixel.setColor(NeoPixel::StateColor::OK);

  Motor &motor = (evenOdd++ % 2 == 0) ? ApplicationDevices::getInstance().getMotor1() : ApplicationDevices::getInstance().getMotor2();

  PeriodicAction<MotorAction> periodicMotorAction(15000, UINT32_MAX, 3, motor);
  periodicMotorAction.start();

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
  periodicMotorAction.stop();

  Serial.println("Waiting for test actions to stop...");
  vTaskDelay(pdMS_TO_TICKS(10000));
  neoPixel.setColor(NeoPixel::StateColor::OFF);

  Serial.println("Startup Tests completed.\n\n");
}
