#include "HTTPEvent.h"

void HTTPEvent::init() {
    Serial.println("Init HTTP server...");
    WebServer server(port);
}

void HTTPEvent::run() {
    server.on("/", home);
    server.on("/keyboard", keyboard);
    server.on("/learn", learn);
    server.onNotFound(fourOFour);
    server.begin();
    Serial.printf("Webserver started listening on port %d\n", port);
}

void HTTPEvent::loop() {
    server.handleClient();
}

void HTTPEvent::home() {
    Serial.printf("GET /\n");
    String response = header();
    response += nav();
    response += "<main><section class=\"remote\">\n";
    response += numbers();
    response += functional();
    response += dpad();
    response += media();
    response += colors();
    response += "</section></main>\n";
    response += footer();
    instance().server.send(200, "text/html", response);
    // Serial.println(ESP.getFreeHeap());
}

void HTTPEvent::learn() {
    Serial.printf("GET /learn\n");
    String response = header();
    response += nav();
    response += "<main><section class=\"remote\">\n";
    response += numbers();
    response += functional();
    response += dpad();
    response += media();
    response += colors();
    response += "</section></main>\n";
    response += "<div class=\"config\">\n";
    response += "<label>Learn <input name=\"learn\" type=\"checkbox\"/></label>\n";
    response += "<button name=\"clear\">Clear Configuration</button>\n";
    response += "</div>\n";
    response += footer();
    instance().server.send(200, "text/html", response);
}

void HTTPEvent::keyboard() {
    Serial.printf("GET /keyboard\n");
    String response = header();
    response += nav();
    response += "<main><section class=\"remote\">\n";
    response += functional();
    response += dpad();
    response += "</section>\n";
    response += "<section class=\"keyboard\">\n";
    response += keyboardRow(LAYOUT_KEYBOARD_NUMBERS);
    response += keyboardRow(LAYOUT_KEYBOARD_ROW1);
    response += keyboardRow(LAYOUT_KEYBOARD_ROW2);
    response += keyboardRow(LAYOUT_KEYBOARD_ROW3);
    response += keyboardRow(LAYOUT_KEYBOARD_ROW4);
    response += "</section></main>\n";
    response += footer();
    instance().server.send(200, "text/html", response);
    // Serial.println(ESP.getFreeHeap());
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
    nav += "<a href=\"/\">&#x2302;</a>\n";
    nav += "<a href=\"/keyboard\">&#x2328;</a>\n";
    nav += "<a href=\"/learn\">&#x2699;</a>\n";
    nav += "</nav>\n";
    return nav;
}

String HTTPEvent::numbers() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_NUMBERS);
    HID_USAGE_KEY layout[size];
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_NUMBERS, layout);
    String buttons = "<div class=\"buttons col-3\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::dpad() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_DPAD);
    HID_USAGE_KEY layout[size];
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_DPAD, layout);
    String buttons = "<div class=\"buttons col-5 p-block\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::functional() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_FUNCTIONAL);
    HID_USAGE_KEY layout[size];
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_FUNCTIONAL, layout);
    String buttons = "<div class=\"buttons col-3\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::keyboardRow(uint8_t layoutId) {
    const uint8_t size = HIDUsageKeys::getLayoutSize(layoutId);
    HID_USAGE_KEY layout[size];
    HIDUsageKeys::getLayout(layoutId, layout);
    String buttons = "<div>\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::media() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_MEDIA);
    HID_USAGE_KEY layout[size];
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_MEDIA, layout);
    String buttons = "<div class=\"buttons col-4\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::colors() {
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_COLORS);
    HID_USAGE_KEY layout[size];
    HIDUsageKeys::getLayout(LAYOUT_REMOTE_COLORS, layout);
    String buttons = "<div class=\"buttons col-4\">\n";
    for (int16_t i=0; i < sizeof layout/sizeof layout[0]; i++) {
        buttons += button(layout[i]);
    }
    buttons += "</div>\n";
    return buttons;
}

String HTTPEvent::button(HID_USAGE_KEY key) {
    String button = "";
    if (key.type == TYPE_NONE) {
        button += "<span>\n";
        button += key.label;
        button += "</span>\n";
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
