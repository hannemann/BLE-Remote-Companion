#include "HTTPEvent.h"
#include "page/index.h"

void HTTPEvent::init() {
    Serial.println("Init HTTP server...");
    WebServer server(port);
}

void HTTPEvent::run() {
    server.on("/", home);
    // server.on("/keyboard", keyboard);
    // server.on("/learn", learn);
    server.onNotFound(fourOFour);
    server.begin();
    Serial.printf("Webserver started listening on port %d\n", port);
}

void HTTPEvent::loop() {
    server.handleClient();
}

void HTTPEvent::home() {
    Serial.printf("GET /\n");
    instance().server.send_P(200, "text/html", indexHTML);
    Serial.println(ESP.getFreeHeap());
}

void HTTPEvent::learn() {
    Serial.printf("GET /learn\n");
    String response = header();
    response += nav();
    response += "<main class=\"learn\"><section class=\"remote\">\n";
    response += "</section>\n";
    response += "<section class=\"keyboard\">\n";
    // response += keyboardRows();
    response += "</section>\n";
    response += "<section class=\"config\">\n";
    response += "<label>Learn <input name=\"learn\" type=\"checkbox\"/></label>\n";
    response += "<button name=\"clear\">Clear Configuration</button>\n";
    response += "</section></main>\n";
    response += footer();
    instance().server.send(200, "text/html", response);
}

void HTTPEvent::keyboard() {
    Serial.printf("GET /keyboard\n");
    String response = header();
    response += nav();
    response += "<main><section class=\"remote\">\n";
    response += "</section>\n";
    response += "<section class=\"keyboard\">\n";
    // response += keyboardRows();
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
    footer += "const buttons = [\"numbers\", \"functional\", \"dpad\", \"media\", \"colors\"];\n";
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

JSONVar HTTPEvent::numbers() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-3";
    root["classes"] = classes;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_NUMBERS);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteNumbers[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    return root;
}

JSONVar HTTPEvent::dpad() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-5";
    classes[1] = "p-block";
    root["classes"] = classes;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_DPAD);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteDPad[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    return root;
}

JSONVar HTTPEvent::functional() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-3";
    root["classes"] = classes;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_FUNCTIONAL);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteFunctional[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    return root;
}

JSONVar HTTPEvent::media() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-4";
    root["classes"] = classes;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_MEDIA);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteMedia[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    return root;
}

JSONVar HTTPEvent::colors() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-4";
    root["classes"] = classes;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_COLORS);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteColors[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    return root;
}

JSONVar HTTPEvent::keyboardRows() {
    JSONVar root;
    JSONVar rows;
    JSONVar rowN;
    JSONVar row1;
    JSONVar row2;
    JSONVar row3;
    JSONVar row4;
    uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_NUMBERS);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutKeyboardNumbers[i]);
        rowN[i] = btn;
    }
    // rows[0] = rowN;
    Serial.println(JSON.stringify(rowN));
    Serial.println();
    Serial.println();
    
    size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_ROW1);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutKeyboardRow1[i]);
        row1[i] = btn;
    }
    // rows[1] = row1;
    Serial.println(JSON.stringify(row1));
    Serial.println();
    Serial.println();

    size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_ROW2);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutKeyboardRow2[i]);
        row2[i] = btn;
    }
    // rows[2] = row2;
    Serial.println(JSON.stringify(row2));
    Serial.println();
    Serial.println();

    size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_ROW3);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutKeyboardRow3[i]);
        row3[i] = btn;
    }
    // rows[3] = row3;
    Serial.println(JSON.stringify(row3));
    Serial.println();
    Serial.println();

    size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_ROW4);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutKeyboardRow4[i]);
        row4[i] = btn;
    }
    // rows[4] = row4;
    Serial.println(JSON.stringify(row4));
    // root["rows"] = rows;
    // Serial.println(JSON.stringify(root));
    return root;
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

JSONVar HTTPEvent::buttonJSON(HID_USAGE_KEY key) {
    JSONVar button;
    button["key"] = key.index;
    button["type"] = key.type;
    button["label"] = key.label;
    return button;
}