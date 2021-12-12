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
    response += ".ws-off button {opacity: .5; pointer-event: none;}\n";
    response += ".buttons {display: inline-grid; gap: 1em; place-items: center; grid-template-columns: repeat(3, 1fr);}\n";
    response += "button {width: 50px; aspect-ratio: 1;}\n";
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
    response += "<label>Learn <input name=\"learn\" type=\"checkbox\"/></label>\n";
    response += "</div>\n";
    response += "<script>\n";
    response += "var ws = new WebSocket('ws://192.168.178.218:2339/jsonrpc');\n";
    response += "ws.onopen = e => document.body.classList.remove('ws-off');\n";
    response += "ws.onerror = e => document.body.classList.add('ws-off');\n";
    response += "ws.onclose = e => document.body.classList.add('ws-off');\n";
    response += "document.querySelectorAll('button').forEach(b => {;\n";
    response += "    b.addEventListener('pointerdown', e => {\n";
    response += "        if (document.querySelector('[name=\"learn\"]:checked')) return;\n";
    response += "        ws.send(JSON.stringify({method:\"keydown\",params:{key:e.target.dataset.key}}))\n";
    response += "    });\n";
    response += "});\n";
    response += "document.querySelectorAll('button').forEach(b => {;\n";
    response += "    b.addEventListener('pointerup', e => {\n";
    response += "        if (document.querySelector('[name=\"learn\"]:checked')) {\n";
    response += "           ws.send(JSON.stringify({method:\"learn\",params:{key:e.target.dataset.key}}))\n";
    response += "        } else {\n";
    response += "           ws.send(JSON.stringify({method:\"keyup\",params:{key:e.target.dataset.key}}))\n";
    response += "        }\n";
    response += "    });\n";
    response += "});\n";
    response += "</script></body></html>";
    server.send(200, "text/html", response);
}

void HTTPEvent::fourOFour(){
  server.send(404, "text/plain", "404 Not found");
}
