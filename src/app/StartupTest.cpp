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

  // Test Tone
  SoundPlayer &soundPlayer = ApplicationDevices::getInstance().getSoundPlayer();
  soundPlayer.playSound(SoundPlayer::Sound::TEST_TONE);
  while (soundPlayer.isPlaying())
    vTaskDelay(pdMS_TO_TICKS(100));

  // Frog Sound and Motor Action
  Motor &motor_frog = ApplicationDevices::getInstance().getMotor1();
  PeriodicAction<MotorAction> periodicMotorAction_frog(15000, UINT32_MAX, 3, motor_frog);
  periodicMotorAction_frog.start();

  soundPlayer.playSound(SoundPlayer::Sound::SPECIES_FROG);
  while (soundPlayer.isPlaying())
    vTaskDelay(pdMS_TO_TICKS(100));

  periodicMotorAction_frog.stop();
  while (periodicMotorAction_frog.isActive())
    vTaskDelay(pdMS_TO_TICKS(100));

  // Pigeon Sound and Motor Action
  Motor &motor_pigeon = ApplicationDevices::getInstance().getMotor2();
  PeriodicAction<MotorAction> periodicMotorAction_pigeon(15000, UINT32_MAX, 3, motor_pigeon);
  periodicMotorAction_frog.start();

  soundPlayer.playSound(SoundPlayer::Sound::SPECIES_PIGEON);
  while (soundPlayer.isPlaying())
    vTaskDelay(pdMS_TO_TICKS(100));

  periodicMotorAction_pigeon.stop();
  while (periodicMotorAction_pigeon.isActive())
    vTaskDelay(pdMS_TO_TICKS(100));

  neoPixel.setColor(NeoPixel::StateColor::OFF);

  Serial.println("Startup Tests completed.\n\n");
}
