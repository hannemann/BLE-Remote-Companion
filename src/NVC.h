#include <Arduino.h>
#include <Arduino_JSON.h>

#include "WebService.h"
#include "Bluetooth.h"
#include "KeyServer.h"

#ifndef NVC_H
#define NVC_H

class NVC
{
    public:
        NVC();
        static NVC& instance()
        {
            static NVC instance;
            return instance;
        }
        void setup();
        void loop();
        const char* getSSID();
        const char* getPSK();
    private:
        const char* ssid = WIFI_SSID;
        const char* psk = WIFI_PASSWORD;
};

#endif // NVC_H
