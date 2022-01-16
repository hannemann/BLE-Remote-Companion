#ifndef HACLIENT_H
#define HACLIENT_H

#include "BLERC.h"

class HAClient : public WebSocketsClient
{
public:
    HAClient() : WebSocketsClient(){};
    static HAClient &instance()
    {
        static HAClient instance;
        return instance;
    }
    static const char *LOG_TAG;
    void init();
    void run();
    static bool running;
    static bool authenticated;
    static void callService(const char *method, const char *keyType, const char *keyCode, int protocol = -1, long irCode = 0, bool longpress = false);

private:
    const char *host;
    uint16_t port;
    const char *protocol = "homeassistant";
    const char *url = "/api/websocket";
    const char *token;
    static long seq;
    static const uint8_t maxAttempts = 10;
    static uint8_t attempt;
    static void eventHandler(WStype_t type, uint8_t *payload, size_t length);
    long subscriptionId;
    void subscribe();
    void unsubscribe();
    void handleHAEvent(JSONVar &params);
    void sendToken();
    void handlePayload(uint8_t *payload);
    static void incrementSeq();
};

#endif // HACLIENT_H