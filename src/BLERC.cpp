#include "BLERC.h"

BLERC::BLERC(){};

void BLERC::setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.print("[SETUP] BOOT WAIT");
  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.print(".");
    Serial.flush();
    delay(1000);
  }
  Serial.println("");
  WebService::instance().init();
  if (!WebService::captiveMode)
  {
    delay(1000);
    Bluetooth::create();
    xTaskCreate(keyTaskServer, "server", 3062, NULL, 5, NULL);
    IRService::instance().init().run();
  }
  WebService::instance().run();
}

void BLERC::loop()
{
  WebService::instance().loop();
  if (!WebService::captiveMode)
  {
    IRService::instance().loop();
  }
}
