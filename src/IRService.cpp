#include "IRService.h"
#include "BLIRC.h"

IRrecv IRService::irrecv = IRrecv(IR_PIN);
Preferences IRService::preferences = Preferences();

IRService& IRService::init() {
    Serial.println("Init IR Service...");
    return *this;
}

void IRService::run() {
    preferences.begin("ir", false);
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
                if (learning == "-") {
                    press(current);
                } else {
                    Serial.printf("Learning %s\n", learning.c_str());
                }
            }
            lastDebounceTime = millis();
        }
        irrecv.resume();
    } else {
        if ((millis() - lastDebounceTime) > debounce && current > 0) {
            Serial.println("Button released");
            if (learning == "-") {
                release(current);
            } else {
                char buffer[20];
                itoa(current, buffer, 10);
                const char* key = buffer;
                preferences.putString(key, learning.c_str());
                Serial.printf("Learned %s - ", key);
                Serial.println(preferences.getString(key, "-"));
                learning = "-";
            }
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
    char buffer[20];
    itoa(code, buffer, 10);
    const char* key = buffer;
    const char* keyCode = (preferences.getString(key, "-")).c_str();
    if (strcmp(keyCode, "-") == 0) {
        return -1;
    }
    return bluetooth.getKeyIndex(keyCode);
}