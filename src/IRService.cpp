#include "IRService.h"

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
            protocol = results.decode_type;
            if (current != lastSteady) {
                lastSteady = current;
                Serial.printf("IR Protocol: %d, Button: ", results.decode_type);
                Serial.print(current);
                Serial.println(" pressed");
                if (!learning.hasOwnProperty("key")) {
                    press();
                }
            }
            lastDebounceTime = millis();
        }
        irrecv.resume();
    } else {
        if ((millis() - lastDebounceTime) > debounce && current > 0) {
            Serial.println("Button released");
            if (learning.hasOwnProperty("key")) {
                storeLearned();
            } else {
                release();
            }
            current = 0;
            lastSteady = current;
        }
    }
}

void IRService::storeLearned() {
    Serial.printf("Learning %s\n", JSON.stringify(learning).c_str());
    String key = getConfigKeyFromIr();
    String value = getConfigValue();
    config[key] = value;

    Serial.printf("Config %s\n", JSON.stringify(config).c_str());
    preferences.begin("ir", false);
    preferences.putString("config", JSON.stringify(config));
    preferences.end();

    Serial.printf("Learned %s - %s", key.c_str(), value.c_str());
    Serial.println(preferences.getString(key.c_str()));
    learning = JSON.parse("{}");
}

String IRService::getConfigKeyFromIr() {
    char pBuffer[3];
    itoa(protocol, pBuffer, 10);
    char buffer[20];
    itoa(current, buffer, 10);

    return (String)pBuffer + "-" + (String)buffer;
}

String IRService::getConfigValue() {
    Serial.printf("Obtain config from %s\n", JSON.stringify(learning).c_str());
    
    String layout = (const char*)learning["layout"];
    String key = (const char*)learning["key"];

    return layout + "-" + key;
}

void IRService::press() {
    const HID_USAGE_KEY key = getHidUsageFromIr();
    key.type == TYPE_KEYBOARD ? bluetooth.keydown(key, false) : bluetooth.mediadown(key, false);
}

void IRService::release() {
    const HID_USAGE_KEY key = getHidUsageFromIr();
    key.type == TYPE_KEYBOARD ? bluetooth.keyup(key) : bluetooth.mediaup(key);
}

HID_USAGE_KEY IRService::getHidUsageFromIr() {
    String configKey = (const char*)config[getConfigKeyFromIr()];
    String delimiter = "-";
    int8_t layoutId = atoi(configKey.substring(0, configKey.indexOf("-")).c_str());
    int8_t keyId = atoi(configKey.substring(configKey.indexOf("-") + 1).c_str());

    const uint8_t size = HIDUsageKeys::getLayoutSize(layoutId);
    HID_USAGE_KEY layout[size];
    HIDUsageKeys::getLayout(layoutId, layout);
    return layout[keyId];
}

void IRService::clearConfig() {
    preferences.begin("ir", false);
    preferences.clear();
    preferences.end();
    config = JSON.parse("{}");
    Serial.println("Configuration cleared...");
}