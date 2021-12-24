#include "WSEvent.h"

WSEvent::WSEvent(uint16_t port)
    : WebSocketsServer(port){};

void WSEvent::init() {
    Serial.println("Init Websocket server...");
    onEvent(webSocketEvent);
}

void WSEvent::run() {
    begin();
    Serial.printf("Websocket started listening on port %d\n", WS_PORT);
}

/**
 * @brief websocket event handler
 * 
 * @param num 
 * @param type 
 * @param payload 
 * @param length 
 */
void WSEvent::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("WEBSOCKET: [%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = WSEvent::instance().remoteIP(num);
        if (strcmp((const char *)payload, "/jsonrpc") != 0)
        {
            WSEvent::instance().disconnect(num);
            Serial.println("WEBSOCKET: [%u] pathname does not match /jsonrpc");
            return;
        }
        Serial.printf("WEBSOCKET: [%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        break;
    }
    case WStype_TEXT:
        WSEvent::instance().handlePayload(num, payload);
        break;
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
        break;
    }
}

/**
 * @brief validate payload
 * 
 * @param num 
 * @param payload 
 * @return true 
 * @return false 
 */
bool WSEvent::validatePayload(uint8_t num, JSONVar &payload)
{
    if (!payload.hasOwnProperty("method"))
    {
        Serial.printf("WEBSOCKET: [%u] no method requested\n", num);
        return false;
    }
    if (payload.hasOwnProperty("params"))
    {
        JSONVar params = payload["params"];
        if (strcmp(payload["method"], "buttons") == 0)
        {
            if (params.hasOwnProperty("type"))
            {
                return true;
            }
        }
        if (strcmp(payload["method"], "keypress") == 0 || strcmp(payload["method"], "keyup") == 0 || strcmp(payload["method"], "keydown") == 0 || strcmp(payload["method"], "learn") == 0)
        {
            if (params.hasOwnProperty("type") && (params.hasOwnProperty("key") || params.hasOwnProperty("code")))
            {
                return true;
            }
        }
        Serial.printf("WEBSOCKET: [%u] %s params invalid\n", num, (const char *)payload["method"]);
    }
    return false;
}

/**
 * @brief handle payload
 * 
 * @param num 
 * @param payload 
 */
void WSEvent::handlePayload(uint8_t num, uint8_t *payload)
{
    unsigned long startTime;
    startTime = millis();
    Serial.printf("WEBSOCKET: [%u] Payload: %s\n", num, payload);
    JSONVar jsonBody = JSON.parse((const char *)payload);

    if (JSON.typeof(jsonBody) == "undefined")
    {
        Serial.printf("WEBSOCKET: [%u] jsonBody missing\n", num);
        resultError(num);
        return;
    }
    if (!validatePayload(num, jsonBody))
    {
        Serial.printf("WEBSOCKET: [%u] input invalid\n", num);
        resultError(num);
        return;
    }
    if (!jsonBody.hasOwnProperty("params"))
    {
        callMethod(num, jsonBody["method"]);
    }
    else
    {
        JSONVar params = jsonBody["params"];
        callMethod(num, jsonBody["method"], params);
    }
    Serial.printf("Function time was %d\n", (int)(millis() - startTime));
}

/**
 * @brief call method
 * 
 * @param num 
 * @param method 
 */
void WSEvent::callMethod(uint8_t num, const char *method)
{
    if (strcmp(method, "btDisconnect") == 0)
    {
        bluetooth.disconnect();
        resultOK(num);
    }
    if (strcmp(method, "reboot") == 0)
    {
        resultOK(num);
        ESP.restart();
    }
    if (strcmp(method, "clear") == 0)
    {
        IRService::instance().clearConfig();
        resultOK(num);
    }
}

/**
 * @brief call method with params
 * 
 * @param num 
 * @param method 
 * @param params 
 */
void WSEvent::callMethod(uint8_t num, const char *method, JSONVar &params)
{
    if (strcmp(method, "learn") == 0)
    {
        IRService::instance().learn(params);
        resultOK(num);
    }
    if (strcmp(method, "buttons") == 0)
    {
        sendButtons(num, params["type"]);
        resultOK(num);
    }
    if (strcmp(method, "keypress") == 0)
    {
        btKeypress(num, params);
    }
    if (strcmp(method, "keydown") == 0)
    {
        btKeydown(num, params);
    }
    if (strcmp(method, "keyup") == 0)
    {
        btKeyup(num, params);
    }
}

/**
 * @brief fetch buttons by type and send json to ws client
 * 
 * @param num 
 * @param type 
 */
void WSEvent::sendButtons(uint8_t num, const char *type)
{
    JSONVar btns;
    JSONVar result;
    if (strcmp(type, "numbers") == 0)
    {
        btns = HTTPEvent::numbers();
    }
    if (strcmp(type, "functional") == 0)
    {
        btns = HTTPEvent::functional();
    }
    if (strcmp(type, "dpad") == 0)
    {
        btns = HTTPEvent::dpad();
    }
    if (strcmp(type, "media") == 0)
    {
        btns = HTTPEvent::media();
    }
    if (strcmp(type, "colors") == 0)
    {
        btns = HTTPEvent::colors();
    }
    if (strcmp(type, "keyboard") == 0)
    {
        HTTPEvent::keyboardRows();
        return;
    }
    // TODO: send in chunks
    result["buttons"] = btns;
    // sendTXT(num, JSON.stringify(result).c_str());
    WSclient_t *client = &_clients[num];
    if (clientIsConnected(client))
    {
        const char *json = JSON.stringify(result).c_str();
        char *payload = (char *)json;
        sendFrame(client, WSop_text, (uint8_t *)payload, strlen(json), true, false);
    }
}

/**
 * @brief send ws broadcast
 * 
 * @param type 
 * @param key 
 * @param method 
 */
void WSEvent::broadcastKey(uint8_t type, uint16_t key, const char *method)
{
    const char *keyName = HIDUsageKeys::getKeyName(type, key);
    const char *keyType = HIDUsageKeys::getKeyType(type);
    char message[255];
    snprintf(message, 255, "{\"id\":1,\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":{\"type\":\"%s\",\"key\":\"%s\"}}", method, keyType, keyName);
    broadcastTXT(message);
}

/**
 * @brief send keypress to bluetooth client
 * 
 * @param num 
 * @param params 
 */
void WSEvent::btKeypress(uint8_t num, JSONVar &params)
{
    if (Bluetooth::BLEconnected)
    {
        params.hasOwnProperty("code") ? bluetooth.keypressByCode(params) : bluetooth.keypress(params);
        resultOK(num);
        return;
    }
    resultError(num);
}

/**
 * @brief send keydown to bluetooth client
 * 
 * @param num 
 * @param params 
 */
void WSEvent::btKeydown(uint8_t num, JSONVar &params)
{
    if (Bluetooth::BLEconnected)
    {
        bluetooth.keydown(params);
        uint8_t typeId = atoi(params["type"]);
        uint16_t idx = atoi(params["key"]);
        broadcastKey(typeId, idx, "keydown");
        resultOK(num);
        return;
    }
    resultError(num);
}

/**
 * @brief send keyup to bluetooth client
 * 
 * @param num 
 * @param params 
 */
void WSEvent::btKeyup(uint8_t num, JSONVar &params)
{
    if (Bluetooth::BLEconnected)
    {
        bluetooth.keyup(params);
        uint8_t typeId = atoi(params["type"]);
        uint16_t idx = atoi(params["key"]);
        broadcastKey(typeId, idx, "keyup");
        resultOK(num);
        return;
    }
    resultError(num);
}

/**
 * @brief send ok message to ws client
 * 
 * @param num 
 */
void WSEvent::resultOK(uint8_t num)
{
    sendTXT(num, "{\"id\":1,\"jsonrpc\":\"2.0\",\"result\":\"OK\"}");
}

/**
 * @brief send error message to ws client
 * 
 * @param num 
 */
void WSEvent::resultError(uint8_t num)
{
    sendTXT(num, "{\"id\":1,\"jsonrpc\":\"2.0\",\"result\":\"Error\"}");
}
