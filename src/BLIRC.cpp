#include <BLIRC.h>

BLIRC::BLIRC(){};

void BLIRC::setup()
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
    xTaskCreate(keyTaskServer, "server", 2048, NULL, 5, NULL);
    IRService::instance().init().run();
  }
  WebService::instance().run();
}

void BLIRC::loop()
{
  WebService::instance().loop();
  if (!WebService::captiveMode)
  {
    IRService::instance().loop();
  }
}
