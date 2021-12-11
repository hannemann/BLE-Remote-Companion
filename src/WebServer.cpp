#include "NVC.h"
#include "WebServer.h"

WebServer::WebServer() {}

void WebServer::init() {
    WiFiMulti wifi;
    Serial.print("Connecting to WIFI");
    wifi.addAP(NVC::instance().getSSID(), NVC::instance().getPSK());
    while (wifi.run() != WL_CONNECTED) {
        Serial.write(".");
        Serial.flush();
        delay(1000);
    }
    Serial.printf("\nConnected\n");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    WSEvent::initIPAddressFailures();
    HTTPEvent::instance().init();
}

void WebServer::run() {
    WSEvent::webSocket.begin();
    WSEvent::webSocket.onEvent(WSEvent::webSocketEvent);
    HTTPEvent::instance().run();
}

void WebServer::loop() {
    WSEvent::webSocket.loop();
    HTTPEvent::instance().loop();
}