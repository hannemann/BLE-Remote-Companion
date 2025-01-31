#include "WSEvent.h"

const char *WSEvent::LOG_TAG = "WebSocket";

WSEvent::WSEvent(uint16_t port)
    : WebSocketsServer(port){};

void WSEvent::init()
{
    Logger::instance().println("Init Websocket server...");
    onEvent(webSocketEvent);
}

void WSEvent::run()
{
    begin();
    Logger::instance().printf("Websocket started listening on port %d\n", WS_PORT);
}

/**
 * @brief websocket event handler
 *
 * @param num
 * @param type
 * @param payload
 * @param length
 */
void WSEvent::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{

    switch (type)
    {
    case WStype_DISCONNECTED:
        Logger::instance().printf("WEBSOCKET: [%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = WSEvent::instance().remoteIP(num);
        if (strcmp((const char *)payload, "/jsonrpc") != 0)
        {
            WSEvent::instance().disconnect(num);
            ESP_LOGE(LOG_TAG, "[%u] pathname does not match /jsonrpc", num);
            return;
        }
        Logger::instance().printf("WEBSOCKET: [%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        WSEvent::instance().sendTXT(num, BLERC::configJSON);
        JSONVar status;
        status["bleStatus"] = Bluetooth::BLEconnected ? Bluetooth::STATUS_CONNECTED : Bluetooth::STATUS_DISCONNECTED;
        WSEvent::instance().sendTXT(num, JSON.stringify(status).c_str());

        JSONVar remote;
        remote["remote"]["mappings"] = (const JSONVar)BLERC::remoteMappings;
        remote["remote"]["ir"] = (const JSONVar)IRService::instance().getConfig();
        WSEvent::instance().sendTXT(num, JSON.stringify(remote).c_str());
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
        ESP_LOGD(LOG_TAG, "[%u] ping", num);
        WSEvent::instance().pong(num);
    case WStype_PONG:
        ESP_LOGD(LOG_TAG, "[%u] pong", num);
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
        ESP_LOGE(LOG_TAG, "[%u] no method requested", num);
        resultError(num, METHOD_NOT_FOUND);
        return false;
    }
    if (payload.hasOwnProperty("params"))
    {
        JSONVar params = payload["params"];
        if (strcmp(payload["method"], "keypress") == 0 || strcmp(payload["method"], "keyup") == 0 || strcmp(payload["method"], "keydown") == 0 || strcmp(payload["method"], "learn") == 0)
        {
            if (params.hasOwnProperty("type") && (params.hasOwnProperty("key") || params.hasOwnProperty("code")))
            {
                return true;
            }
        }
        if (strcmp(payload["method"], "remap") == 0)
        {
            if (params.hasOwnProperty("old") && params.hasOwnProperty("new"))
            {
                return true;
            }
        }
        if (strcmp(payload["method"], "resetBtn") == 0)
        {
            if (params.hasOwnProperty("default"))
            {
                return true;
            }
        }
        if (strcmp(payload["method"], "mouseup") == 0 || strcmp(payload["method"], "mousedown") == 0 || strcmp(payload["method"], "mouseclick") == 0)
        {
            if (params.hasOwnProperty("button"))
            {
                return true;
            }
        }
        if (strcmp(payload["method"], "mousemove") == 0)
        {
            if (params.hasOwnProperty("x") && params.hasOwnProperty("y") && params.hasOwnProperty("wheel") && params.hasOwnProperty("hWheel"))
            {
                return true;
            }
        }
        if (strcmp(payload["method"], "config") == 0)
        {
            if (params.hasOwnProperty("config"))
            {
                return true;
            }
        }
        ESP_LOGE(LOG_TAG, "[%u] %s params invalid", num, (const char *)payload["method"]);
    }
    if (strcmp(payload["method"], "uptime") == 0 || strcmp(payload["method"], "deleteMappings") == 0 || strcmp(payload["method"], "btDisconnect") == 0 || strcmp(payload["method"], "cancelIr") == 0 || strcmp(payload["method"], "forget") == 0 || strcmp(payload["method"], "clear") == 0 || strcmp(payload["method"], "reboot") == 0 || strcmp(payload["method"], "logon") == 0 || strcmp(payload["method"], "logoff") == 0 || strcmp(payload["method"], "resetWifi") == 0)
    {
        return true;
    }
    ESP_LOGE(LOG_TAG, "[%u] method %s invalid", num, (const char *)payload["method"]);
    resultError(num, INVALID_PARAMS);
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
    ESP_LOGD(LOG_TAG, "[%u] Payload: %s", num, payload);
    JSONVar jsonBody = JSON.parse((const char *)payload);

    if (JSON.typeof(jsonBody) == "undefined")
    {
        ESP_LOGE(LOG_TAG, "[%u] jsonBody missing", num);
        resultError(num, PARSE_ERROR);
        return;
    }
    if (!validatePayload(num, jsonBody))
    {
        ESP_LOGE(LOG_TAG, "[%u] input invalid", num);
        return;
    }
    requestId = long(jsonBody["id"]);
    if (!jsonBody.hasOwnProperty("params"))
    {
        callMethod(num, jsonBody["method"]);
    }
    else
    {
        JSONVar params = jsonBody["params"];
        callMethod(num, jsonBody["method"], params);
    }
    Logger::instance().printf("[%s] Function time was %d\n", LOG_TAG, (int)(millis() - startTime));
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
        resultOK(num, "{\"method\":\"reboot\",\"result\":\"OK\",\"message\":\"Rebooting...\"}");
        ESP.restart();
    }
    if (strcmp(method, "forget") == 0)
    {
        JSONVar params;
        params["client"] = num;
        IRService::instance().forget(params);
        resultOK(num);
    }
    if (strcmp(method, "cancelIr") == 0)
    {
        IRService::instance().endConfig();
        resultOK(num, "{\"method\":\"cancelIr\",\"result\":\"OK\"}");
    }
    if (strcmp(method, "clear") == 0)
    {
        IRService::instance().clearConfig();
        resultOK(num, "{\"method\":\"clear\",\"result\":\"OK\",\"message\":\"IR Config cleared\"}");
    }
    if (strcmp(method, "deleteMappings") == 0)
    {
        BLERC::deleteRemoteMappings();
        resultOK(num, "{\"method\":\"deleteMappings\",\"result\":\"OK\",\"message\":\"Mappings deleted\"}");
    }
    if (strcmp(method, "logon") == 0)
    {
        Logger::instance().setClient(num);
        resultOK(num, "{\"method\":\"log\",\"result\":\"OK\",\"message\":\"Logging active\"}");
    }
    if (strcmp(method, "logoff") == 0)
    {
        Logger::instance().setClient(-1);
        resultOK(num, "{\"method\":\"log\",\"result\":\"OK\",\"message\":\"Logging inactive\"}");
    }
    if (strcmp(method, "resetWifi") == 0)
    {
#ifndef LAN_NETWORK
        WebService::instance().deleteCredentials();
        resultOK(num, "{\"method\":\"log\",\"result\":\"OK\",\"message\":\"Wifi credentials cleared\"}");
#else
        resultError(num, METHOD_NOT_FOUND);
#endif
    }
    if (strcmp(method, "uptime") == 0)
    {
        JSONVar result;
        result["method"] = "uptime";
        result["result"] = "OK";
        result["uptime"] = long(esp_timer_get_time() / 1000);
        resultOK(num, JSON.stringify(result).c_str());
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
        params["client"] = num;
        IRService::instance().learn(params);
        resultOK(num);
    }
    if (strcmp(method, "remap") == 0)
    {
        bool result = BLERC::addRemoteMapping(params);
        if (result)
        {
            char buffer[1024];
            snprintf(buffer, 1024, "{\"remote\":{\"mappings\":%s}}", BLERC::remoteMappingsJSON.c_str());
            WSEvent::instance().sendTXT(num, buffer);
            resultOK(num);
        }
        else
        {
            resultError(num);
        }
    }
    if (strcmp(method, "resetBtn") == 0)
    {
        bool result = BLERC::removeRemoteMapping(params);
        if (result)
        {
            char buffer[127];
            snprintf(buffer, 127, "{\"method\":\"resetBtn\",\"result\":\"OK\",\"message\":\"Button resetted\",\"btn\":\"%s\"}", (const char *)params["default"]);
            resultOK(num, buffer);
        }
        else
        {
            resultError(num);
        }
    }
    if (strcmp(method, "keypress") == 0)
    {
        btKeypress(num, params);
    }
    if (strcmp(method, "keydown") == 0)
    {
        if (strcmp(params["type"], "INTERNAL") != 0)
        {
            btKeydown(num, params);
        }
    }
    if (strcmp(method, "keyup") == 0)
    {
        if (strcmp(params["type"], "INTERNAL") == 0)
        {
            if (strcmp(params["code"], "TOGGLE_MOUSE") == 0)
            {
                IRService::mouseMode = IRService::mouseMode == true ? false : true;
            }
        }
        else
        {
            btKeyup(num, params);
        }
    }
    if (strcmp(method, "mousedown") == 0)
    {
        bluetooth.mouseDown(int(params["button"]));
    }
    if (strcmp(method, "mouseup") == 0)
    {
        bluetooth.mouseUp(int(params["button"]));
    }
    if (strcmp(method, "mouseclick") == 0)
    {
        bluetooth.mouseClick(int(params["button"]));
    }
    if (strcmp(method, "mousemove") == 0)
    {
        bluetooth.mouseMove(int(params["x"]), int(params["y"]), int(params["wheel"]), int(params["hWheel"]));
    }
    if (strcmp(method, "config") == 0)
    {
        BLERC::instance().saveConfig(params);
        resultOK(num, "{\"method\":\"config\",\"result\":\"OK\",\"message\":\"Config saved\"}");
        sendTXT(num, BLERC::configJSON);
    }
}

/**
 * @brief notify clients if applicable
 *
 * @param method
 * @param type
 * @param code
 */
void WSEvent::notifyClients(const char *method, const char *type, const char *code, bool longpress)
{
    int irProtocol = IRService::getIrProtocolByKey(type, code);
    uint64_t irCode = IRService::getIrCodeByKey(type, code);

    if ((irProtocol == 0 && irCode == 0) || BLERC::ws_br_send_assigned)
    {
        broadcastKey(method, type, code, irProtocol, irCode, longpress);
    }
    if ((irProtocol == 0 && irCode == 0) || BLERC::ha_send_assigned)
    {
        HAClient::callService(method, type, code, irProtocol, irCode, longpress);
    }
}

/**
 * @brief send ws broadcast
 *
 * @param type
 * @param key
 * @param irProtocol
 * @param irKey
 */
void WSEvent::broadcastKey(const char *method, const char *type, const char *code, int irProtocol, uint64_t irKey, bool longpress)
{
    if (BLERC::ws_br_enable)
    {
        char message[255];
        snprintf(message, 255, "{\"event\":\"ble_rc_to_ws\",\"data\":{\"room\":\"%s\",\"method\":\"%s\",\"type\":\"%s\",\"code\":\"%s\",\"ir_protocol\":%d,\"ir_code\":%llu,\"longpress\":%s}}", BLERC::room.c_str(), method, type, code, irProtocol, irKey, longpress ? "true" : "false");
        broadcastTXT(message);
    }
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
        bluetooth.keypress(params);
        bool longpress = false;
        if (params.hasOwnProperty("longpress"))
        {
            longpress = bool(params["longpress"]);
        }
        notifyClients("keypress", (const char *)params["type"], (const char *)params["code"], longpress);
        resultOK(num);
        return;
    }
    resultError(num, BLE_NOT_CONNECTED);
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
        start = millis();
        bluetooth.keydown(params);
        notifyClients("keydown", (const char *)params["type"], (const char *)params["code"]);
        resultOK(num);
        return;
    }
    resultError(num, BLE_NOT_CONNECTED);
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
        bool longpress = millis() - start > 500;
        bluetooth.keyup(params);
        notifyClients("keyup", (const char *)params["type"], (const char *)params["code"], longpress);
        resultOK(num);
        return;
    }
    resultError(num, BLE_NOT_CONNECTED);
}

/**
 * @brief send ok message to ws client
 *
 * @param num
 * @param message
 */
void WSEvent::resultOK(uint8_t num, const char *message)
{
    char payload[255];
    if (message == nullptr)
    {
        snprintf(payload, 255, "{\"id\":%llu,\"jsonrpc\":\"2.0\",\"result\":\"OK\"}", requestId);
        sendTXT(num, payload);
    }
    else
    {
        snprintf(payload, 255, "{\"id\":%llu,\"jsonrpc\":\"2.0\",\"result\":%s}", requestId, message);
        sendTXT(num, payload);
    }
}

/**
 * @brief send error message to ws client
 *
 * @param num
 * @param error
 */
void WSEvent::resultError(uint8_t num, int32_t error)
{
    char message[255];
    if (error == PARSE_ERROR || error == INVALID_REQUEST)
    {
        snprintf(message, 255, "{\"id\":null,\"jsonrpc\":\"2.0\",\"error\":%d}", error);
    }
    else
    {
        snprintf(message, 255, "{\"id\":%llu,\"jsonrpc\":\"2.0\",\"error\":%d}", requestId, error);
    }
    sendTXT(num, message);
}

void WSEvent::pong(uint8_t num)
{
    WSclient_t *client = &_clients[num];
    if (clientIsConnected(client))
    {
        String payload = "pong";
        sendFrame(client, WSop_pong, (uint8_t *)payload.c_str(), payload.length());
    }
}