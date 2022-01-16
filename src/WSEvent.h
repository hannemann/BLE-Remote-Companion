#ifndef WSEVENT_H
#define WSEVENT_H

#include "BLERC.h"

const int webSocketPort = WS_PORT;

class WSEvent : public WebSocketsServer
{
public:
    WSEvent(uint16_t port);
    static WSEvent &instance()
    {
        static WSEvent instance(WS_PORT);
        return instance;
    }
    static const char *LOG_TAG;
    void init();
    void run();
    void notifyClients(const char *method, const char *type, const char *code, bool longpress = false);
    void broadcastKey(const char *method, const char *type, const char *code, int irProtocol = -1, uint64_t irKey = 0, bool longpress = false);
    void resultOK(uint8_t num, const char *message = nullptr);
    void resultError(uint8_t num, int32_t error = ERROR_UNKNOWN);
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

    enum error
    {
        ERROR_UNKNOWN = -32000,
        BLE_NOT_CONNECTED = -32001,
        PARSE_ERROR = -32700,
        INVALID_REQUEST = -32600,
        METHOD_NOT_FOUND = -32601,
        INVALID_PARAMS = -32602,
        INTERNAL_ERROR = -32603,
    };

private:
    uint64_t requestId;
    unsigned long start = 0;
    bool validatePayload(uint8_t num, JSONVar &payload);
    void handlePayload(uint8_t num, uint8_t *payload);
    void callMethod(uint8_t num, const char *method);
    void callMethod(uint8_t num, const char *method, JSONVar &params);
    void btKeypress(uint8_t num, JSONVar &params);
    void btKeydown(uint8_t num, JSONVar &params);
    void btKeyup(uint8_t num, JSONVar &params);
    void pong(uint8_t num);
};

#endif // WSEVENT_H