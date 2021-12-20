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
    Serial.println(ESP.getFreeHeap());
}

void HTTPEvent::learn() {
    Serial.printf("GET /learn\n");
    String response = header();
    response += nav();
    response += "<main class=\"learn\"><section class=\"remote\">\n";
    response += numbers();
    response += functional();
    response += dpad();
    response += media();
    response += colors();
    response += "</section>\n";
    response += "<section class=\"keyboard\">\n";
    response += keyboardRows();
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
    response += functional();
    response += dpad();
    response += "</section>\n";
    response += "<section class=\"keyboard\">\n";
    response += keyboardRows();
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
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-3";
    root["classes"] = classes;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_NUMBERS);
    String buttons;
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteNumbers[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    buttons += "<script>\nvar numbers = ";
    buttons += JSON.stringify(root);
    buttons += ";\n</script>";
    return buttons;
}

String HTTPEvent::dpad() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-5";
    classes[1] = "p-block";
    root["classes"] = classes;
    String buttons;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_DPAD);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteDPad[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    buttons += "<script>\nvar dpad = ";
    buttons += JSON.stringify(root);
    buttons += ";\n</script>";
    return buttons;
}

String HTTPEvent::functional() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-3";
    root["classes"] = classes;
    String buttons;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_FUNCTIONAL);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteFunctional[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    buttons += "<script>\nvar functional = ";
    buttons += JSON.stringify(root);
    buttons += ";\n</script>";
    return buttons;
}

String HTTPEvent::media() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-4";
    root["classes"] = classes;
    String buttons;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_MEDIA);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteMedia[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    buttons += "<script>\nvar media = ";
    buttons += JSON.stringify(root);
    buttons += ";\n</script>";
    return buttons;
}

String HTTPEvent::colors() {
    JSONVar root;
    JSONVar classes;
    JSONVar btns;
    classes[0] = "col-4";
    root["classes"] = classes;
    String buttons;
    const uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_REMOTE_COLORS);
    for (int16_t i=0; i < size; i++) {
        JSONVar btn = buttonJSON(HIDUsageKeys::layoutRemoteColors[i]);
        btns[i] = btn;
    }
    root["buttons"] = btns;
    buttons += "<script>\nvar colors = ";
    buttons += JSON.stringify(root);
    buttons += ";\n</script>";
    return buttons;
}

String HTTPEvent::keyboardRows() {
    uint8_t size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_NUMBERS);
    String buttons = "<div>\n";
    for (int16_t i=0; i < size; i++) {
        buttons += button(HIDUsageKeys::layoutKeyboardNumbers[i]);
    }
    buttons += "</div><div>\n";
    size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_ROW1);
    for (int16_t i=0; i < size; i++) {
        buttons += button(HIDUsageKeys::layoutKeyboardRow1[i]);
    }
    buttons += "</div><div>\n";
    size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_ROW2);
    for (int16_t i=0; i < size; i++) {
        buttons += button(HIDUsageKeys::layoutKeyboardRow2[i]);
    }
    buttons += "</div><div>\n";
    size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_ROW3);
    for (int16_t i=0; i < size; i++) {
        buttons += button(HIDUsageKeys::layoutKeyboardRow3[i]);
    }
    buttons += "</div><div>\n";
    size = HIDUsageKeys::getLayoutSize(LAYOUT_KEYBOARD_ROW4);
    for (int16_t i=0; i < size; i++) {
        buttons += button(HIDUsageKeys::layoutKeyboardRow4[4]);
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

JSONVar HTTPEvent::buttonJSON(HID_USAGE_KEY key) {
    JSONVar button;
    button["key"] = key.index;
    button["type"] = key.type;
    button["label"] = key.label;
    return button;
}