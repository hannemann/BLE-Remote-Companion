#ifndef WSCLIENT_H
#define WSCLIENT_H

#include "BLERC.h"

class WSClient : public WebSocketsClient
{
public:
    WSClient() : WebSocketsClient(){};
    static WSClient &instance()
    {
        static WSClient instance;
        return instance;
    }
    static const char *LOG_TAG;
    void init();
    void run();
    static bool running;
    static bool authenticated;
    static void callService(const char *method, uint8_t protocol, uint64_t code);

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
    void sendToken();
    void handlePayload(uint8_t *payload);
};

#endif // WSCLIENT_H