#include <Arduino.h>
#include <Arduino_JSON.h>

#include "WebService.h"
#include "Bluetooth.h"
#include "KeyServer.h"

#ifndef BLIRC_H
#define BLIRC_H

class BLIRC
{
    public:
        BLIRC();
        static BLIRC& instance()
        {
            static BLIRC instance;
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

#endif // BLIRC_H
