#include "IRService.h"
#include "BLIRC.h"

IRrecv IRService::irrecv = IRrecv(IR_PIN);

IRService& IRService::init() {
    Serial.println("Init IR Service...");
    return *this;
}

void IRService::run() {
    irrecv.enableIRIn();
    Serial.printf("IRService running on pin %d\n", IR_PIN);
}

void IRService::loop() {
    if (irrecv.decode(&results)) {
        if (results.value > 0) {
            // ignore NEC repeats
            if (results.value != 0xFFFFFFFFFFFFFFFF) {
                current = results.value;
            }
            // remove RC5 toggle bit
            if (results.decode_type == RC5) {
                current &= 0xf7ff;
            }
            // remove RC6 toggle bit
            if (results.decode_type == RC6) {
                current &= 0xfeffff;
            }
            if (current != lastSteady) {
                lastSteady = current;
                Serial.printf("IR Protocol: %d, Button: ", results.decode_type);
                Serial.print(current);
                Serial.println(" pressed");
                press(current);
            }
            lastDebounceTime = millis();
        }
        irrecv.resume();
    } else {
        if ((millis() - lastDebounceTime) > debounce && current > 0) {
            Serial.println("Button released");
            release(current);
            current = 0;
            lastSteady = current;
        }
    }
}

void IRService::press(uint64_t code) {
    int16_t idx = getKeyIndex(code);
    if (idx > -1) {
        JSONMethodToCecType key = JSONMethodToCec[idx];
        key.KeyboardAction == 1 ? bluetooth.keydown(key, false) : bluetooth.mediadown(key, false);
    }
}

void IRService::release(uint64_t code) {
    int16_t idx = getKeyIndex(code);
    if (idx > -1) {
        JSONMethodToCecType key = JSONMethodToCec[idx];
        key.KeyboardAction == 1 ? bluetooth.keyup(key) : bluetooth.mediaup(key);
    }
}

int16_t IRService::getKeyIndex(uint64_t code) {
    int16_t idx = -1;
    switch (code) {
        case 1357:
            idx = bluetooth.getKeyIndex("KEYCODE_DPAD_UP");
            break;
        case 1361:
            idx = bluetooth.getKeyIndex("KEYCODE_DPAD_DOWN");
            break;
        case 1358:
            idx = bluetooth.getKeyIndex("KEYCODE_DPAD_LEFT");
            break;
        case 1360:
            idx = bluetooth.getKeyIndex("KEYCODE_DPAD_RIGHT");
            break;
        case 1359:
            idx = bluetooth.getKeyIndex("KEYCODE_ENTER");
            break;
        case 1363:
            idx = bluetooth.getKeyIndex("KEYCODE_ESCAPE");
            break;
    }
    return idx;
}