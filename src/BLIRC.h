#include <Arduino.h>
#include <Arduino_JSON.h>

#include "WebService.h"
#include "Bluetooth.h"
#include "KeyServer.h"
#include "IRService.h"

#ifndef BLIRC_H
#define BLIRC_H

#ifndef IR_PIN
#define IR_PIN 23
#endif

#ifndef WS_PORT
#define WS_PORT 2339
#endif

#ifndef HTTP_PORT
#define HTTP_PORT 80
#endif

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
