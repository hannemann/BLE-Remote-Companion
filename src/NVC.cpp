#include <NVC.h>

NVC::NVC() {};

void NVC::setup() {
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
  WebService::instance().run();
}

void NVC::loop() {
  WebService::instance().loop();
}

const char* NVC::getSSID() {
  return ssid;
}

const char* NVC::getPSK() {
  return psk;
}
