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
    response += "<meta name=\"color-scheme\" content=\"dark light\">";
    response += "<title>BLE-LIRC</title>";
    response += "<style>";
    response += ".ws-off button {opacity: .5; pointer-event: none;}";
    response += ".buttons {display: inline-grid; gap: 1em; place-items: center; grid-template-columns: repeat(3, 1fr);}";
    response += "button {width: 50px; aspect-ratio: 1;}";
    response += "</style>";
    response += "<link rel=\"icon\" href=\"data:,\"></head><body class=\"ws-off\">";
    response += "<h1>BLE-LIRC</h1>";
    response += "<div class=\"buttons\">";
    response += "<button data-key=\"KEYCODE_POWER\">Power</button>";
    response += "<span></span>";
    response += "<button data-key=\"KEYCODE_MENU\">Home</button>";
    response += "<button data-key=\"KEYCODE_ESCAPE\">Back</button>";
    response += "<button data-key=\"KEYCODE_DPAD_UP\">Up</button>";
    response += "<button data-key=\"KEYCODE_HOME\">Home</button>";
    response += "<button data-key=\"KEYCODE_DPAD_LEFT\">Left</button>";
    response += "<button data-key=\"KEYCODE_ENTER\">OK</button>";
    response += "<button data-key=\"KEYCODE_DPAD_RIGHT\">Right</button>";
    response += "<button data-key=\"KEYCODE_MEDIA_PLAY_PAUSE\">Play</button>";
    response += "<button data-key=\"KEYCODE_DPAD_DOWN\">Down</button>";
    response += "<button data-key=\"KEYCODE_MEDIA_STOP\">Stop</button>";
    response += "</div>";
    response += "<script>";
    response += "var ws = new WebSocket('ws://192.168.178.218:2339/jsonrpc');";
    response += "ws.onopen = e => document.body.classList.remove('ws-off');";
    response += "ws.onerror = e => document.body.classList.add('ws-off');";
    response += "ws.onclose = e => document.body.classList.add('ws-off');";
    response += "document.querySelectorAll('button').forEach(b => b.addEventListener('click', e => ws.send(JSON.stringify({method:e.target.dataset.key}))));";
    response += "</script></body></html>";
    server.send(200, "text/html", response);
}

void HTTPEvent::fourOFour(){
  server.send(404, "text/plain", "404 Not found");
}
