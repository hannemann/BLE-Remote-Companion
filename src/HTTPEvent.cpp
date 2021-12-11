#include "HTTPEvent.h"
#include "BLIRC.h"

WebServer server;

void HTTPEvent::init() {
    Serial.println("Init HTTP server...");
    WiFiServer server(port);
}

void HTTPEvent::run() {
    server.on("/", home);
    server.onNotFound(fourOFour);
    server.begin();
    Serial.printf("Webserver started listening on port %d\n", port);
}

void HTTPEvent::loop() {
    server.handleClient();
}

void HTTPEvent::home() {
    String response = "<!DOCTYPE html><html><head>";
    response += "<meta charset=\"utf-8\">";
    response += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    response += "<title>BLE-LIRC</title>";
    response += "<link rel=\"icon\" href=\"data:,\"></head><body>";
    response += "<h1>BLE-LIRC</h1>";
    response += "</body></html>";
    server.send(200, "text/html", response);
}

void HTTPEvent::fourOFour(){
  server.send(404, "text/plain", "404 Not found");
}
