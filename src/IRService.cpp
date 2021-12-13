#include "IRService.h"
#include "BLIRC.h"

IRrecv IRService::irrecv = IRrecv(IR_PIN);
Preferences IRService::preferences = Preferences();

IRService& IRService::init() {
    Serial.println("Init IR Service...");
    preferences.begin("ir", true);
    Serial.println("Read IR configuration from flash...");
    long start = millis();
    String configJSON = preferences.getString("config", "{}");
    config = JSON.parse(configJSON);
    preferences.end();
    Serial.printf("%d Configuration bytes parsed in ", config.length());
    Serial.println((millis() - start));
    return *this;
}

void IRService::run() {
    irrecv.enableIRIn();
    Serial.printf("IRService waiting for input from pin %d\n", IR_PIN);
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
                config[key] = bluetooth.getKeyIndex(learning.c_str());
                preferences.begin("ir", false);
                preferences.putString("config", JSON.stringify(config));
                preferences.end();
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
    if (config.hasOwnProperty(key)) {
        long idx = config[key];
        return (int16_t)idx;
    }
    return -1;
}

void IRService::clearConfig() {
    preferences.begin("ir", false);
    preferences.clear();
    preferences.end();
    config = JSON.parse("{}");
    Serial.println("Configuration cleared...");
}