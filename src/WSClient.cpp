#include "WSClient.h"

const char *WSClient::LOG_TAG = "WSClient";
bool WSClient::running = false;
bool WSClient::authenticated = false;
long WSClient::seq = 1;
uint8_t WSClient::attempt = 0;

void WSClient::init()
{
    if (BLERC::ha_api_enable && BLERC::ha_ip != "" && BLERC::ha_port > 0 && BLERC::ha_token != "")
    {
        Serial.println("Websocket client initialized...");
    }
}

void WSClient::run()
{
    if (BLERC::ha_api_enable && BLERC::ha_ip != "" && BLERC::ha_port > 0 && BLERC::ha_token != "")
    {
        onEvent(eventHandler);
        this->begin(BLERC::ha_ip, BLERC::ha_port, url, protocol);
        running = true;
        Serial.println("Websocket client running...");
    }
}

void WSClient::eventHandler(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        break;
    case WStype_CONNECTED:
        Serial.printf("[WSc] Connected to url: ws://%s:%d%s\n", BLERC::ha_ip.c_str(), BLERC::ha_port, payload);
        break;
    case WStype_TEXT:
        ESP_LOGD(LOG_TAG, "[WSc] get text: %s\n", payload);
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
            attempt++;
            Serial.println("Websocket client authentication failed");
            disconnect();
            if (attempt >= maxAttempts)
            {
                running = false;
                WSEvent::instance().broadcastTXT(
                    "{\"error\":401,\"message\":\"Home Assistant websocket authentication failed. Giving up\"}");
            }
            else
            {
                WSEvent::instance().broadcastTXT(
                    "{\"error\":401,\"message\":\"Home Assistant websocket authentication failed.\"}");
            }
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
    auth["access_token"] = String(BLERC::ha_token);
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
    if (seq + 1 >= LONG_MAX)
    {
        seq = 1;
        instance().disconnect();
    }
    else
    {
        seq++;
    }
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