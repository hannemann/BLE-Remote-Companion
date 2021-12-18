#include "HTTPEvent.h"

void HTTPEvent::init() {
    Serial.println("Init HTTP server...");
    WebServer server(port);
}

void HTTPEvent::run() {
    server.on("/", home);
    server.on("/learn", learn);
    server.onNotFound(fourOFour);
    server.begin();
    Serial.printf("Webserver started listening on port %d\n", port);
}

void HTTPEvent::loop() {
    server.handleClient();
}

void HTTPEvent::home() {
    String response = header();
    response += nav();
    response += "<h1>BLE-LIRC</h1>\n";
    response += buttons();
    response += footer();
    instance().server.send(200, "text/html", response);
}

void HTTPEvent::learn() {
    String response = header();
    response += nav();
    response += "<h1>BLE-LIRC</h1>\n";
    response += buttons();
    response += "<div class=\"config\">\n";
    response += "<label>Learn <input name=\"learn\" type=\"checkbox\"/></label>\n";
    response += "<button name=\"clear\">Clear Configuration</button>\n";
    response += "</div>\n";
    response += footer();
    instance().server.send(200, "text/html", response);
}

void HTTPEvent::fourOFour(){
  instance().server.send(404, "text/plain", "404 Not found");
}

String HTTPEvent::header() {
    String header = "<!DOCTYPE html><html><head>\n";
    header += "<meta charset=\"utf-8\">\n";
    header += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
    header += "<meta name=\"color-scheme\" content=\"dark light\">\n";
    header += "<title>BLE-LIRC</title>\n";
    header += "<style>\n";
    header += Files::css;
    header += "</style>\n";
    header += "<link rel=\"icon\" href=\"data:,\"></head>\n";
    header += "<body class=\"ws-off\">\n";
    return header;
}

String HTTPEvent::footer() {
    String footer = "<script>\n";
    footer += Files::scripts;
    footer += "</script></body></html>";
    return footer;
}

String HTTPEvent::nav() {
    String nav = "<nav>\n";
    nav += "<a href=\"/\">Home</a>\n";
    nav += "<a href=\"/learn\">Learn</a>\n";
    nav += "</nav>\n";
    return nav;
}

String HTTPEvent::buttons() {
    String buttons = "<div class=\"buttons\">\n";
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_MINIMAL);
    HID_USAGE_KEY layout[size];
    Serial.println(size);
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_MINIMAL, layout);
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        if (layout[i].type == TYPE_NONE) {
            buttons += "<span></span>\n";
        } else {
            buttons += "<button data-key=\"";
            char bufIdx[3];
            itoa(i, bufIdx, 10);
            buttons += bufIdx;
            buttons += "\" data-layout=\"";
            char bufLayout[3];
            itoa(LAYOUT_REMOTE_MINIMAL, bufLayout, 10);
            buttons += bufLayout;
            buttons += "\">";
            buttons += layout[i].label;
            buttons += "</button>\n";
        }
    }
    buttons += "</div>\n";
    return buttons;
}
