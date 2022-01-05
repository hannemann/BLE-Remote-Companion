#include "WSClient.h"

const char *WSClient::LOG_TAG = "WSClient";
bool WSClient::running = false;
bool WSClient::authenticated = false;
uint16_t WSClient::seq = 0;

void WSClient::init()
{
#ifdef HA_WS_HOST
    host = HA_WS_HOST;
    port = 8123;
#endif
    // BLERC::preferences.begin("wsclient", true);
    // token = BLERC::preferences.getString("token", "").c_str();
#ifdef HA_WS_TOKEN
    token = HA_WS_TOKEN;
    Serial.println("Websocket client initialized...");
#endif
}

void WSClient::run()
{
#ifdef HA_WS_TOKEN
    if (strcmp(token, "") != 0)
    {
        onEvent(eventHandler);
        this->begin(host, port, url, protocol);
        running = true;
        Serial.println("Websocket client running...");
    }
#endif
}

void WSClient::eventHandler(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        break;
    case WStype_CONNECTED:
        Serial.printf("[WSc] Connected to url: %s\n", payload);
        break;
    case WStype_TEXT:
        Serial.printf("[WSc] get text: %s\n", payload);
        instance().handlePayload(payload);
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

void WSClient::handlePayload(uint8_t *payload)
{
    ESP_LOGD(LOG_TAG, "Payload: %s", payload);
    JSONVar jsonBody = JSON.parse((const char *)payload);
    if (JSON.typeof(jsonBody) == "undefined")
    {
        ESP_LOGE(LOG_TAG, "jsonBody missing");
        return;
    }
    if (jsonBody.hasOwnProperty("type"))
    {
        const char *type = (const char *)jsonBody["type"];
        if (strcmp(type, "auth_required") == 0)
        {
            sendToken();
        }
        if (strcmp(type, "auth_ok") == 0)
        {
            authenticated = true;
            Serial.println("Websocket client authenticated");
        }
        if (strcmp(type, "auth_invalid") == 0)
        {
            authenticated = false;
            Serial.println("Websocket client authentication failed");
        }
        if (strcmp(type, "result") == 0)
        {
            Serial.printf("WSClient result: %s\n", payload);
        }
    }
}

void WSClient::sendToken()
{
    JSONVar auth;
    auth["type"] = String("auth");
    auth["access_token"] = String(token);
    sendTXT(JSON.stringify(auth).c_str());
}

void WSClient::callService(const char *method, uint8_t protocol, uint64_t code)
{
    if (!running || !authenticated)
    {
        return;
    }
    JSONVar serviceCall;
    serviceCall["id"] = long(seq);
    seq++;
    serviceCall["type"] = String("call_service");
    serviceCall["domain"] = String("script");
    serviceCall["service"] = String("ble_rc_ir_in");
    serviceCall["service_data"] = JSONVar();
    serviceCall["service_data"]["method"] = String(method);
    serviceCall["service_data"]["protocol"] = (int)protocol;
    serviceCall["service_data"]["code"] = (long)code;
    serviceCall["service_data"]["room"] = BLERC::room;
    instance().sendTXT(JSON.stringify(serviceCall).c_str());
}