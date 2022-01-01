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
    uint16_t dur = millis() - start;
    Serial.printf("%d Configuration bytes parsed in %dms\n", configJSON.length(), dur);
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
                // Serial.print(resultToHumanReadableBasic(&results));
                Serial.printf("IR Protocol: %d - %s, Button: %llu pressed\n", results.decode_type, typeToString(results.decode_type, results.repeat).c_str(), current);
                if (!learning.hasOwnProperty("client") && !forgetRemoteBtn.hasOwnProperty("client"))
                {
                    press();
                }
            }
            lastDebounceTime = millis();
        }
        irrecv.resume();
    } else {
        if ((millis() - lastDebounceTime) > debounce && current > 0) {
            Serial.println("Button released");
            if (learning.hasOwnProperty("client"))
            {
                storeLearned();
            }
            else if (forgetRemoteBtn.hasOwnProperty("client"))
            {
                deleteLearned();
            }
            else
            {
                release();
            }
            current = 0;
            lastSteady = current;
        }
    }
}

void IRService::storeLearned()
{
    Serial.printf("Learning %s\n", JSON.stringify(learning).c_str());
    String key = getConfigKeyFromIr();
    String value = getConfigValue();
    config[key] = value;
    saveConfig();
    printConfig();
    Serial.printf("Learned %s - %s\n", key.c_str(), value.c_str());
    uint8_t client = int(learning["client"]);
    endConfig();
    WSEvent::instance().resultOK(client, "{\"method\":\"learn\",\"result\":\"OK\"}");
}

void IRService::deleteLearned()
{
    String key = getConfigKeyFromIr();
    Serial.printf("Attempt to delete %s from IR config\n", key.c_str());
    if (config.hasOwnProperty(key))
    {
        config[key] = undefined;
        saveConfig();
    }
    printConfig();
    uint8_t client = int(forgetRemoteBtn["client"]);
    endConfig();
    WSEvent::instance().resultOK(client, "{\"method\":\"forget\",\"result\":\"OK\"}");
}

void IRService::endConfig()
{
    forgetRemoteBtn = JSON.parse("{}");
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
    
    String type = (const char*)learning["type"];
    String key = (const char*)learning["key"];

    return type + "-" + key;
}

void IRService::saveConfig()
{
    preferences.begin("ir", false);
    preferences.putString("config", JSON.stringify(config));
    preferences.end();
}

void IRService::printConfig()
{
    Serial.printf("Config %s\n", JSON.stringify(config).c_str());
}

void IRService::press() {
    const int16_t key = getHidUsageFromIr();
    getTypeId() == TYPE_KEYBOARD ? bluetooth.keydown(key, false) : bluetooth.mediadown(key, false);
    WSEvent::instance().broadcastKey(getTypeId(), getKeyId(), "keydown", protocol, current);
}

void IRService::release() {
    getTypeId() == TYPE_KEYBOARD ? bluetooth.keyup() : bluetooth.mediaup();
    WSEvent::instance().broadcastKey(getTypeId(), getKeyId(), "keyup", protocol, current);
}

int16_t IRService::getHidUsageFromIr() {
    return HIDUsageKeys::getKey(getTypeId(), getKeyId());
}

uint8_t IRService::getTypeId() {
    String configKey = (const char*)config[getConfigKeyFromIr()];
    String delimiter = "-";
    uint8_t type = atoi(configKey.substring(0, configKey.indexOf("-")).c_str());
    return type;
}

uint16_t IRService::getKeyId() {
    String configKey = (const char*)config[getConfigKeyFromIr()];
    String delimiter = "-";
    uint16_t key = atoi(configKey.substring(configKey.indexOf("-") + 1).c_str());
    return key;
}

void IRService::clearConfig() {
    preferences.begin("ir", false);
    preferences.clear();
    preferences.end();
    config = JSON.parse("{}");
    Serial.println("Configuration cleared...");
}