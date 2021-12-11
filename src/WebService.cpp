#include "BLIRC.h"
#include "WebService.h"

WebService::WebService() {}

void WebService::init() {
    Serial.print("Connecting to WIFI");
    WiFi.begin(BLIRC::instance().getSSID(), BLIRC::instance().getPSK());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.printf("\nConnected! IP address: ");
    Serial.println(WiFi.localIP());
    WSEvent::initIPAddressFailures();
    HTTPEvent::instance().init();
}

void WebService::run() {
    WSEvent::webSocket.begin();
    WSEvent::webSocket.onEvent(WSEvent::webSocketEvent);
    HTTPEvent::instance().run();
}

void WebService::loop() {
    WSEvent::webSocket.loop();
    HTTPEvent::instance().loop();
}