#include "IRService.h"

bool IRService::mouseMode = false;
int8_t IRService::mouseStep = 5;
int8_t IRService::maxMouseStep = INT8_MAX;

IRrecv IRService::irrecv = IRrecv(IR_PIN);

IRService &IRService::init()
{
    Logger::instance().println("Init IR Service...");
    BLERC::preferences.begin("ir", true);
    Logger::instance().println("Read IR configuration from flash...");
    long start = millis();
    String configJSON = BLERC::preferences.getString("config", "{}");
    config = JSON.parse(configJSON);
    BLERC::preferences.end();
    uint16_t dur = millis() - start;
    Logger::instance().printf("%d Configuration bytes parsed in %dms\n", configJSON.length(), dur);
    return *this;
}

void IRService::run()
{
    irrecv.enableIRIn();
    Logger::instance().printf("IRService waiting for input from pin %d\n", IR_PIN);
}
void IRService::loop()
{
    if (irrecv.decode(&results))
    {
        if (results.value > 0)
        {
            protocol = results.decode_type;
            if (BLERC::ir_ign_unknown && protocol == UNKNOWN)
            {
                return;
            }
            start = millis();
            // ignore NEC repeats
            if (results.value != 0xFFFFFFFFFFFFFFFF)
            {
                current = results.value;
            }
            // remove RC5 toggle bit
            if (results.decode_type == RC5)
            {
                current &= 0xf7ff;
            }
            // remove RC6 toggle bit
            if (results.decode_type == RC6)
            {
                current &= 0xfeffff;
            }
            currentKey = getKeyId();
            currentType = getTypeId();
            currentHid = getHidUsageFromIr();
            bool isMouse = (mouseMode && isDpad());
            bool isClick = (mouseMode && currentHid == DPAD_CENTER);
            if (current != lastSteady && !isMouse && !isClick)
            {
                lastSteady = current;
                // Logger::instance().print(resultToHumanReadableBasic(&results));
                Logger::instance().printf("IR Protocol: %d - %s, Button: %llu pressed\n", results.decode_type, typeToString(results.decode_type, results.repeat).c_str(), current);
                if (!learning.hasOwnProperty("client") && !forgetRemoteBtn.hasOwnProperty("client"))
                {
                    press();
                }
            }
            else if (isMouse)
            {
                mouseMove();
            }
            lastDebounceTime = millis();
        }
        irrecv.resume();
    }
    else
    {
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
            if (millis() - keyStart >= 5000 && currentType == TYPE_KEYBOARD && currentHid == 0x0029) // Escape
            {
                WSEvent::instance().broadcastTXT("{\"notify\":{\"type\":\"info\",\"message\":\"Rebooting... Reload after 20 Seconds\"},\"jsonrpc\":\"2.0\",\"method\":\"reload\",\"params\":{\"timeout\":20000}}");
                delay(1000);
                ESP.restart();
            }
            current = 0;
            lastSteady = current;
            if (start > 0)
            {
                Logger::instance().printf("Button released. Runtime: %lums\n", millis() - start);
                start = 0;
            }
            else
            {
                Logger::instance().println("Button released");
            }
        }
    }
}

void IRService::storeLearned()
{
    Logger::instance().printf("Learning %s\n", JSON.stringify(learning).c_str());
    String key = getConfigKeyFromIr();
    String value = getConfigValue();
    config[key] = value;
    saveConfig();
    printConfig();
    Logger::instance().printf("Learned %s - %s\n", key.c_str(), value.c_str());
    uint8_t client = int(learning["client"]);
    endConfig();
    WSEvent::instance().resultOK(client, "{\"method\":\"learn\",\"result\":\"OK\"}");
}

void IRService::deleteLearned()
{
    String key = getConfigKeyFromIr();
    Logger::instance().printf("Attempt to delete %s from IR config\n", key.c_str());
    if (config.hasOwnProperty(key))
    {
        config[key] = undefined;
        saveConfig();
    }
    printConfig();
    uint8_t client = int(forgetRemoteBtn["client"]);
    endConfig();
    WSEvent::instance().resultOK(client, "{\"method\":\"forget\",\"result\":\"OK\",\"message\":\"IR button cleared\"}");
}

void IRService::endConfig()
{
    forgetRemoteBtn = JSON.parse("{}");
    learning = JSON.parse("{}");
}

String IRService::getConfigKeyFromIr()
{
    char pBuffer[3];
    itoa(protocol, pBuffer, 10);
    char buffer[20];
    itoa(current, buffer, 10);

    return (String)pBuffer + "-" + (String)buffer;
}

String IRService::getConfigValue()
{
    Logger::instance().printf("Obtain config from %s\n", JSON.stringify(learning).c_str());

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
    Logger::instance().printf("Config %s\n", JSON.stringify(config).c_str());
}

JSONVar IRService::getConfig()
{
    return config;
}

void IRService::press()
{
    if (currentType != TYPE_INTERNAL)
    {
        keyStart = millis();
        currentType == TYPE_KEYBOARD ? bluetooth.keydown(currentHid, false) : bluetooth.mediadown(currentHid, false);
        notifyClients(currentHid, "keydown");
    }
}

void IRService::mouseMove()
{
    switch (currentHid)
    {
    case DPAD_UP:
        bluetooth.mouseMove(0, mouseStep * -1);
        break;
    case DPAD_DOWN:
        bluetooth.mouseMove(0, mouseStep);
        break;
    case DPAD_LEFT:
        bluetooth.mouseMove(mouseStep * -1, 0);
        break;
    case DPAD_RIGHT:
        bluetooth.mouseMove(mouseStep, 0);
        break;
    }

    if (mouseStep < maxMouseStep)
    {
        mouseStep = mouseStep + 10 <= maxMouseStep ? mouseStep + 10 : maxMouseStep;
    }
}

void IRService::release()
{
    mouseStep = 5;
    if (currentType != TYPE_INTERNAL)
    {
        if (mouseMode && currentType == TYPE_CONSUMER && currentHid == DPAD_CENTER)
        {
            bluetooth.mouseClick(1);
        }
        else
        {
            bool longpress = millis() - keyStart >= 500;
            currentType == TYPE_KEYBOARD ? bluetooth.keyup() : bluetooth.mediaup();
            notifyClients(currentHid, "keyup", longpress);
        }
    }
    else
    {
        if (currentHid == TOGGLE_MOUSE)
        {
            mouseMode = mouseMode == true ? false : true;
            if (mouseMode)
            {
                bluetooth.mouseMove(1, 0);
            }
        }
    }
}

bool IRService::isDpad()
{
    return currentType == TYPE_KEYBOARD && (currentHid == DPAD_UP || currentHid == DPAD_DOWN || currentHid == DPAD_LEFT || currentHid == DPAD_RIGHT);
}

void IRService::notifyClients(const int16_t key, const char *method, bool longpress)
{
    const char *type = HIDUsageKeys::getKeyType(currentType);
    const char *code = HIDUsageKeys::getKeyName(currentType, currentKey);
    if (key == -1 || BLERC::ws_br_send_assigned)
    {
        WSEvent::instance().broadcastKey(method, type, code, protocol, current, longpress);
    }
    if (key == -1 || BLERC::ha_send_assigned)
    {
        HAClient::callService(method, type, code, protocol, current, longpress);
    }
}

int16_t IRService::getHidUsageFromIr()
{
    return HIDUsageKeys::getKey(currentType, currentKey);
}

uint8_t IRService::getTypeId()
{
    String configKey = getConfigKeyFromIr();
    if (config.hasOwnProperty(configKey))
    {
        String configValue = (const char *)config[configKey];
        return atoi(configValue.substring(0, configValue.indexOf("-")).c_str());
    }
    return 0;
}

uint16_t IRService::getKeyId()
{
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

void IRService::clearConfig()
{
    BLERC::preferences.begin("ir", false);
    BLERC::preferences.clear();
    BLERC::preferences.end();
    config = JSON.parse("{}");
    Logger::instance().println("Configuration cleared...");
}