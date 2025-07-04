#include "actions/PeriodicAction.h"
#include "actions/MotorAction.h"
#include "actions/VibratingMotorAction.h"
#include "devices/SoundPlayer.h"
#include "devices/SoundButton.h"
#include "devices/ApplicationDevices.h"

#include "StartupTest.h"

void startupTest()
{

  NeoPixel &neoPixel = ApplicationDevices::getInstance().getNeoPixel();
  neoPixel.setColor(NeoPixel::StateColor::TEST);

  Serial.println("Waiting for sound button press...");
  SoundButton &button = ApplicationDevices::getInstance().getSoundButton();
  while (!button.isPressed())
    ;

  Serial.println("Starting Startup Tests...");
  neoPixel.setColor(NeoPixel::StateColor::OK);

  SoundPlayer &soundPlayer = ApplicationDevices::getInstance().getSoundPlayer();

  // MODEBUG Disable this annoying sound. The other sounds prove the speaker works.
  // Test Tone
  // soundPlayer.playSound(SoundPlayer::Sound::TEST_TONE);
  // while (soundPlayer.isPlaying())
  //   vTaskDelay(pdMS_TO_TICKS(100));

  // Vibrating Motor Test
  VibratingMotor &vibratingMotor = ApplicationDevices::getInstance().getVibratingMotor();
  PeriodicAction<VibratingMotorAction> periodicVibratingMotorAction(1000, 10, 500, 500, vibratingMotor);
  periodicVibratingMotorAction.start();
  while (periodicVibratingMotorAction.isActive())
    vTaskDelay(pdMS_TO_TICKS(100));

  // Frog Sound and Motor Action
  Serial.println("Starting Frog Sound and Motor Action...");
  Motor &motor_frog = ApplicationDevices::getInstance().getMotorFrog();
  PeriodicAction<MotorAction> periodicMotorAction_frog(10000, UINT32_MAX, 2, motor_frog);
  periodicMotorAction_frog.start();

  soundPlayer.playSound(SoundPlayer::Sound::SPECIES_FROG);
  while (soundPlayer.isPlaying())
    vTaskDelay(pdMS_TO_TICKS(100));

  periodicMotorAction_frog.stop();
  while (periodicMotorAction_frog.isActive())
    vTaskDelay(pdMS_TO_TICKS(100));

  // Pause between actions
  vTaskDelay(pdMS_TO_TICKS(2000));

  // Pigeon Sound and Motor Action
  Serial.println("Starting Pigeon Sound and Motor Action...");
  Motor &motor_pigeon = ApplicationDevices::getInstance().getMotorPigeon();
  PeriodicAction<MotorAction> periodicMotorAction_pigeon(10000, UINT32_MAX, 2, motor_pigeon);
  periodicMotorAction_pigeon.start();

  soundPlayer.playSound(SoundPlayer::Sound::SPECIES_PIGEON);
  while (soundPlayer.isPlaying())
    vTaskDelay(pdMS_TO_TICKS(100));

  periodicMotorAction_pigeon.stop();
  while (periodicMotorAction_pigeon.isActive())
    vTaskDelay(pdMS_TO_TICKS(100));

  neoPixel.setColor(NeoPixel::StateColor::OFF);

  Serial.println("Startup Tests completed.\n\n");
}
