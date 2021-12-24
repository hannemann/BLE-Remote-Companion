#ifndef BLIRC_H
#define BLIRC_H

#include <Arduino.h>
#include <Arduino_JSON.h>

#ifndef IR_PIN
#define IR_PIN 23
#endif

#ifndef WS_PORT
#define WS_PORT 2339
#endif

#ifndef HTTP_PORT
#define HTTP_PORT 80
#endif

#define DNS_PORT 53

#include <WebServer.h>
#include <WebSocketsServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <iterator>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"

#include <IRrecv.h>
#include <Preferences.h>

#include "Files.h"
#include "Keys.h"

#include "WebService.h"
#include "WSEvent.h"
#include "HTTPEvent.h"
#include "IRService.h"
#include "Bluetooth.h"

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
};

#endif // BLIRC_H
