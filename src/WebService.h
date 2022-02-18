#ifndef WIFI_H
#define WIFI_H

#include "BLERC.h"

class WebService
{
public:
    WebService();
    static WebService &instance()
    {
        static WebService instance;
        return instance;
    }
    void init();
    void run();
    void loop();
    bool hasWifiCredentials();
    void saveCredentials(const char *ssid, const char *psk);
    void deleteCredentials();
    static DNSServer dnsServer;
    static bool captiveMode;

private:
    static Preferences preferences;
    const uint16_t reconnectInterval = 30000;
    void wifiHealth();
    uint64_t lastConnectTry = millis();
    uint8_t reconnectTries = 10;
    static void WiFiEvent(WiFiEvent_t event);
    static bool eth_connected;
};

#endif // WIFI_H