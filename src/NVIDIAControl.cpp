#include <NVIDIAControl.h>

void setup() {
  Serial.begin(115200);
  
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("WEBSOCKET: [SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  wifi.addAP(ssid, password);

  while (wifi.run() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("Wifi Connected");
  xTaskCreate(keyTaskServer, "server", 20000, NULL, 5, NULL);

  WSEvent::initIPAddressFailures();
  WSEvent::webSocket.begin();
  WSEvent::webSocket.onEvent(WSEvent::webSocketEvent);
}

void loop() {
  WSEvent::webSocket.loop();
}
