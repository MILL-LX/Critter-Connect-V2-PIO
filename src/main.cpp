
#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "StartupTest.h"

#include "devices/SoundPlayer.h"

void setup()
{
  delay(2000); // let USB mount
  Serial.begin(115200);
  delay(2000); // let Serial Stabilize

  Serial.println("Setup complete.");
}

void loop()
{
  //startupTest();
  SoundPlayer sp;

  while(true) {
    sp.playSound(SoundPlayer::Sound::TEST_TONE);
    Serial.println("Main loop doing nothing...");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
