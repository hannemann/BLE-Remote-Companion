#ifndef WIFI_H
#define WIFI_H

#include "BLIRC.h"

class WebService {
    public:
        WebService();
        static WebService& instance()
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
};

#endif // WIFI_H