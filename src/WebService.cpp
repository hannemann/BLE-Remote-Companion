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
    WSEvent::instance().init();
    HTTPEvent::instance().init();
}

void WebService::run() {
    WSEvent::instance().run();
    HTTPEvent::instance().run();
}

void WebService::loop() {
    WSEvent::instance().loop();
    HTTPEvent::instance().loop();
}