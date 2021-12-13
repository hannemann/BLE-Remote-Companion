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
    String response = "<!DOCTYPE html><html><head>\n";
    response += "<meta charset=\"utf-8\">\n";
    response += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
    response += "<meta name=\"color-scheme\" content=\"dark light\">\n";
    response += "<title>BLE-LIRC</title>\n";
    response += "<style>\n";
    response += Files::instance().readFile("/styles.css");
    response += "</style>\n";
    response += "<link rel=\"icon\" href=\"data:,\"></head><body class=\"ws-off\">\n";
    response += "<h1>BLE-LIRC</h1>\n";
    response += "<div class=\"buttons\">\n";
    response += "<button data-key=\"KEYCODE_POWER\">Power</button>\n";
    response += "<span></span>\n";
    response += "<button data-key=\"KEYCODE_MENU\">Menu</button>\n";
    response += "<button data-key=\"KEYCODE_ESCAPE\">Back</button>\n";
    response += "<button data-key=\"KEYCODE_DPAD_UP\">Up</button>\n";
    response += "<button data-key=\"KEYCODE_HOME\">Home</button>\n";
    response += "<button data-key=\"KEYCODE_DPAD_LEFT\">Left</button>\n";
    response += "<button data-key=\"KEYCODE_ENTER\">OK</button>\n";
    response += "<button data-key=\"KEYCODE_DPAD_RIGHT\">Right</button>\n";
    response += "<button data-key=\"KEYCODE_MEDIA_PLAY_PAUSE\">Play</button>\n";
    response += "<button data-key=\"KEYCODE_DPAD_DOWN\">Down</button>\n";
    response += "<button data-key=\"KEYCODE_MEDIA_STOP\">Stop</button>\n";
    response += "</div>\n";
    response += "<div class=\"config\">\n";
    response += "<label>Learn <input name=\"learn\" type=\"checkbox\"/></label>\n";
    response += "<button name=\"clear\">Clear Configuration</button>\n";
    response += "</div>\n";
    response += "<script>\n";
    response += Files::instance().readFile("/scripts.js");
    response += "</script></body></html>";
    server.send(200, "text/html", response);
}

void HTTPEvent::fourOFour(){
  server.send(404, "text/plain", "404 Not found");
}
