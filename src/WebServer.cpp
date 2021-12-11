#include "NVC.h"
#include "WebServer.h"

WebServer::WebServer() {}

void WebServer::init() {
    WiFiMulti wifi;
    Serial.print("Connecting to WIFI");
    wifi.addAP(NVC::instance().getSSID(), NVC::instance().getPSK());
    while (wifi.run() != WL_CONNECTED) {
        Serial.printf(".");
        Serial.flush();
        delay(1000);
    }
    Serial.printf("\nConnected\n");
    WSEvent::initIPAddressFailures();
}

void WebServer::run() {
    WSEvent::webSocket.begin();
    WSEvent::webSocket.onEvent(WSEvent::webSocketEvent);
}

void WebServer::loop() {
    WSEvent::webSocket.loop();
}