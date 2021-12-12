#include <BLIRC.h>

BLIRC::BLIRC() {};

void BLIRC::setup() {
  Serial.begin(115200);
  delay(500);
  Serial.print("[SETUP] BOOT WAIT");
  for (uint8_t t=4;t>0;t--) {
      Serial.print(".");
      Serial.flush();
      delay(1000);
  }
  Serial.println("");
  WebService::instance().init();
  xTaskCreate(keyTaskServer, "server", 20000, NULL, 5, NULL);
  IRService::instance().init().run();
  WebService::instance().run();
}

void BLIRC::loop() {
  WebService::instance().loop();
  IRService::instance().loop();
}

const char* BLIRC::getSSID() {
  return ssid;
}

const char* BLIRC::getPSK() {
  return psk;
}
