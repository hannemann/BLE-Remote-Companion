#include "IRService.h"

bool IRService::mouseMode = false;

IRrecv IRService::irrecv = IRrecv(IR_PIN);

IRService& IRService::init() {
    Serial.println("Init IR Service...");
    BLERC::preferences.begin("ir", true);
    Serial.println("Read IR configuration from flash...");
    long start = millis();
    String configJSON = BLERC::preferences.getString("config", "{}");
    config = JSON.parse(configJSON);
    BLERC::preferences.end();
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
            start = millis();
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
            if (current != lastSteady && !mouseMode)
            {
                lastSteady = current;
                // Serial.print(resultToHumanReadableBasic(&results));
                Serial.printf("IR Protocol: %d - %s, Button: %llu pressed\n", results.decode_type, typeToString(results.decode_type, results.repeat).c_str(), current);
                if (!learning.hasOwnProperty("client") && !forgetRemoteBtn.hasOwnProperty("client"))
                {
                    press();
                }
            }
            else if (mouseMode)
            {
                press();
            }
            lastDebounceTime = millis();
        }
        irrecv.resume();
    } else {
        if ((millis() - lastDebounceTime) > debounce && current > 0)
        {
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
            if (start > 0)
            {
                Serial.printf("Button released. Runtime: %lums\n", millis() - start);
                start = 0;
            }
            else
            {
                Serial.println("Button released");
            }
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

    uint8_t typeId = HIDUsageKeys::getKeyTypeId((const char *)learning["type"]);
    int16_t keyId = HIDUsageKeys::getKeyIndex(typeId, (const char *)learning["code"]);

    char pBuffer[3];
    itoa(typeId, pBuffer, 10);
    char buffer[20];
    itoa(keyId, buffer, 10);

    return (String)pBuffer + "-" + (String)buffer;
}

void IRService::saveConfig()
{
    BLERC::preferences.begin("ir", false);
    BLERC::preferences.putString("config", JSON.stringify(config));
    BLERC::preferences.end();
}

void IRService::printConfig()
{
    Serial.printf("Config %s\n", JSON.stringify(config).c_str());
}

void IRService::press() {
    const int16_t key = getHidUsageFromIr();
    const uint8_t typeId = getTypeId();
    if (mouseMode && typeId == TYPE_KEYBOARD && (key == DPAD_UP || key == DPAD_DOWN || key == DPAD_LEFT || key == DPAD_RIGHT))
    {
        switch (key)
        {
        case DPAD_UP:
            bluetooth.mouseMove(0, -10);
            break;
        case DPAD_DOWN:
            bluetooth.mouseMove(0, 10);
            break;
        case DPAD_LEFT:
            bluetooth.mouseMove(-10, 0);
            break;
        case DPAD_RIGHT:
            bluetooth.mouseMove(10, 0);
            break;
        }
    }
    else if (!mouseMode && typeId != TYPE_INTERNAL)
    {
        typeId == TYPE_KEYBOARD ? bluetooth.keydown(key, false) : bluetooth.mediadown(key, false);
        notifyClients(key, "keydown");
    }
}

void IRService::release() {
    const uint8_t typeId = getTypeId();
    const int16_t key = getHidUsageFromIr();
    if (!mouseMode && typeId != TYPE_INTERNAL)
    {
        const int16_t key = getHidUsageFromIr();
        typeId == TYPE_KEYBOARD ? bluetooth.keyup() : bluetooth.mediaup();
        notifyClients(key, "keyup");
    }

    else if (mouseMode && typeId == TYPE_KEYBOARD && key == DPAD_OK)
    {
        bluetooth.mouseClick(1);
    }
    else if (typeId == TYPE_INTERNAL)
    {
        if (key == TOGGLE_MOUSE)
        {
            mouseMode = mouseMode == true ? false : true;
        }
    }
}

void IRService::notifyClients(const int16_t key, const char *method)
{
    uint8_t typeId = getTypeId();
    const char *type = HIDUsageKeys::getKeyType(typeId);
    const char *code = HIDUsageKeys::getKeyName(typeId, getKeyId());
    if (key == -1 || BLERC::ws_br_send_assigned)
    {
        WSEvent::instance().broadcastKey(method, type, code, protocol, current);
    }
    if (key == -1 || BLERC::ha_send_assigned)
    {
        HAClient::callService(method, type, code, protocol, current);
    }
}

int16_t IRService::getHidUsageFromIr() {
    return HIDUsageKeys::getKey(getTypeId(), getKeyId());
}

uint8_t IRService::getTypeId() {
    String configKey = getConfigKeyFromIr();
    if (config.hasOwnProperty(configKey))
    {
        String configValue = (const char *)config[configKey];
        return atoi(configValue.substring(0, configValue.indexOf("-")).c_str());
    }
    return 0;
}

uint16_t IRService::getKeyId() {
    String configKey = getConfigKeyFromIr();
    if (config.hasOwnProperty(configKey))
    {
        String configValue = (const char *)config[configKey];
        return atoi(configValue.substring(configValue.indexOf("-") + 1).c_str());
    }
    return 0;
}

String IRService::getAssignedIRDataByKey(const char *type, const char *code)
{
    uint8_t typeId = HIDUsageKeys::getKeyTypeId(type);
    int16_t keyId = HIDUsageKeys::getKeyIndex(typeId, code);

    char pBuffer[3];
    itoa(typeId, pBuffer, 10);
    char buffer[20];
    itoa(keyId, buffer, 10);

    String key = (String)pBuffer + "-" + (String)buffer;
    String result;

    JSONVar conf = instance().config;
    JSONVar configKeys = conf.keys();
    for (uint16_t i = 0; i < configKeys.length(); i++)
    {
        if (String((const char *)conf[configKeys[i]]) == key)
        {
            result = (const char *)configKeys[i];
            break;
        }
    }
    return result.c_str();
}

int IRService::getIrProtocolByKey(const char *type, const char *code)
{
    String configKey = getAssignedIRDataByKey(type, code);
    String delimiter = "-";
    int protocolId = atoi(configKey.substring(0, configKey.indexOf("-")).c_str());
    return protocolId;
}

uint64_t IRService::getIrCodeByKey(const char *type, const char *code)
{
    String configKey = getAssignedIRDataByKey(type, code);
    String delimiter = "-";
    uint16_t irCode = atol(configKey.substring(configKey.indexOf("-") + 1).c_str());
    return irCode;
}

void IRService::clearConfig() {
    BLERC::preferences.begin("ir", false);
    BLERC::preferences.clear();
    BLERC::preferences.end();
    config = JSON.parse("{}");
    Serial.println("Configuration cleared...");
}