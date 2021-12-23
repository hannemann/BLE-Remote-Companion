#ifndef WSEVENT_H
#define WSEVENT_H

#include "BLIRC.h"

const int webSocketPort = WS_PORT;

struct IPAddressFail {
    int FailedAttempts;
    time_t lastFailure;
};

class WSEvent
{
    public:
    WSEvent() {};
    static WSEvent& instance()
    {
        static WSEvent instance;
        return instance;
    }
    void init();
    void run();
    void loop();
    static WebSocketsServer webSocket;
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
    static void initIPAddressFailures();
    static void broadcastKey(uint8_t type, uint16_t key, const char *method);

private:
    static IPAddressFail IPAddressFailures[254];
};

#endif // WSEVENT_H