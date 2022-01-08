#include "HAClient.h"

const char *HAClient::LOG_TAG = "HAClient";
bool HAClient::running = false;
bool HAClient::authenticated = false;
long HAClient::seq = 1;
uint8_t HAClient::attempt = 0;

void HAClient::init()
{
    if (BLERC::ha_api_enable && BLERC::ha_ip != "" && BLERC::ha_port > 0 && BLERC::ha_token != "")
    {
        Serial.println("Websocket client initialized...");
    }
}

void HAClient::run()
{
    if (BLERC::ha_api_enable && BLERC::ha_ip != "" && BLERC::ha_port > 0 && BLERC::ha_token != "")
    {
        onEvent(eventHandler);
        this->begin(BLERC::ha_ip, BLERC::ha_port, url, protocol);
        running = true;
        Serial.println("Websocket client running...");
    }
}

void HAClient::eventHandler(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        seq = 1;
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
        break;
    case WStype_PING:
        // Serial.println("ping");
        break;
    case WStype_PONG:
        // Serial.println("pong");
        break;
    }
}

void HAClient::handlePayload(uint8_t *payload)
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
            enableHeartbeat(10000, 1000, 30); // every 10 seconds with a timout of 1 second, diconnect after 5 Minutes
            subscribe();
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
            Serial.printf("HAClient result: %s\n", payload);
            if (seq + 1 >= LONG_MAX)
            {
                instance().disconnect();
            }
            else
            {
                seq++;
            }
        }
        if (strcmp(type, "event") == 0)
        {
            if (jsonBody.hasOwnProperty("event") && jsonBody["event"].hasOwnProperty("data"))
            {
                if (jsonBody["event"].hasOwnProperty("data") && jsonBody["event"]["data"].hasOwnProperty("room"))
                {
                    if (strcmp(jsonBody["event"]["data"]["room"], BLERC::room.c_str()) == 0)
                    {
                        Serial.printf("HAClient event: %s\n", payload);
                    }
                }
            }
        }
    }
}

void HAClient::sendToken()
{
    JSONVar auth;
    auth["type"] = String("auth");
    auth["access_token"] = String(BLERC::ha_token);
    sendTXT(JSON.stringify(auth).c_str());
}

void HAClient::subscribe()
{
    JSONVar subscription;
    subscriptionId = seq;
    subscription["id"] = long(seq);
    subscription["type"] = String("subscribe_events");
    subscription["event_type"] = "ha_to_ble_rc";
    instance().sendTXT(JSON.stringify(subscription).c_str());
}

void HAClient::unsubscribe()
{
    JSONVar unsubscription;
    unsubscription["id"] = long(seq);
    unsubscription["type"] = String("unsubscribe_events");
    unsubscription["subscription"] = long(subscriptionId);
    instance().sendTXT(JSON.stringify(unsubscription).c_str());
}

void HAClient::callService(const char *method, uint8_t protocol, uint64_t code)
{
    if (!running || !authenticated)
    {
        return;
    }
    JSONVar serviceCall;
    serviceCall["id"] = long(seq);
    serviceCall["type"] = String("call_service");
    serviceCall["domain"] = String("script");
    serviceCall["service"] = String("ble_rc_to_ha");
    serviceCall["service_data"] = JSONVar();
    serviceCall["service_data"]["method"] = String(method);
    serviceCall["service_data"]["protocol"] = (int)protocol;
    serviceCall["service_data"]["code"] = (long)code;
    serviceCall["service_data"]["room"] = BLERC::room;
    instance().sendTXT(JSON.stringify(serviceCall).c_str());
}