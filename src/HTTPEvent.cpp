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
    response += numbers();
    response += dpad();
    response += media();
    response += colors();
    response += footer();
    instance().server.send(200, "text/html", response);
}

void HTTPEvent::learn() {
    String response = header();
    response += nav();
    response += "<h1>BLE-LIRC</h1>\n";
    response += numbers();
    response += "<div class=\"config\">\n";
    response += "<label>Learn <input name=\"learn\" type=\"checkbox\"/></label>\n";
    response += "<button name=\"clear\">Clear Configuration</button>\n";
    response += "</div>\n";
    response += footer();
    instance().server.send(200, "text/html", response);
}

void HTTPEvent::fourOFour() {
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

String HTTPEvent::numbers() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_NUMBERS);
    HID_USAGE_KEY layout[size];
    Serial.println(size);
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_NUMBERS, layout);
    String buttons = "<div class=\"buttons numbers\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::dpad() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_DPAD);
    HID_USAGE_KEY layout[size];
    Serial.println(size);
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_DPAD, layout);
    String buttons = "<div class=\"buttons dpad\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::media() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_MEDIA);
    HID_USAGE_KEY layout[size];
    Serial.println(size);
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_MEDIA, layout);
    String buttons = "<div class=\"buttons media\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::colors() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_COLORS);
    HID_USAGE_KEY layout[size];
    Serial.println(size);
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_COLORS, layout);
    String buttons = "<div class=\"buttons colors\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::button(HID_USAGE_KEY key) {
    String button = "";
    if (key.type == TYPE_NONE) {
        button += "<span></span>\n";
    } else {
        button += "<button data-key=\"";
        char bufIdx[3];
        itoa(key.index, bufIdx, 10);
        button += bufIdx;
        button += "\" data-type=\"";
        char bufL[3];
        itoa(key.type, bufL, 10);
        button += bufL;
        button += "\">";
        button += key.label;
        button += "</button>\n";
    }
    return button;
}
