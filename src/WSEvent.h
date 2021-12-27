#ifndef WSEVENT_H
#define WSEVENT_H

#include "BLERC.h"

const int webSocketPort = WS_PORT;

struct IPAddressFail {
    int FailedAttempts;
    time_t lastFailure;
};

class WSEvent : public WebSocketsServer
{
public:
    WSEvent(uint16_t port);
    static WSEvent &instance()
    {
        static WSEvent instance(WS_PORT);
        return instance;
    }
    void init();
    void run();
    void broadcastKey(uint8_t type, uint16_t key, const char *method);
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

private:
    bool validatePayload(uint8_t num, JSONVar &payload);
    void handlePayload(uint8_t num, uint8_t *payload);
    void callMethod(uint8_t num, const char *method);
    void callMethod(uint8_t num, const char *method, JSONVar &params);
    void sendButtons(uint8_t num, const char *type);
    void btKeypress(uint8_t num, JSONVar &params);
    void btKeydown(uint8_t num, JSONVar &params);
    void btKeyup(uint8_t num, JSONVar &params);
    void resultOK(uint8_t num);
    void resultError(uint8_t num);
    void pong(uint8_t num);
};

#endif // WSEVENT_H